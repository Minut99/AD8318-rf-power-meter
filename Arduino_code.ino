
/*

   written for STM32F103C8T6

   RF power meter

   by : Manuel Minutello

   V0.1 : 06/04/2020
   V0.2 : 20/05/3030 ( this <- )

   !!!!! STILL IN BETA !!!!!

   loock appnote to view changenote and bug to be fixed

*/

// variabili globali


const double maxFrequency = 6000000000; // limiting value for the meter
const double minFrequency =    1000000;

const double minMeasuredPower = -55; //dBm
const double maxMeasuredPower = +5;

const int screenFPS = 2;
String selectedUnit = "";
double expectedFrequency = 10000000; //10Mhz, updatable woth keyboard


const double measureBuffer_fillTime = 1000000L / screenFPS;                                          // fill buffer  in time for next screen refresh
const int measureBuffer_length = 100;                                                        // number of sample in buffer
const double measureBuffer_sampleTime = (measureBuffer_fillTime / measureBuffer_length) * 0.9 ; // interval between samples  and *0.9 to enshoure sample fill between screen outputs
double measureBuffer[measureBuffer_length];


long lastBufferFill = 0;
int bufferCounter = 0;
boolean bufferFull = true;

String outputReading = "   ";


#include"math.h"
#include"keyboard.h"
#include"ui.h"
#include"calibration_table.h"
#include"sensor_interfacing.h"


void setup() {

  selectedUnit = units_dBm;
  keyboardSetup();

  lcdSetup();

  Serial.begin(9600);
}


void loop() {
  /******************************** buffer */

  if ( micros() > lastBufferFill + measureBuffer_sampleTime ) {
    lastBufferFill = micros();

    // add buferPoint
    if ( bufferCounter < measureBuffer_length ) {
      measureBuffer[bufferCounter] = measure();
      bufferCounter ++;
    }
    else {
      bufferFull = true;
      Serial.println("OVFL");
    }
    
    
  }

  /******************************/

  if ( bufferFull ) {
    bufferFull = false;
    bufferCounter = 0; 

    outputReading = "   ";

    double measuredPower = getBufferMed();


    // checkLimit
    if ( measuredPower > maxMeasuredPower) {
      outputReading += " " + overload;
    }

    else if ( measuredPower < minMeasuredPower) {
      outputReading += lowReading;
      outputReading += " ";
      outputReading += selectedUnit;
    }

    else {
      if ( measuredPower > 0) outputReading += " "; // clear negative sign place

      if ( selectedUnit == units_dBm ) outputReading += String(measuredPower);
      else if ( selectedUnit == units_mV) outputReading += String(convertTo_mV(measuredPower));
      else if ( selectedUnit == units_uW) outputReading += String(convertTo_uW(measuredPower));

      outputReading += " ";
      outputReading += selectedUnit;
    }



  }
  printScreen(outputReading);



  //check keyboard

  String K = checkKey();

  if ( K != "-" ) {

    if ( K == hotKeys_dBm ) selectedUnit = units_dBm;
    else if ( K == hotKeys_mV ) selectedUnit = units_mV;
    else if ( K == hotKeys_uW ) selectedUnit = units_uW;
    else if ( K == hotKeys_unused) lcd.clear(); // do nothing but show sign of life
    else if ( K != "." && K != hotKeys_enter ) {

      // enter in while mode to accept new frequency

      boolean input = true;
      String lastKeyPress = "-";
      String newData = "";
      boolean decimalPoint = false;

      while (input) {

        K = checkKey(); // update

        if ( isNumber(K) ) {
          if ( K != lastKeyPress && K != "-") {

            if ( K != ".") newData += K;
            else if ( !decimalPoint) { // no double decimal point
              newData += K;
              decimalPoint = true;
            }
          }
          lastKeyPress = K;
        }
        else if ( K == hotKeys_enter ) input = false;

        if ( lastKeyPress != "-" && K == "-" ) lastKeyPress = "-";

        displayInput( newData + units_MHz); // update display
      }

      double newFreq = newData.toFloat();
      expectedFrequency = newFreq * pow(10 , 6);

      //constrain
      if ( expectedFrequency < minFrequency) expectedFrequency = minFrequency;
      if ( expectedFrequency > maxFrequency) expectedFrequency = maxFrequency;

    }
  }
}



