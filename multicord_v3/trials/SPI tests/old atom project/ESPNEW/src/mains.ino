//taken from https://github.com/esp8266/Arduino/issues/1429


#include <ESP8266WiFi.h>
#include <SPI.h>             //SPI
#define INST_VOLTAGE B01000
#define DEBUG 0

SPISettings SPIset(200000, MSBFIRST, SPI_MODE0);


const byte CS_pin = 4;
const byte _relay_pin = 5;

byte _voltage_bytes[4] = {0};
uint8_t test_arr[3] = {0x33,0x22,0x11}; //--

void init_port(byte CS = CS_pin);
void start_conversions(byte CS = CS_pin);
unsigned long get_voltage(byte CS = CS_pin);

uint8_t * read_register(uint8_t cmd, uint8_t CS);
uint8_t * read_register(uint8_t cmd);

void write_array_register(uint8_t cmd, uint8_t reg_data[], uint8_t CS = CS_pin);//---
void write_register_(uint8_t cmd, uint32_t reg_value);
void print_byte_arr(byte * arr, byte length);

void init_relay(uint8_t relay_pin = _relay_pin);
void init_chip();

void turn_device_on(uint8_t rP = _relay_pin);
void turn_device_off(uint8_t rP = _relay_pin);

void halt_until_any_key(char * label);
void halt_until_any_key();


void setup() {
        Serial.begin(115200);
        init_relay();
        delay(150); //it may fix auto press issue
        halt_until_any_key("device_off");
        turn_device_off();
        halt_until_any_key("device_on");
        turn_device_on();
        delay(200);
        halt_until_any_key("init_chip");
        init_chip(true);
}

void loop() {

        //halt_unB00110t_byte_arr("INST VOLTAGE: ",read_register(INST_VOLTAGE),4);
        halt_until_any_key("CLICK TO WRITE: 0x00FFEEDD");
        write_register_(B00110,0x00FFEEDD);
        halt_until_any_key("CLICK TO READ: 0xXXXXXXX");
        serial_printer("RED VALUE", read_register_(B00110));

        //serial_printer("new INST VOL: ", read_register_(INST_VOLTAGE));

//write_array_register(0x44, test_arr);//---
//write_register_(0x44, 0x99332211);

}
