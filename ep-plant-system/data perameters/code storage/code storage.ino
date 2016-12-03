
// Code is degined to work on arduino pro mini
struct streamToEsp{
 int a; //averaging 0/1. should it be enabled or not 
 int p; //measurment period. A time mcu waits to take in new measurment
   int d; //duration a time interval for which whoom the measurmetns are done
 int v; // verification 0/(all acii values). After pushing data will wait for ACK ACK is all ASCII characters from(1-255)
  int r; // repeat packet 0/(how many repeats). Decides wether the packet will be repeated if not ack has received. How many repeats means that how many times it will repeat a packet until timeOut
   
}


// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
 // pinMode(led, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}



void serialEvent(){
char in;
  Serial.readBytesUntil(' ',,1); // will read the command untill i // MAKE SURE wether terminator values are removed  
  
  
switch (in){
  
case 1: //enable the method
serial.read() // remove space
char ena = serial.peek(); // will get 1 or 0 or -
if(ena == '1') 
 streamToEsp.ena = 1;
else if (ena == '0')
streamToEsp.ena = 0;
 else { // edit  parameters of the method order does not matter can't be called w/ enabling mode
while(Serial.available()){
char param = Serial.read(); // gets the parameter id

switch (param){
case: 'a' // !!! 1 !!! assuming parse int removes the terminator symbol if I am wrong and Serial.read to remove 
streamToEsp.a = Serial.parseInt();
case: 'p'
streamToEsp.p = Serial.parseInt();
case: 'd'
streamToEsp.d = Serial.parseInt();
case: 'b'
streamToEsp.b = Serial.parseInt();
case: 'v'
streamToEsp.v = Serial.parseInt();
case: 'r'
streamToEsp.r =Serial.parseInt();
default: 
Serial.println(”Invalid param”);
}

}

 }       
break;
case 2:
break;
…
case Nn
break;
}
  
  
}
