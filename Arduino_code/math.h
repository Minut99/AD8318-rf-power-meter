float interpolate( float x1 , float y1 , float x2 , float y2 , float x){

  float m;
  float q;
  
  if( x1 != x2 ){
    m = ( y1 - y2 ) / ( x1 - x2);
    q =(( x1 * y2 ) - ( x2 * y1)) / ( x1 - x2);
  }
  else{
    return y1;
  }

  return m * x + q;
}


boolean isBetween (float  val , float a , float b){
  if( a > b ){
    if( val <= a && val >= b) return true;
    else return false;
  }
  else{
    if( val <= b && val >= b) return true;
    else return false;
  }
}


boolean isNumber( String n ){
  if( n == "1" || n == "2" || n == "3" || n == "4" || n == "5" ||
      n == "6" || n == "7" || n == "8" || n == "9" || n == "0" || n == ".") return true;
  else return false;
}
float convertTo_mV ( float val) {
  return val;
}

float convertTo_mW (float val) {
  return val;
}
