#include <CmdMessenger.h>  // CmdMessenger
CmdMessenger cmdMessenger = CmdMessenger(Serial);

#define CELL_CLK (13)
#define CELL_DATA (12)
#define CELL_LT (11)

boolean _count_down_enabled = false;
long _now = 0, _lastNow = 0;
int _countDownTime = 40, _i = 40, _updatePeriod = 1000;


void printNumber(int n, boolean suppress_leading_zeroes = true);

void attachCommandCallbacks()// All callbacks must be added here!
{
  cmdMessenger.attach(1, start_countdown);
  cmdMessenger.attach(2, stop_countdown);
  cmdMessenger.attach(3, set_update_period);
  cmdMessenger.attach(OnUnknownCommand);// default command ,ust be placed at the end!
}

void initSerial() {
  Serial.begin(9600);
  cmdMessenger.printLfCr(); // Adds newline to every command
  attachCommandCallbacks();
  cmdMessenger.sendCmd(0, "Arduino has started!");  //boot message
}


void start_countdown()
{
  cmdMessenger.sendCmd(10, "\t@start_countdown");
  _count_down_enabled = true;
}

void stop_countdown() { // pretty insconsistent with start_countdown because there no display methods are called
  cmdMessenger.sendCmd(11, "\t@stop_countdown");
  _count_down_enabled = false;
  _i = _countDownTime; //reset the i counter
  printNumber(0, false);
}

void set_update_period () {
  cmdMessenger.sendCmd(11, "\t@set_update_period");
  _updatePeriod = cmdMessenger.readInt16Arg(); //get update period from serial monitor in seconds
  _countDownTime = cmdMessenger.readInt16Arg(); //add one extra for if statement
  _i = _countDownTime;
}

void OnUnknownCommand() // Called when a received command has no attached function
{
  cmdMessenger.sendCmd(0, "ERROR NO CMD");
}

void setup() {
  initSerial();
  pinMode(CELL_CLK, OUTPUT);
  pinMode(CELL_DATA, OUTPUT);
  pinMode(CELL_LT, OUTPUT);
}

void loop() {
  delay(1);
  _now = millis(); // get the current ms
  
  if (_count_down_enabled) { // begin countdown
    if (timed_trigger(_now, &_lastNow, _updatePeriod)) {
      printNumber(_i);
      if (!_i) { //countdown time == 0
        stop_countdown();
      }
      _i--;
    }
  }

  cmdMessenger.feedinSerialData();//must be called always
}//looop








