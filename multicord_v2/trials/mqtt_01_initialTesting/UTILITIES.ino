boolean time_to_trigger( long now, long *lastNow, long callPeriod) { // need a pointer variable to get the address of a value
  if ((now - *lastNow) >= callPeriod) { // if scan period is reached
    *lastNow = now;
    return true;
  }
  else {
    return false;
  }
}
