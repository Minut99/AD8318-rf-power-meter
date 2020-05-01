
// keypad
const int r1 = PB12,
          r2 = PB13,
          r3 = PB14,
          r4 = PB15,

          c1 = PA11,
          c2 = PA12,
          c3 = PA15,
          c4 = PB4;


const int rows = 4,
          cols = 4;



const String keyMap[rows][cols] = {
  { "D" , "C" , "B" , "A" },
  { "E" , "9" , "6" , "3" },
  { "0" , "8" , "5" , "2" },
  { "." , "7" , "4" , "1" }
};


const int rowMap[rows] = {
  r1 , r2 , r3 , r4
};
const int colMap[cols] = {
  c1 , c2 , c3 , c4
};


String hotKeys_enter = "E",
       hotKeys_dBm = "A",
       hotKeys_mV = "B",
       hotKeys_uW = "C",
       hotKeys_unused = "D";





void keyboardSetup() {

  for ( int r = 0; r < rows; r++) {
    pinMode( rowMap[r] , OUTPUT);
  }
  for ( int c = 0; c < cols; c++) {
    pinMode( colMap[c] , INPUT_PULLUP);
  }
}




String checkKey() {
  String pressed = "-"; // default return char

  for ( int row = 0; row < rows; row++) {

    //setPins
    for ( int rowSet = 0; rowSet < rows; rowSet++) {
      boolean state = true;
      if ( rowSet == row ) state = false;
      digitalWrite( rowMap[rowSet] , state);

    }
    delayMicroseconds(10);

    //scan
    for ( int col = 0; col < cols; col++) {
      if ( !digitalRead( colMap[col] )) {
        pressed = keyMap[row][col];
        delay(70); //debounce
      }
    }
  }

  return pressed;

}
