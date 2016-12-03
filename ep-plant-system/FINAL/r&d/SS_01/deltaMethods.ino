int changeInValue(float cValue, float pMean, int trigger ) { //current value received after measuring, last calculated mean , change value which will induce the event

  long delta = (long)(cValue - pMean);

  if ((delta > 0) && (delta >= trigger)) { // trigger value was reached and the change is positive
    return 1;
  }
  else if ((delta < 0) && (delta >= trigger)) { // trigger value was reached and the change is neagtive
    return -1;
  }
  else return 0;
}

/* REFFERED STRUCTURE IS FROM processDataREwrittenForFloat this structure should be reduced in order to accomodaste only mean values needed(pluminosity and pmoisture)
 * struct strToEsp {
  int ena ; //enabled. indicator of whether or not function i
  int p; //measurment period. A time mcu waits to take in new measurment in MS
  int n; //number of measurment user wants to take
  int ln; // last number. of measurment taken
  long lc; // last call. last time function was called

  float avgTemp; // average temperature
  float avgHum; // average humidity
  float avgLum; //average luminosity
  float avgMoi;// average moisture

  float pavgLum; //average luminosity
  float pavgMoi;// average moisture

  String cData;// current piece of data aggregated
  int v; // verification 0/(all acii values). After pushing data will wait for ACK ACK is all ASCII characters from(1-255)
  int r; // repeat packet 0/(how many repeats). Decides wether the packet will be repeated if not ack has received. How many repeats means that how many times it will repeat a packet until timeOut

 int PLT; //positice luminostiy trigger
 int NLT; // negative luminostiy trigger
 int PMT; // postiive moisture trigger

} streamToEsp{1, 2000, ,240, 0, 0.0, 0.0, 0.0, 0.0,0.0,0.0,"", 0, 0,100,1000,-1000};//measuring period 6minutes
 */


////Must be called at measuring processDataMethod. Process data
// Luminosity case

if (changeInValue(, streamToEsp.pavgLum, PLT) == 1) { // positice sufficient change
  Serial.println("l:11");
}
else if (changeinValue(, streamToEsp.pavgLum, NLT) == -1) { // negatice sufficient chagbe
  Serial.println("l:12");
}
if (changeInValue(, streamToEsp.pavgLum, PMT) == 1) { // positive suffient change in moisture
  Serial.println("m:21");
}


//////////////////////////////////////



