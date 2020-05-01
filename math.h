double interpolate( double x1 , double y1 , double x2 , double y2 , double x){

  double m;
  double q;
  
  if( x1 != x2 ){
    m = ( y1 - y2 ) / ( x1 - x2);
    q =(( x1 * y2 ) - ( x2 * y1)) / ( x1 - x2);
  }
  else{
    return y1;
  }

  return m * x + q;
}


boolean isBetween (double  val , double a , double b){
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


double convertTo_mV ( double val) {
  return pow(10, -3) * pow(10 , (val - 10) / 20) * sqrt(2);
  //     0.001               Vpk                    to rms
}


double convertTo_uW (double val) {
  return pow(10 , val/10) * pow(10 , -3);
}


double getBufferMed(){

  // sum all values
  double sum = 0;
  for( int i = 0; i < measureBuffer_length; i++){
    sum += measureBuffer[i];
  }

  
  //divide by samples
  sum /= measureBuffer_length;

  


  bufferFull = false;
  return sum;
}

