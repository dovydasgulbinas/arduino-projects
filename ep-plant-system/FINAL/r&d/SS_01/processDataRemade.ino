void serialEvent() { //1 0 | 1 a 1 | 1 d 100 p 99|
  while (Serial.available()) {
    // Serial.println("AVALABLET!"); //----
    int in = Serial.parseInt();
    //++??++Serial.readBytesUntil(' ', in, 1); // will read the command untill i // MAKE SURE wether terminator values are removed

    //  Serial.print("Igot: ");//----
    // Serial.println(in); //----
    Serial.read(); // remove space
    if (in == 1) { //ParseParameters| Do one of 3 things: enable, disable, edit parameters //Method here is already selected

      haltSerial();

      char enable = Serial.peek(); // will get 1 or 0 or - paramters
      if (enable == '1') {
        streamToEsp.ena = 1; //enable the StreamToESP
        Serial.read(); //remove the command call
        Serial.println("ENABLIOD!");//----
      }
      else if (enable == '0') {
        streamToEsp.ena = 0; // //Disable the StreamToESP
        Serial.read(); //remove the command call
        Serial.println("DISABLIOD!");//----;

      }
      else { // edit  parameters of the method order does not matter can't be called w/ enabling mode
        haltSerial();
        delay(10);
getParam:
        if (Serial.available()) {
          char param = Serial.read(); // gets the parameter id

          switch (param) {
            case 'a': // !!! 1 !!! assuming parse int removes the terminator symbol if I am wrong and Serial.read to remove
              streamToEsp.a = Serial.parseInt();
              Serial.print("a: "); //----
              Serial.println(streamToEsp.a);//----
              Serial.read(); // remove space
              goto getParam;
            case 'p':
              streamToEsp.p = Serial.parseInt();
              Serial.print("p: ");//----
              Serial.println(streamToEsp.p);//----
              Serial.read(); // remove space
              goto getParam;
            case 'n':
              streamToEsp.n = Serial.parseInt();
              Serial.print("n: ");//----
              Serial.println(streamToEsp.n);//----
              Serial.read(); // remove space
              goto getParam;
            case 'v':
              streamToEsp.v = Serial.parseInt();
              Serial.print("v: ");//----
              Serial.println(streamToEsp.v);//----
              Serial.read(); // remove space
              goto getParam;
            case 'r':
              streamToEsp.r = Serial.parseInt();
              Serial.print("r: ");//----
              Serial.println(streamToEsp.r);//----
              Serial.read(); // remove space
              goto getParam;
            default:
              Serial.println("Invalid param"); //---~
              clearSerial();
          }

        }
      }//ELSE cmd 1
    } //parseParams
    else {
      Serial.println("NO COMMAND BLYEAT!"); //----
      clearSerial();
    }//else
  }//while Serial available
}//Serial event
///#################SERIAL EVENT END
