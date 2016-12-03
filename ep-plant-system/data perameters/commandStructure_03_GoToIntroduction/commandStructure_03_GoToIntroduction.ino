// Code is degined to work on arduino pro mini
struct strToEsp {
  int ena ; //enabled. indicator of whether or not function is
  int a; //averaging 0/1. should it be enabled or not
  int p; //measurment period. A time mcu waits to take in new measurment
  int d; //duration a time interval for which whoom the measurmetns are done
  int v; // verification 0/(all acii values). After pushing data will wait for ACK ACK is all ASCII characters from(1-255)
  int r; // repeat packet 0/(how many repeats). Decides wether the packet will be repeated if not ack has received. How many repeats means that how many times it will repeat a packet until timeOut
} streamToEsp{0, 0, 0, 0, 0, 0};


// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  // initialize the digital pin as an output.
  // pinMode(led, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {

}



void serialEvent() { //1 0 | 1 a 1 | 1 d 100 p 99|
  while (Serial.available()) {
    Serial.println("AVALABLET!");
    int in = Serial.parseInt();
    //++??++Serial.readBytesUntil(' ', in, 1); // will read the command untill i // MAKE SURE wether terminator values are removed

    Serial.print("Igot: ");
    Serial.println(in);
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
              Serial.println("Invalid param");
              clearSerial();

          }

        }
      }//ELSE cmd 1


    } //parseParams
    else {
      Serial.println("NO COMMAND BLYEAT!");
      clearSerial();
    }//else
  }//while Serial available
}//Serial event

void clearSerial() {
  while (Serial.available()) { // clears the serial buffer if wrong command was called
    Serial.read();
  }
}
void haltSerial() {
  while (!Serial.available()) { // will wait intill there will be bytes in the buffer
  }
}

