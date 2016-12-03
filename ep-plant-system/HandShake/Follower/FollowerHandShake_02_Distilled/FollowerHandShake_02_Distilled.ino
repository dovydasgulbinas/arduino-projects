// Code is degined to work on arduino pro mini
struct strToEsp {
  int ena ; //enabled. indicator of whether or not function is
  int a; //averaging 0/1. should it be enabled or not
  int p; //measurment period. A time mcu waits to take in new measurment
  int d; //duration a time interval for which whoom the measurmetns are done
  int v; // verification 0/(all acii values). After pushing data will wait for ACK ACK is all ASCII characters from(1-255)
  int r; // repeat packet 0/(how many repeats). Decides wether the packet will be repeated if not ack has received. How many repeats means that how many times it will repeat a packet until timeOut
} streamToEsp{0, 0, 0, 0, 0, 0};

void handShakeFl(char HET = '#', char HAT = '$');

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
handShakeFl(); 
}

// the loop routine runs over and over again forever:
void loop() {
Serial.println("FOLLOWING!"); //----
}





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

