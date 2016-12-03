#include "SPI.h"
const int cs=10;                         // chip select (pin 10)
unsigned int DAC_val = 0;                // DAC value
unsigned int last_val = 0;               // last value (for compare)


unsigned int DAC_read( byte channel ) {
  byte commandbits = B00001101;          //command bits - 0000, start, mode, chn, MSBF
  unsigned int b1 = 0;                   // get the return var's ready
  unsigned int b2 = 0;
  commandbits|=((channel-1)<<1);         // update the command bit to select either ch 1 or 2
  digitalWrite(cs, LOW);                 // select the MCP3202
  SPI.transfer(commandbits);             // send out the command bits
  unsigned int hi = SPI.transfer(b1);       // read back the result high byte
  unsigned int lo = SPI.transfer(b2);       // then the low byte
  digitalWrite(cs, HIGH);                // let the DAC go, we'done
                 
  return (hi<<4)|lo;                      // push hi 4 bits to left in order to free up space for remainig 4 lo bits
                                        
}

void setup() {                           // basic setup stuff here
  pinMode(SS, OUTPUT);                   // make sure slave select is an output
  SPI.setClockDivider( SPI_CLOCK_DIV16 ); // slow the SPI bus down
  SPI.setBitOrder(MSBFIRST);
  SPI.begin();                           // SPI at the ready
  Serial.begin(9600);                   // Serial at the ready
}

void loop() {                            // Loop time
  DAC_val = DAC_read( 1 );               // Read channel 0
 if (millis()%500==0){                                        // look for a rising edge x-ing point
  Serial.println(DAC_val);    // 0 == 512 from the DAC
  Serial.println((float)((DAC_val)*5)/4096);
 }
  
}
