
/*
 * 
 * written for STM32F103C8T6
 * 
 * RF power meter
 * 
 * by : Manuel Minutello
 * 
 * 06/04/2020
 * 
 * !!!!! STILL IN BETA !!!!!
 * 
 */ 

// variabili globali


const float maxFrequency = 6000000000; // limiting value for the meter
const float minFrequency =    1000000;

const float minMeasuredPower = -55; //dBm
const float maxMeasuredPower = +5;


const int screenFPS = 3;
String selectedUnit = "";
float expectedFrequency = 10000000; //10Mhz, updatable woth keyboard


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

  float measuredPower = measure();
  String outputReading = "   ";

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
    else if ( selectedUnit == units_mW) outputReading += String(convertTo_mV(measuredPower));

    outputReading += " ";
    outputReading += selectedUnit;
  }

  printScreen(outputReading);


  //check keyboard

  String K = checkKey();

  if ( K != "-" ) {

    if ( K == hotKeys_dBm ) selectedUnit = units_dBm;
    else if ( K == hotKeys_mV ) selectedUnit = units_mV;
    else if ( K == hotKeys_mW ) selectedUnit = units_mW;
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
          if ( K != lastKeyPress && K != "-"){
            
            if( K != ".") newData += K;
            else if( !decimalPoint) { // no double decimal point
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

      float newFreq = newData.toFloat();
      expectedFrequency = newFreq * pow(10 , 6);

      //constrain
      if( expectedFrequency < minFrequency) expectedFrequency = minFrequency;
      if( expectedFrequency > maxFrequency) expectedFrequency = maxFrequency;

    }
  }
}
