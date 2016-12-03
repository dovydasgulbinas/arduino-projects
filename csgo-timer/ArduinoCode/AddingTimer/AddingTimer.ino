#include <CmdMessenger.h>  // CmdMessenger
#include  <TimerOne.h>
CmdMessenger cmdMessenger = CmdMessenger(Serial);

#define CELL_CLK (13)
#define CELL_DATA (12)
#define CELL_LT (11)
#define DEBUG 1

//const byte n_displays = 5;
//const byte n_displays_less = n_displays-1;

boolean _count_down_enabled = false;
long _now = 0, _lastNow = 0;
int _countDownTime = 40, _i = 40, _updatePeriod = 250;
byte _printArray[5] = {138, 138, 138, 138, 138}; //it the initial value of the array after first push it will be displayed


void printNumber(int n, byte reserved_slots = 2, byte shift_to_left = 1, boolean show_leading_zeroes = true);

void attachCommandCallbacks()// All callbacks must be added here!
{
  cmdMessenger.attach(1, start_countdown);
  cmdMessenger.attach(2, stop_countdown);
  //cmdMessenger.attach(3, round_kill);
  cmdMessenger.attach(4, show_score);
  cmdMessenger.attach(OnUnknownCommand);// default command ,ust be placed at the end!
}

void initSerial() {
  Serial.begin(9600);
  cmdMessenger.printLfCr(); // Adds newline to every command
  attachCommandCallbacks();
#if DEBUG
  cmdMessenger.sendCmd(0, "Arduino has started!");  //boot message
#endif
}

void start_countdown() // will need changes if other methods will need start countdown
{
#if DEBUG
  cmdMessenger.sendCmd(1, "\t@start_countdown");
#endif
  _countDownTime = cmdMessenger.readInt16Arg(); //add one extra for if statement
  _i = _countDownTime;
  _count_down_enabled = true;
  suppress_dots(); // must be suppressed only before the first call of countdown
  count_down(); // call this method once to enchance precission on when call starts
}

void stop_countdown() { // pretty insconsistent with start_countdown because there no display methods are called
#if DEBUG
  cmdMessenger.sendCmd(2, "\t@stop_countdown");
#endif
  _count_down_enabled = false;
  _i = _countDownTime; //reset the i counter
  //broadcast_value(8);
}


void show_score() {// this will completely override the values on the display // show delay,
#if DEBUG
  cmdMessenger.sendCmd(4, "\t@show_score");
#endif
  //The score arragement XX_YY, where XX is ct score, _ is blank segment, YY is t score
  byte ct_score = cmdMessenger.readInt16Arg();
  byte t_score = cmdMessenger.readInt16Arg();
  p_show_score(ct_score, t_score);
}



void OnUnknownCommand() // Called when a received command has no attached function
{
#if DEBUG
  cmdMessenger.sendCmd(0, "ERROR NO CMD");

#endif
  while (Serial.available()) {
    Serial.read();
  }
}

void count_down() { // this method i called every second using timer interrupt(for greater time precision)
  if (_count_down_enabled && _i) { //if countdown is enabled and _i > 0
    Serial.println(_i);
    printNumber(_i);
    _i--;
  }
  else if (_i == 0) { // counted down
    stop_countdown(); //this will handle reseting of the variables
  }
}



void setup() {
  initSerial();
  pinMode(CELL_CLK, OUTPUT);
  pinMode(CELL_DATA, OUTPUT);
  pinMode(CELL_LT, OUTPUT);
  Timer1.initialize(long(_updatePeriod) * 1000); //Transform miliSeconds to microseconds casting to long() is CRUCIAL!
  Timer1.attachInterrupt(count_down, long(_updatePeriod) * 1000);
}

void loop() {
  cmdMessenger.feedinSerialData();//must be called always
}//looop








