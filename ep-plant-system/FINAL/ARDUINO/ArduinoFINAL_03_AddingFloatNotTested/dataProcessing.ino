void processData() {
  if (streamToEsp.ena == 1) {
    if (((millis() - streamToEsp.lc) >= streamToEsp.p) || (streamToEsp.ln == 0)) { // this means that p ms has passed since the last measurment and not enought messuremnt were taken
      if ((streamToEsp.ln < streamToEsp.n)) { // grouping parameters by quickest receive time fastest at the top

        streamToEsp.avgMoi += analogRead(SH);
        streamToEsp.avgLum += outputLux(BH1750_address);
        streamToEsp.avgHum += dht.readHumidity();
        streamToEsp.avgTemp += dht.readTemperature();
        //...
        streamToEsp.ln ++;
        analogWrite(LED, map(streamToEsp.ln, 0, streamToEsp.n, 0, 255));
        streamToEsp.lc = millis();

      } // if ln<n
      else { //.ln == 8 pushData, reset ln =0,cast to int,parseString, reset all values
        //GET MEAN
        streamToEsp.avgMoi /= streamToEsp.n;
        streamToEsp.avgLum /= streamToEsp.n;
        streamToEsp.avgHum /= streamToEsp.n;
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

        streamToEsp.avgMoi = 0.0;
        streamToEsp.avgLum = 0.0;
        streamToEsp.avgHum = 0.0;
        streamToEsp.avgTemp = 0.0;
        streamToEsp.ln = 0;
      } // else ln<n
    } // if millis ....
  }// if ena
  else {//function is disabled //not necessary if you dont want to enable the function from com port!
    streamToEsp.avgMoi = 0.0;
    streamToEsp.avgLum = 0.0;
    streamToEsp.avgHum = 0.0;
    streamToEsp.avgTemp = 0.0;
    streamToEsp.ln = 0;
    streamToEsp.cData = "";
  }
} //processData
