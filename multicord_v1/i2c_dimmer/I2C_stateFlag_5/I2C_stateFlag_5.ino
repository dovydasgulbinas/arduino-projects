#include <Wire.h>


#define socketPin_B0 (3) // Socket pin BASIC0
#define socketPin_B1 (4) // Socket pin BASIC1
#define socketPin_D0 (5) //Socket pin w/ DIMMER0
#define dimmerPin (6) //nuspresti veliau
#define interruptPin_1 (3)// hardware interrupt used for dimmer circuit

//NON pin constants
#define OFF (0)
#define ON  (1)
#define DIM (2)
#define I2C_ID (4)

const byte dimmerisON = 2;
const byte dimmerisOFF = 3;
const byte rozeteON = 4;
const byte rozeteOFF = 5;
const byte readPower_MSB = 6;
const byte readPower_LSB =7;
const byte getStates = 8;

uint16_t power = 1234;
byte lastCMD =0;
byte MSB = 0;
byte LSB =0;
byte stateByte = 0;

void ctrlSockets(byte socketID, byte socketState){ //Assuming sockets are counted from 0; socketID is the id of a specific socket; socketState is on or off or dim ; statesByte is the refere
	//Assuming that socketState is either HIGH or LOW; HIGH means socket is ON; LOW means socket is OFF!

	switch (socketID){
	case 0:
		digitalWrite(socketPin_B0,(socketState == ON) ? HIGH : LOW);
		bitWrite(stateByte,socketID,(socketState == ON) ? 1 : 0); // if socketState is ON the n-th bit will be turned to 1 else 0
		break;
		////////////////// 
	case 1:
		digitalWrite(socketPin_B1,(socketState == ON) ? HIGH : LOW); // turns
		bitWrite(stateByte,socketID,(socketState == ON) ? 1 : 0);
		break;
		/////////////////
	case 2: //Dimmed socket everything is different!!!
if(socketState == ON){ // DIMMER.PIN = LOW; socketPin_D0 = HIGH; //Relay as been switched!
digitalWrite(dimmerPin, LOW);
digitalWrite(socketPin_D0,HIGH);
}//if1
else if (socketState == DIM){ //Investigation is needed

}//else if1

else (socketState == OFF){
digitalWrite(dimmerPin, LOW);
digitalWrite(socketPin_D0, LOW);
}//else if21


		digitalWrite(socketPin_D0,socketState);
		bitWrite(stateByte,socketID,(socketState == HIGH) ? 1 : 0);
		break;

	}




}// ctrlSockets

void setup()
{
	Wire.begin(I2C_ID); //               // join i2c bus with address #4
	Wire.onReceive(receiveEvent); // register event
	Wire.onRequest(requestEvent);
	Serial.begin(9600);           // start serial for output
	delay(100);

	//DEFINE ALL FUCKING PINS AND SET THEM TO LOW!!!!
	
	}

void loop()
{

	if(Serial.available()>0){
		Serial.read();
		MSB = (byte)(power >> 8);
		LSB = byte(power);
		
		Serial.print("MSB = ");
		Serial.println(MSB);
		
		Serial.print("LSB = ");
		Serial.println(LSB);
		
	}


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
		Serial.print("rozeteON! = ");
		Serial.println(byte2);
		Serial.println("");
		break;
		
	case rozeteOFF:
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



void requestEvent( ){ //After receive event has been triggered MCU stores lastCMD which is used in I2C request so that data could be sent

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
