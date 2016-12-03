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
const byte readPowerII =7;
uint16_t power = 1234;
byte lastCMD =0;
byte MSB = 0;
byte LSB =0;


#include <Wire.h>

void setup()
{
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
  Serial.begin(9600);           // start serial for output
   delay(100);
}

void loop()
{
 
 Wire.write(44);
 
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
      
       case readPower :
       //Wire.write(99);
    Serial.print("READPOWER = ");
    Serial.println(byte2);
    /// convert to byte array;
     MSB = (byte)(power >> 8);
     Serial.println("");
     break;
   
   case readPowerII:
   Serial.print("READPOWER-II = ");
   Serial.println(byte2);
    LSB = (byte)(power);
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



void requestEvent( ){
 
  switch (lastCMD) {
       case readPower :
      
    Serial.print("READPOWER = ");
    Wire.write(MSB);
     Serial.println("");
      break;
      
      case readPowerII:
       Serial.print("READPOWER = ");
  Wire.write(LSB);
     Serial.println("");
      break;
      
      default :
      
      Serial.println("NO CASE!");
      
                        
}//switch
}//requestEvent
