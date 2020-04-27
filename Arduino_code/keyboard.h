
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
  { "1" , "2" , "3" , "A" },
  { "4" , "5" , "6" , "B" },
  { "7" , "8" , "9" , "C" },
  { "." , "0" , "E" , "D" }
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
       hotKeys_mW = "C",
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
  String pressed = "-";

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
