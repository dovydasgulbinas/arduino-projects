void processData() {
  float moi, lum;
     // Serial.println("HO");
  if (streamToEsp.ena == 1) {
       // Serial.println("in first if");
    if (((millis() - streamToEsp.lc) >= streamToEsp.p) || streamToEsp.ln == 0) { // this means that p ms has passed since the last measurment and not enought messuremnt were taken
         // Serial.println("in second if");
      if ((streamToEsp.ln < streamToEsp.n)) { // grouping parameters by quickest receive time fastest at the top
           // Serial.println("in third if");
        moi = analogRead(SH);
            //Serial.println(moi);
        //lum = outputLux(BH1750_address);
        ////INSERT DELTA METHODS HERE! ALSO USE PROCESS DATA!!!! from this file

        ////
        streamToEsp.avgMoi += moi;//==== / streamToEsp.n;
        //streamToEsp.avgLum += lum;//==== / streamToEsp.n; //get luminosity //++++
        streamToEsp.avgHum += dht.readHumidity();//==== / streamToEsp.n;//++++
        streamToEsp.avgTemp += dht.readTemperature();//====dht.readTemperature()// / streamToEsp.n;//++++
        //...
        streamToEsp.ln ++;
        analogWrite(LED, map(streamToEsp.ln, 0, streamToEsp.n, 0, 255));
        streamToEsp.lc = millis();

      } // if ln<n
      else { //.ln == 8 pushData, reset ln =0,cast to int,parseString, reset all values
        //GET MEAN
        streamToEsp.avgMoi /= streamToEsp.n;
        streamToEsp.avgLum /= streamToEsp.n;
        //streamToEsp.avgHum /= streamToEsp.n;
        streamToEsp.avgTemp /= streamToEsp.n;

        streamToEsp.cData = "1:";
        streamToEsp.cData += String(streamToEsp.avgTemp);
        streamToEsp.cData += "|";
        streamToEsp.cData += String(streamToEsp.avgHum);
        streamToEsp.cData += "|";
        streamToEsp.cData += String(streamToEsp.avgLum);
        streamToEsp.cData += "|";
        streamToEsp.cData += String(streamToEsp.avgMoi);
        streamToEsp.cData += "|";
        Serial.println(streamToEsp.cData);
        //Reasign Variables

        streamToEsp.avgMoi = streamToEsp.avgMoi; //reasign old mean values
       // streamToEsp.avgLum = streamToEsp.avgLum;

        streamToEsp.avgMoi = 0.0;
        streamToEsp.avgLum = 0.0;
        streamToEsp.avgHum = 0.0;
        streamToEsp.avgTemp = 0.0;
        streamToEsp.ln = 0;
      } // else ln<n
    } // if millis ....
  }// if ena
  else {
  }
} //processData
