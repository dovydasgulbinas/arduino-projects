#include "SPI.h"
const int SSP=10;                         // slave select pin
unsigned int DAC_val = 0;                // DAC value
unsigned int last_val = 0;               // last value (for compare)


unsigned int DAC_read( byte channel ) {
  const byte SYNC_BYTE = B00000001; //Pusing data in MSB manner // LSB is the start data trasmission marker
  byte commandByte = B00001101;          //command bits - 0000, start, mode, chn, MSBF
  unsigned int msBits = 0;
  byte lsBits = 0;
  
  commandByte|=((channel-1)<<1);         // update the command bit to select either ch 1 or 2
  digitalWrite(SSP, LOW);                 // select the MCP3202
  SPI.transfer(SYNC_BYTE);             // Sending out the Syncronisation byte w/ leading zeroes
  msBits = SPI.transfer(commandByte);   // Pushes a command and receives the first nibble w/ MSB values of the DAC
  lsBits = SPI.transfer((byte)0);       // Pushes zeroes to get the remaining 8 bits
  digitalWrite(SSP, HIGH);                // let the DAC go, we'done
                 
  return (msBits<<8)+lsBits;              // Left shifts so that it could accomodate lsBits
                                        
}

void setup() {                           // basic setup stuff here
  pinMode(SSP, OUTPUT);                   // make sure slave select is an output
  digitalWrite(SSP,HIGH);// ensures that device connected SSP pin will respond 
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
