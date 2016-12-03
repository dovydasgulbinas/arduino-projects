void test_sector( byte clk, byte data, byte lt, byte *tick, long now, long *lastNow, long callPeriod) {
  if (time_to_trigger(now, lastNow, callPeriod)) {
    //Serial.print ("TRIGGERED, UP-TICK: \t");
    //Serial.println(*tick);
    byte sec[5] = {0};
    // insert shit here
    if (*tick < 11) { //will display digits on the sector
      for (int i = 0; i < 5; i++) {
        sec[i] = *tick;
      }

    }
    else if (*tick == 11) { // white leds
      for (int i = 0; i < 5; i++) {
        sec[i] = 16;
      }


    }
    else if (*tick == 12) { //red leds
      for (int i = 0; i < 5; i++) {
        sec[i] = 32;
      }

    }

    
//print_byte_sector("SECTOR BEFORE PUSH: ", 0, sec);

    
    shiftToSec(sec, clk, data, lt);
    ++(*tick); //*to a pointer variable will access the value given in the argument
    
  //  Serial.print ("\t UP-TICK++: \t");
    //Serial.println(*tick);
    
    *tick = *tick % 13;

     //Serial.print ("\t UP-TICK%13: \t");
    //Serial.println(*tick);
  }
}



