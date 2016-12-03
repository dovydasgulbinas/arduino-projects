// Code is degined to work on arduino pro mini
//WORKING but sofware serial must be added instead of SERIAL
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
  int v; // verification 0/(all acii values). After pushing data will wait for ACK ACK is all ASCII characters from(1-255)
  int r; // repeat packet 0/(how many repeats). Decides wether the packet will be repeated if not ack has received. How many repeats means that how many times it will repeat a packet until timeOut

} streamToEsp{1, 250, 16, 0, 0, 0.0, 0.0, 0.0, 0.0,0.0,0.0,0.0,0.0,"", 0, 0};

void handShakeFl(char HET = '#', char HAT = '$');

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
handShakeFl(); 
  Serial.println("FOLLOWING!"); //----
}

// the loop routine runs over and over again forever:
void loop() {
  processData();

}


/*
void serialEvent() { //1 0 | 1 a 1 | 1 d 100 p 99|
  while (Serial.available()) {
   // Serial.println("AVALABLET!"); //----
    int in = Serial.parseInt();
    //++??++Serial.readBytesUntil(' ', in, 1); // will read the command untill i // MAKE SURE wether terminator values are removed

  //  Serial.print("Igot: ");//----
    // Serial.println(in); //----
    Serial.read(); // remove space
    if (in == 1) { //ParseParameters| Do one of 3 things: enable, disable, edit parameters //Method here is already selected

      haltSerial();

      char enable = Serial.peek(); // will get 1 or 0 or - paramters
      if (enable == '1') {
        streamToEsp.ena = 1; //enable the StreamToESP
        Serial.read(); //remove the command call
        Serial.println("ENABLIOD!");//----
      }
      else if (enable == '0') {
        streamToEsp.ena = 0; // //Disable the StreamToESP
        Serial.read(); //remove the command call
        Serial.println("DISABLIOD!");//----;

      }
      else { // edit  parameters of the method order does not matter can't be called w/ enabling mode
        haltSerial();
delay(300);
getParam:
        if (Serial.available()) {
          char param = Serial.read(); // gets the parameter id

          switch (param) {
            case 'a': // !!! 1 !!! assuming parse int removes the terminator symbol if I am wrong and Serial.read to remove
              streamToEsp.a = Serial.parseInt();
              Serial.print("a: "); //----
              Serial.println(streamToEsp.a);//----
            Serial.read(); // remove space
              goto getParam;
            case 'p':
              streamToEsp.p = Serial.parseInt();
              Serial.print("p: ");//----
              Serial.println(streamToEsp.p);//----
            Serial.read(); // remove space
              goto getParam;
            case 'd':
              streamToEsp.d = Serial.parseInt();
             Serial.print("d: ");//----
              Serial.println(streamToEsp.d);//----
            Serial.read(); // remove space
              goto getParam;
            case 'v':
              streamToEsp.v = Serial.parseInt();
              Serial.print("v: ");//----
              Serial.println(streamToEsp.v);//----
            Serial.read(); // remove space
              goto getParam;
            case 'r':
              streamToEsp.r = Serial.parseInt();
              Serial.print("r: ");//----
              Serial.println(streamToEsp.r);//----
            Serial.read(); // remove space
              goto getParam;
            default:
              Serial.println("Invalid param"); //---~
              clearSerial();
          }

        }
      }//ELSE cmd 1
    } //parseParams
    else {
     Serial.println("NO COMMAND BLYEAT!"); //----
      clearSerial();
    }//else
  }//while Serial available
}//Serial event
*/

void clearSerial() {
  while (Serial.available()) { // clears the serial buffer if wrong command was called
    Serial.read();
  }
}
void haltSerial() {
  while (!Serial.available()) { // will wait intill there will be bytes in the buffer
  }
}

void handShakeFl(char HET, char HAT){ // followers handshake function
 while(true){//
 clearSerial();
 haltSerial();
 if(Serial.read()==HET){
 clearSerial(); 
 Serial.write(HAT);
 break;
 }
}//while
}



void processData() {
  if (streamToEsp.ena == 1) {
    if (((millis() - streamToEsp.lc) >= streamToEsp.p) || (streamToEsp.ln == 0)) { // this means that p ms has passed since the last measurment and not enought messuremnt were taken
      if ((streamToEsp.ln < streamToEsp.n)) { // grouping parameters by quickest receive time fastest at the top
        streamToEsp.avgMoi += 1.01;//====analogRead(SH) / streamToEsp.n;
        streamToEsp.avgLum += 2.02;//====outputLux(BH1750_address) / streamToEsp.n; //get luminosity //++++
        streamToEsp.avgHum += 3.03;//====dht.readHumidity() / streamToEsp.n;//++++
        streamToEsp.avgTemp += 4.04;//====dht.readTemperature()// / streamToEsp.n;//++++
        //...
        streamToEsp.ln ++;
  //++++++++++      analogWrite(LED, map(streamToEsp.ln, 0, streamToEsp.n, 0, 255));
        streamToEsp.lc = millis();

      } // if ln<n
      else { //.ln == 8 pushData, reset ln =0,cast to int,parseString, reset all values
        //GET MEAN
        streamToEsp.avgMoi /= streamToEsp.n;
        streamToEsp.avgLum /= streamToEsp.n;
        streamToEsp.avgHum /= streamToEsp.n;
        streamToEsp.avgTemp /= streamToEsp.n;

        streamToEsp.cData = "1:";
        streamToEsp.cData += String(streamToEsp.avgTemp);
        streamToEsp.cData += "|";
        streamToEsp.cData += String(streamToEsp.avgHum);
        streamToEsp.cData += "|";
        streamToEsp.cData += String(streamToEsp.avgLum);
        streamToEsp.cData += "|";
        streamToEsp.cData += String(streamToEsp.avgMoi);
        streamToEsp.cData += "|";
        Serial.println(streamToEsp.cData);
        //Reasign Variables

        streamToEsp.avgMoi = streamToEsp.pavgMoi;
        streamToEsp.avgLum = streamToEsp.pavgLum;
        streamToEsp.avgHum = streamToEsp.pavgHum;
        streamToEsp.avgTemp = streamToEsp.pavgTemp;

        streamToEsp.avgMoi = 0.0;
        streamToEsp.avgLum = 0.0;
        streamToEsp.avgHum = 0.0;
        streamToEsp.avgTemp = 0.0;
        streamToEsp.ln = 0;
      } // else ln<n
    } // if millis ....
  }// if ena
  else {
  }
} //processData

