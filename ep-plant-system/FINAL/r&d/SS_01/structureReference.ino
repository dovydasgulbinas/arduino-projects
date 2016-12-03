struct strToEsp {
  int ena ; //enabled. indicator of whether or not function i
  int p; //measurment period. A time mcu waits to take in new measurment in MS
  int n; //number of measurment user wants to take
  int ln; // last number. of measurment taken
  long lc; // last call. last time function was called

  float avgTemp; // average temperature
  float avgHum; // average humidity
  float avgLum; //average luminosity
  float avgMoi;// average moisture

  float pavgTemp; // average temperature
  float pavgHum; // average humidity
  float pavgLum; //average luminosity
  float pavgMoi;// average moisture

  String cData;// current piece of data aggregated
} streamToEsp{1, 2000, 8, 0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, ""};
