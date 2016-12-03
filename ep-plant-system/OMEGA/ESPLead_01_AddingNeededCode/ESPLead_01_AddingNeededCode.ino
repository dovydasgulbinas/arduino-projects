// Code is degined to work on arduino pro mini

void handShakeLd(char HET = '#', char HAT = '$', long SLP = 350);
// the setup routine runs once when you press reset:
void setup() {
  handShakeLd();
  Serial.begin(9600);


  
}

// the loop routine runs over and over again forever:
void loop() {
  Serial.println("LEADING!");

}



void serialEvent() {
  while (Serial.available()) {


  }//while Serial available
}//Serial event



