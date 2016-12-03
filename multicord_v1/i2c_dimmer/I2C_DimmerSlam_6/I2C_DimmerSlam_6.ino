
#include <Wire.h>
#include  <TimerOne.h>

#define socketPin_B0 (3) // Socket pin BASIC0
#define socketPin_B1 (4) // Socket pin BASIC1
#define socketPin_D0 (5) //Socket pin w/ DIMMER0
#define dimmerPin (6) //nuspresti veliau
#define interruptPin_1 (2)// pin 0 hardware interrupt used for dimmer circuit

///NON pin constants
#define OFF (0)
#define ON  (1)
#define DIM (2)
#define I2C_ID (4)
////
///DIMMER VARIABLES
volatile byte increment = 0;             // Variable to use as a counter
volatile boolean zero_cross = 0; // Stores state of zero cross true means zero cross was present
volatile boolean powerDimmer = false;
volatile byte brightness = 64; // Dimming level (0-128) 0 = on;128 = off
byte freqStep = 75; // frequency step in uS that defines how often we make interrupt
////
///Communication VARIABLES
const byte dimmerisON = 2;
const byte dimmerisOFF = 3;
const byte rozeteON = 4;
const byte rozeteOFF = 5;
const byte readPower_MSB = 6;
const byte readPower_LSB = 7;
const byte getStates = 8;
////
////misc VARIABLES
uint16_t power = 1234;
uint16_t power2 = 200;
uint16_t power3 = 300;
byte lastCMD = 0;
byte MSB = 0;
byte LSB = 0;
byte stateByte = 0;

void ctrlSockets(byte socketID, byte socketState) { //Assuming sockets are counted from 0; socketID is the id of a specific socket; socketState is on or off or dim ; statesByte is the refere
  //Assuming that socketState is either HIGH or LOW; HIGH means socket is ON; LOW means socket is OFF! //Function must be modified for button inverting states

  switch (socketID) {
    case 0:
      digitalWrite(socketPin_B0, (socketState == ON) ? HIGH : LOW);
      bitWrite(stateByte, socketID, (socketState == ON) ? 1 : 0); // if socketState is ON the n-th bit will be turned to 1 else 0
      break;
    //////////////////
    case 1:
      digitalWrite(socketPin_B1, (socketState == ON) ? HIGH : LOW); // turns
      bitWrite(stateByte, socketID, (socketState == ON) ? 1 : 0);
      break;
    /////////////////
    case 2: //Dimmed socket everything is different!!!
      if (socketState == ON) { // DIMMER.PIN = LOW; socketPin_D0 = HIGH; //Relay as been switched!
        powerDimmer = false; // was an error no false mechanism
        digitalWrite(dimmerPin, LOW);
        digitalWrite(socketPin_D0, HIGH);
        break;
      }//if1
      else if (socketState == DIM) { //Investigation is needed
        //digitalWrite(socketPin_D0,LOW); // I made an erros previuosly because going from on to dim i did not set dimmer pin to low
        //delay(600);
        powerDimmer = true;
        //Value is set by global variable brightness
        break;
      }//else if1

      else {//socketState == OFF
        powerDimmer = false; // is needed so that dimmerPin state is not overridden by dim_check
        digitalWrite(dimmerPin, LOW);
        digitalWrite(socketPin_D0, LOW);
        bitWrite(stateByte, socketID, (socketState == ON) ? 1 : 0);


      }//else1

  } //Switch
}// ctrlSockets
///Interupt based functions
void zero_cross_detect() {
  zero_cross = true;               // set the boolean to true to tell our dimming function that a zero cross has occured
  increment = 0;
  digitalWrite(dimmerPin, LOW);       // turn off TRIAC (and AC)
}

void dim_check() {
  if (powerDimmer == true) { // if dimmer power is requested then open!
    if (zero_cross == true ) { //Cross turns of the the TRIAC if cross is present then dim_check turns
      if (increment >= brightness) {
        digitalWrite(dimmerPin, HIGH); // turn on light
        increment = 0; // reset time step counter
        zero_cross = false; //reset zero cross detection
      }
      else {
        increment++; // we will incremement until we have reached needed number of steps i*75uS/step
      }
    }//if2
  }//if1
}


void setup()
{
  pinMode(dimmerPin, OUTPUT);  // Set the Triac pin as output
  pinMode(socketPin_B0, OUTPUT);
  pinMode(socketPin_B1, OUTPUT);
  pinMode(socketPin_D0, OUTPUT);
  //attachInterrupt(0, zero_cross_detect, RISING);    // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection
  //Timer1.initialize(freqStep);                      // Initialize TimerOne library for the freq we need
  //Timer1.attachInterrupt(dim_check, freqStep);

  Wire.begin(I2C_ID); //               // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
  Serial.begin(9600);           // start serial for output
  delay(100);

  //DEFINE ALL FUCKING PINS AND SET THEM TO LOW!!!!

}

void loop()
{


}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  byte byte1 = Wire.read();
  lastCMD = byte1;
  byte byte2 = Wire.read();

  switch (byte1) {

    case dimmerisON:
      Serial.print("DimmerON! = ");
      Serial.println(byte2);
      Serial.println("");
      break;
    case dimmerisOFF:
      Serial.print("DimmerOFF! = ");
      Serial.println(byte2);
      Serial.println("");
      break;

    case rozeteON:
      ctrlSockets(byte2, ON);
      Serial.print("rozeteON! = ");
      Serial.println(byte2);
      Serial.println("");
      break;

    case rozeteOFF:
      ctrlSockets(byte2, OFF);
      Serial.print("rozeteOFF! = ");
      Serial.println(byte2);
      Serial.println("");
      break;

    case readPower_MSB :
      Serial.print("POWER_MSB = ");
      Serial.println(byte2);
      /// convert to byte array;
      MSB = (byte)(power >> 8);
      Serial.println("");
      break;

    case readPower_LSB:
      Serial.print("POWER_LSB = ");
      Serial.println(byte2);
      LSB = (byte)(power); //Should be handeled by iternal power measuring function
      Serial.println("");
      break;

    case getStates:
      Serial.print("socket States = ");
      Serial.println(stateByte, BIN);
      Serial.println("");
      break;

    default:
      Serial.print("No Such command! = ");
      Serial.println(byte2);
      Serial.println("");

      // if nothing else matches, do the default
      // default is optional
  } //Switch

} //Event



void requestEvent( ) { //After receive event has been triggered MCU stores lastCMD which is used in I2C request so that data could be sent

  switch (lastCMD) {
    case readPower_MSB :

      Serial.print("READPOWER = ");
      Wire.write(MSB);
      Serial.println("");
      break;

    case readPower_LSB:
      Serial.print("READPOWER = ");
      Wire.write(LSB);
      Serial.println("");
      break;

    case getStates:
      Serial.print("statesPushed!");
      Wire.write(stateByte);
      Serial.println("");
      break;


    default :

      Serial.println("NO CASE!");


  }//switch
}//requestEvent

void serialEvent() {
  int socket = 0;
  switch (Serial.read()) {

    case 'b': // Used for setting the brightness
      brightness = Serial.parseInt();
      Serial.print("brightnes = ");
      Serial.println(brightness);
      break;


    case 'd':
      ctrlSockets(2, DIM);
      Serial.println("Dimming!");
      break;

    case 'o':
      ctrlSockets(2, OFF);
      Serial.println("dim OFF!");
      break;


    case 'q': // turns on any socket
      socket = Serial.parseInt();
      //digitalWrite(socketPin_B0,HIGH);
      ctrlSockets(socket, ON);
      Serial.print("socket on = ");
      Serial.println(socket);
      break;

    case 'w': // turns off any socket
      socket = Serial.parseInt();
      //digitalWrite(socketPin_B0,LOW);
      ctrlSockets(socket, OFF);
      Serial.print("socket off = ");
      Serial.println(socket);
      break;



    default :
      Serial.println("NO SUCH COMMAND!");


  }//switch

}
