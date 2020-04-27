


String units_dBm = "dBm",
       units_mV = "mV",
       units_mW = "mW",
       units_MHz = "MHz",
       units_GHz = "GHz";




String overload   = "OVLD!";
String lowReading = "-----";


long lastRefreshTime = 0;


//lcd
const int lcd_rs = PB11,
          lcd_en = PB10,
          lcd_d4 = PB1,
          lcd_d5 = PB0,
          lcd_d6 = PA7,
          lcd_d7 = PA6 ;

const int lcd_char = 16;
const int lcd_cols = 2 ;

#include <LiquidCrystal.h>
LiquidCrystal lcd ( lcd_rs , lcd_en , lcd_d4 , lcd_d5 , lcd_d6 , lcd_d7 );


void lcdSetup() {
  lcd.begin(lcd_char , lcd_cols);
}


void printScreen(String val) {

  if ( millis() > lastRefreshTime + 1000 / screenFPS) {

    lastRefreshTime = millis();

    lcd.clear();

    String firstLine = "f: ";

    firstLine += String( expectedFrequency / pow(10 , 6));
    firstLine += units_MHz;

    lcd.setCursor(0, 0);
    lcd.print(firstLine);

    lcd.setCursor(0, 1);
    lcd.print(val);

  }
}

void displayInput(String data) {

  if ( millis() > lastRefreshTime + 1000 / screenFPS) {
    lastRefreshTime = millis();
    lcd.clear();

    lcd.setCursor( 0 , 0);
    lcd.print ( data + " MHz");

    lcd.setCursor(0, 1);
    lcd.print( "press E to enter" );
  }
}
