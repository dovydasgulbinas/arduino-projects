

void buzz_bursts(unsigned int buzz_duration, byte duty_cycle, boolean * buzz, byte buzz_pin ){
  static long last_buzz;
  static unsigned int buzz_cnt; //unsigned will prevent us from negative values in the counter
  if (*buzz) {
    analogWrite(buzzer_pin, duty_cycle);
    if (buzz_cnt == 0) {
      last_buzz = millis(); // this will prevent from stopping buzzer to early
    }
    buzz_cnt++; //order is crucial
    if (time_to_trigger(millis(), &last_buzz, buzz_duration)) {
      digitalWrite(buzzer_pin, LOW);
      *buzz = false;
      buzz_cnt = 0;
    }
  }
}


void buzz_once(int buzz_duration, boolean * buzz, byte buzz_pin) {
  static long last_buzz;
  static unsigned int buzz_cnt; //unsigned will prevent us from negative values in the counter
  if (*buzz) {
    digitalWrite(buzzer_pin, HIGH);
    if (buzz_cnt == 0) {
      last_buzz = millis(); // this will prevent from stopping buzzer to early
    }
    buzz_cnt++; //order is crucial
    if (time_to_trigger(millis(), &last_buzz, buzz_duration)) {
      digitalWrite(buzzer_pin, LOW);
      *buzz = false;
      buzz_cnt = 0;
    }
  }
}



/*
  if (_alarm_on && time_to_trigger(millis(), &_last_alarm_now, 500)) {
    digitalWrite(buzzer_pin, (beep_counter % 2 == 0) ? HIGH : LOW);
    beep_counter++;
    if (beep_counter > 20) { //will beep for ten seconds 1000/500 = 2 * 15sec
      beep_counter = 0;
      _alarm_on = false;
      digitalWrite(buzzer_pin, LOW); //turn the buzzer_off
    }
  }
  */
