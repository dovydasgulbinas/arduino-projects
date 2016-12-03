boolean time_to_trigger( long now, long *lastNow, long callPeriod) { // need a pointer variable to get the address of a value
  if ((now - *lastNow) >= callPeriod) { // if scan period is reached
    *lastNow = now;
    return true;
  }
  else {
    return false;
  }
}

void init_alarm( byte hh, byte mm, byte ss) {
  DS3231_init(DS3231_INTCN);
  DS3231_clear_a1f();
  set_alarm(ss, mm, hh);
}

void set_alarm(byte wake_SECOND, byte wake_MINUTE, byte wake_HOUR )
{
  // flags define what calendar component to be checked against the current time in order
  // to trigger the alarm - see datasheet
  // A1M1 (seconds) (0 to enable, 1 to disable)
  // A1M2 (minutes) (0 to enable, 1 to disable)
  // A1M3 (hour)    (0 to enable, 1 to disable)
  // A1M4 (day)     (0 to enable, 1 to disable)
  // DY/DT          (dayofweek == 1/dayofmonth == 0)
  uint8_t flags[5] = { 0, 0, 0, 1, 1 };

  // set Alarm1
  DS3231_set_a1(wake_SECOND, wake_MINUTE, wake_HOUR, 0, flags);

  // activate Alarm1
  DS3231_set_creg(DS3231_INTCN | DS3231_A1IE);
}

void get_time(boolean show_the_dot) {
  struct ts t;
  DS3231_get(&t);
  print_real_time(t.hour, t.min, show_the_dot);
  /* User for printing to serial monitor
     const byte BUFF_MAX = 255;
    char buff[BUFF_MAX];
    snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d",
           t.year, t.mon, t.mday, t.hour, t.min, t.sec);

    Serial.println(buff);
  */
}

