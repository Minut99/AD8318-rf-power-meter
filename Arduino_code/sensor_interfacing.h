

float compensationVoltage = 1.2330; // LM385 voltage reference

// sensor voltage limiting 
const float underPower_threshold = 0.1;
const float overPower_threshold = 3.2;
const float sensorLowerPowerRange = -60;
const float sensorUpperPowerRange = 0;

// coarse power reading
const float Vout_firstCalPoint = 0.608; // 0dBm
const float Vout_secondCalPoint = 1.803;// -50dBm
const float power_firstCalPoint = 0;    //dBm
const float power_secondCalPoint = -50; //dBm

//ADC
const float ADC_resolution = 4096;
const float ADC_Vmax = 3.3;

//PINS
const int analog_sensor = PA0;
const int analog_reference = PA1;

//
const int overPower_read = 99.99;
const int underPower_read = -99.99;

const int overWatt_read = 9999;
const int underWatt_read = 0;

const int overVolt_read = 9999;
const int underVolt_read = 0;



float measure(){

  // ADC calibration

  float refVoltage = analogRead(analog_reference) * (ADC_Vmax / ADC_resolution);
  float voltageCalFactor = refVoltage - compensationVoltage;

  float sensorVoltage = analogRead(analog_sensor) * (ADC_Vmax / ADC_resolution);

  // calibrate sensor voltage reading
  sensorVoltage += voltageCalFactor;


  // interpolate to get a coarse power reading from ideal linear curve
  float coarseReading = interpolate(Vout_firstCalPoint , power_firstCalPoint , // calibration point at sensor dynamic range begin
                                    Vout_secondCalPoint , power_secondCalPoint , // .... @ end of dynamic range
                                    sensorVoltage); // readValue

  // apply calibration table
  float calibratedReading = calibrateReading( coarseReading ); // calibrate

  return calibratedReading;
}
