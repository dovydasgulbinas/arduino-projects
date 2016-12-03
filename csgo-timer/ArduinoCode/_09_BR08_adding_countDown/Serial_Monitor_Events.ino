


void attachCommandCallbacks()// All callbacks must be added here!
{
  cmdMessenger.attach(0, bomb_planted);
  cmdMessenger.attach(1, start_countdown);
  cmdMessenger.attach(2, stop_countdown);
  cmdMessenger.attach(3, round_kill);
  cmdMessenger.attach(4, show_score);
  cmdMessenger.attach(5, show_time);
  cmdMessenger.attach(6, add_alarm);
  cmdMessenger.attach(7, pwm_buzz);
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

void bomb_planted() {
  _show_time = false;
  _countDownTime = cmdMessenger.readInt16Arg(); //add one extra for if statement
  _sena = true; // will enable the beeping
  suppress_dots(); // must be suppressed only before the first call of countdown
  _i = _countDownTime;
  _count_down_enabled = true;
  _callCounter = _count_until; //enable instant method call will error of 1/10 of a second
}

void start_countdown() // will need changes if other methods will need start countdown
{
  _show_time = false;
  _sena = false; // this addition here fixed the bug seen at the end of the video
#if DEBUG
  cmdMessenger.sendCmd(1, "\t@start_countdown");
#endif
  _countDownTime = cmdMessenger.readInt16Arg(); //add one extra for if statement
  suppress_dots(); // must be suppressed only before the first call of countdown
  _i = _countDownTime;
  _count_down_enabled = true;
  _callCounter = _count_until; //enable instant method call will error of 1/10 of a second
}

void stop_countdown() { // pretty insconsistent with start_countdown because there no display methods are called
  _show_time = false;
  _sena = false;
#if DEBUG
  cmdMessenger.sendCmd(2, "\t@stop_countdown");
#endif
  _count_down_enabled = false;
  _i = _countDownTime; //reset the i counter
  broadcast_value(0);
}

void round_kill() {
  _show_time = false;
#if DEBUG
  cmdMessenger.sendCmd(3, "\t@round_kill");
#endif
  //byte kill_count = cmdMessenger.readInt16Arg();
  byte tmp[5] = {8};
  shiftToSec(tmp, CELL_CLK, CELL_DATA, CELL_LT); // push any value to display without overriding display matrix _printArray
  _buzz_once = true;
}

void show_score() {// this will completely override the values on the display // show delay,
  _show_time = false;
  _sena = false;
#if DEBUG
  cmdMessenger.sendCmd(4, "\t@show_score");
#endif
  byte ct_score = cmdMessenger.readInt16Arg();
  byte t_score = cmdMessenger.readInt16Arg();
  //***********STOP COUNTDOWN***********
  _count_down_enabled = false;
  _i = _countDownTime; //reset the i counter
  //***********COUNTDOWN***********
  p_show_score(ct_score, t_score);
}

void show_time() {
  _show_time = cmdMessenger.readInt16Arg();
#if DEBUG
  if (_show_time) {
    Serial.println("SHOWING CLOCK");
  }
  else {
    Serial.println("CLOCK DISABLED");
  }
#endif
}

void add_alarm() {
  byte hh, mm, ss;
  hh = cmdMessenger.readInt16Arg();
  mm = cmdMessenger.readInt16Arg();
  ss = cmdMessenger.readInt16Arg();
  init_alarm(hh, mm, ss);
#if DEBUG
  char buff[55];
  snprintf(buff, 55, "Alarm set to: %02d:%02d:%02d \n", hh, mm, ss);
  Serial.print(buff);
#endif
}

void pwm_buzz() {
#if DEBUG
  cmdMessenger.sendCmd(7, "\t@pwm_buzz");
#endif
  _bb.dur = cmdMessenger.readInt16Arg();
  _bb.dc = cmdMessenger.readInt16Arg();

  if (_bb.dc == 0) {
    _ena = false;
    _sena = true;
  }
  else {
    _sena = false;
    _ena = true;
  }

}


void OnUnknownCommand() // Called when a received command has no attached function
{
#if DEBUG
  cmdMessenger.sendCmd(0, "ERROR NO CMD");

#endif
  while (Serial.available()) { //clear out the serial buffer
    Serial.read();
  }
}
