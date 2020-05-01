class freqCalPoint{
  public:
  float freq;

  double meas_0 ;
  double meas_10;
  double meas_20;
  double meas_30;
  double meas_40;
  double meas_50;

  freqCalPoint( double f , double meas0 , double meas1 , double meas2 , double meas3 , double meas4 , double meas5 ) {
    freq = f;
    meas_0 = meas0;
    meas_10 = meas1;
    meas_20 = meas2;
    meas_30 = meas3;
    meas_40 = meas4;
    meas_50 = meas5;
  }
};


const freqCalPoint calTable[] = {

  freqCalPoint( 10000000 ,  //10Mhz
  0  ,
  -10,
  -20,
  -30,
  -40,
  -50 ),

  freqCalPoint( 50000000 ,  //50Mhz
  0  ,
  -10,
  -20,
  -30,
  -40,
  -50 ),

  freqCalPoint( 100000000 , //100Mhz
  0  ,
  -10,
  -20,
  -30,
  -40,
  -50 ),

  freqCalPoint( 200000000 , //200Mhz
  0  ,
  -10,
  -20,
  -30,
  -40,
  -50 ),

  freqCalPoint( 500000000 , //500Mhz
  0  ,
  -10,
  -20,
  -30,
  -40,
  -50 ),

  //****************************************
  freqCalPoint( maxFrequency , //6Ghz
  0  ,
  -10,
  -20,
  -30,
  -40,
  -50 )

};

int calTable_length = sizeof(calTable) / sizeof(freqCalPoint);

/*

         CAL FUNCTIONS

*/


double calibrateReading( double coarseReading) {

  // find right calibration coefficents based on expected frequency

  int lowerCalPoint;
  int upperCalPoint;

  for ( int i = 0; i < calTable_length; i++) {

    if ( i == 0 ) {  //first run
      if (expectedFrequency <= calTable[i].freq){
        upperCalPoint = 0;
        lowerCalPoint = 0;
      }
    }
    else { // interm

      if ( expectedFrequency >=  calTable[ i - 1 ].freq && expectedFrequency <= calTable[i].freq) {
        lowerCalPoint = i - 1;
        upperCalPoint = i;
      }
    }
  }


  // interpolate an intermediate frequency cal table

  freqCalPoint if_cal = freqCalPoint( expectedFrequency , 
                                      interpolate( calTable[lowerCalPoint].freq , calTable[lowerCalPoint].meas_0  , calTable[upperCalPoint].freq , calTable[upperCalPoint].meas_0  , expectedFrequency),
                                      interpolate( calTable[lowerCalPoint].freq , calTable[lowerCalPoint].meas_10 , calTable[upperCalPoint].freq , calTable[upperCalPoint].meas_10 , expectedFrequency),
                                      interpolate( calTable[lowerCalPoint].freq , calTable[lowerCalPoint].meas_20 , calTable[upperCalPoint].freq , calTable[upperCalPoint].meas_20 , expectedFrequency),
                                      interpolate( calTable[lowerCalPoint].freq , calTable[lowerCalPoint].meas_30 , calTable[upperCalPoint].freq , calTable[upperCalPoint].meas_30 , expectedFrequency),
                                      interpolate( calTable[lowerCalPoint].freq , calTable[lowerCalPoint].meas_40 , calTable[upperCalPoint].freq , calTable[upperCalPoint].meas_40 , expectedFrequency),
                                      interpolate( calTable[lowerCalPoint].freq , calTable[lowerCalPoint].meas_50 , calTable[upperCalPoint].freq , calTable[upperCalPoint].meas_50 , expectedFrequency) );

  double compensatedValue;
  // interpolare coarse power level to mean reding correction
  
  if( coarseReading > 0) compensatedValue = coarseReading;
  else if( isBetween(coarseReading , 0   , -10)) compensatedValue = interpolate( 0   , if_cal.meas_0  , -10 , if_cal.meas_10 , coarseReading);
  else if( isBetween(coarseReading , -10 , -20)) compensatedValue = interpolate( -10 , if_cal.meas_10 , -20 , if_cal.meas_20 , coarseReading);
  else if( isBetween(coarseReading , -20 , -30)) compensatedValue = interpolate( -20 , if_cal.meas_20 , -30 , if_cal.meas_30 , coarseReading);
  else if( isBetween(coarseReading , -30 , -40)) compensatedValue = interpolate( -30 , if_cal.meas_30 , -40 , if_cal.meas_40 , coarseReading);
  else if( isBetween(coarseReading , -40 , -50)) compensatedValue = interpolate( -40 , if_cal.meas_40 , -50 , if_cal.meas_50 , coarseReading);
  else if( coarseReading < - 50) compensatedValue = coarseReading;

  return compensatedValue;
                                      
}
