#include <CmdMessenger.h>  // CmdMessenger
#include  <TimerOne.h>
//////////////
#include "ds3231.h"
#include <Wire.h>
///////////////

CmdMessenger cmdMessenger = CmdMessenger(Serial);

#define CELL_CLK (13)
#define CELL_DATA (12)
#define CELL_LT (11)
#define DEBUG 1
#define BUZZER 1
#define buzzer_pin 3

//const byte n_displays = 5;


boolean _count_down_enabled = false;
int _countDownTime = 40, _i = 40, _callPeriod = 10, _updatePeriod = 1000; //callPeriod is used for enhaces precision meaning that we will call the function every 1/10 th of a second
byte _printArray[5] = {138, 138, 138, 138, 138}; //it the initial value of the array after first push it will be displayed
int _callCounter = 0;  // will call the timer interrupt method at higher frequency therefore counter is needed
const int _count_until = _updatePeriod / _callPeriod;
//////////
long _last_now = 0;
byte _show_time = 1;
/////////
byte beep_counter = 0;
boolean _alarm_on = false;
long _last_alarm_now = 0;
boolean _buzz_once = false;
long _last_buzz = 0;
uint16_t _run_cnt = 0;
int dot_cnt = 0;
boolean _ena = false, _sena = false;


struct buzzer {
  byte dc;//duty cycle of buzz burtst
  unsigned int dur; //duration of buzzbursts in ms
  byte pin;
} _bb{122, 5000, buzzer_pin};



void count_down() { // this method i called every 1/100th of a sec using timer interrupt(for greater time precision)
  if (_callCounter >= _count_until) {
    if (_count_down_enabled && _i) { //if countdown is enabled and _i > 0
      Serial.println(_i);
      //print_number(_i);
      print_time(_i);
      _i--;
    }
    else if (_i == 0) { // counted down
      stop_countdown(); //this will handle reseting of the variables
    }
    _callCounter = -1;
  }//main if
  _callCounter++;
}

void setup() {
  Wire.begin();
  initSerial();
#if BUZZER
  pinMode(buzzer_pin, OUTPUT);
#endif
  pinMode(CELL_CLK, OUTPUT);
  pinMode(CELL_DATA, OUTPUT);
  pinMode(CELL_LT, OUTPUT);
  Timer1.initialize(long(_callPeriod) * 1000); //Transform miliSeconds to microseconds casting to long() is CRUCIAL! // work with interrupts every milisecond
  Timer1.attachInterrupt(count_down);

}

void loop() {

  buzz_bursts(50, 255, &_buzz_once , _bb.pin);
  buzz_bursts(_bb.dur, _bb.dc, &_ena , _bb.pin);
  buzz_once(_bb.dur,&_sena,_bb.pin);

  if (_show_time && time_to_trigger(millis(), &_last_now, _updatePeriod)) {
    get_time(dot_cnt % 2 == 0);
  }

  if (DS3231_triggered_a1()) { // our alarm has been triggered
    _alarm_on = true;
    DS3231_clear_a1f(); // reset the first alarm flag
  }
  
if(_alarm_on){
  buzz_bursts(15000,200,&_alarm_on, buzzer_pin); 
}
  dot_cnt++;
  cmdMessenger.feedinSerialData();//must be called always
}//looop








