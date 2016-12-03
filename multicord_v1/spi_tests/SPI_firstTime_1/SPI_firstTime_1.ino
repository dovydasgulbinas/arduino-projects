#include<SPI.h>

int SS = 10;

void setup() {
 

SPI.begin(); //initializes SPI
pinMode(SS,OUTPUT); //Creates a Slave select pin
digitalWrite(SS,HIGH);
}
void pushBytes(int reg, int data){
  digitalWrite(SS,LOW); // SS pin is active low
  SPI.transfer(); //Push the bytes from data sheet?
  SPI.transfer(); // What push?
  // ....
  digitalWrite(SS,HIGH); // Ending Session
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
