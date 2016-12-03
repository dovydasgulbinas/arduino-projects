/*
   CS5460.h - Library for interfacing with CS5460A power measuring IC
   Created by Dovydas Gulbinas, March 16, 2016.
   Released into the public domain.
 */

#include "Arduino.h"
#include "SPI.h"
#include "CS5460.h"


// Commonly used constants
#define CLEAR 0xFF
//Commands
#define START_CONVERSIONS 0xE8
#define SYNC0 0xFE
#define SYNC1 0xFF
#define POWER_UP 0xA0
//Register Read/Write adresses
#define CONFIG_REGISTER B00000
#define CURRENT_DC_OFFS B00001
#define CURRENT_GAIN B00010
//...
#define CYCLE_COUNT B00101
#define INST_CURRENT B00111
#define INST_VOLTAGE B01000
#define INST_POWER B01001
//...
#define STATUS_REGISTER B01111



CS5460::CS5460(uint32_t data_rate,uint8_t CS_pin, boolean init_after_creation)
{
        pinMode(CS_pin, OUTPUT);
        _CS_pin = CS_pin;
        SPI.begin();
        if(init_after_creation){
          init_port();
        }

}


void CS5460::init_port()
{
        SPI.beginTransaction(SPISettings(_data_rate, MSBFIRST, SPI_MODE0));
        digitalWrite(_CS_pin, LOW);
        for (uint8_t i = 0; i < 3; i++) { //we will send 4 SYNC1 to initialize connection with cs5460
                SPI.transfer(SYNC1);
        }
        SPI.transfer(SYNC0); // sequence of SYNC1 must be followed by one SYNC0
        digitalWrite(_CS_pin, HIGH);
        SPI.endTransaction();

}
