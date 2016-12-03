// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

const byte dimmerisON = 2;
const byte dimmerisOFF = 3;
const byte rozeteON = 4;
const byte rozeteOFF = 5;
const byte readPower = 6;



#include <Wire.h>

void setup()
{
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop()
{
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{

    byte byte1 = Wire.read();
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
      
       case readPower :
    Serial.print("READPOWER = ");
    Serial.println(byte2);
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
