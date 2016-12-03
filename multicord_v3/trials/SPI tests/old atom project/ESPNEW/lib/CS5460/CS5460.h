/*
   CS5460.h - Library for interfacing with CS5460A power measuring IC
   Created by Dovydas Gulbinas, March 16, 2016.
   Released into the public domain.
 */

#ifndef CS5460_h
#define CS5460_h

#include "Arduino.h"
#include "SPI.h"

class CS5460
{
public:
        CS5460(uint32_t data_rate,uint8_t CS_pin, boolean init_after_creation);
        void init_port();

private:
        uint8_t _CS_pin;
        uint32_t _data_rate;


};

#endif
