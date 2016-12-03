void processData() {
  if (streamToEsp.ena == 1) {
    if (((millis() - streamToEsp.lc) >= streamToEsp.p)) { // this means that p ms has passed since the last measurment and not enought messuremnt were taken
      if ((streamToEsp.ln < streamToEsp.n)) { // grouping parameters by quickest receive time fastest at the top
        streamToEsp.avgMoi += analogRead(SH) / streamToEsp.n;
        streamToEsp.avgLum += outputLux(BH1750_address) / streamToEsp.n; //get luminosity //++++
        streamToEsp.avgHum += dht.readHumidity() / streamToEsp.n;//++++
        streamToEsp.avgTemp += dht.readTemperature() / streamToEsp.n;//++++
        //...
        streamToEsp.ln ++;
        analogWrite(LED, map(streamToEsp.ln, 0, streamToEsp.n, 0, 255));
        streamToEsp.lc = millis();

      } // if ln<n
      else { //.ln == 8 pushData, reset ln =0,cast to int,parseString, reset all values
        streamToEsp.pData = "1:";
        streamToEsp.pData += String((int)streamToEsp.avgTemp);
        streamToEsp.pData += "|";
        streamToEsp.pData += String((int)streamToEsp.avgHum);
        streamToEsp.pData += "|";
        streamToEsp.pData += String((int)streamToEsp.avgLum);
        streamToEsp.pData += "|";
        streamToEsp.pData += String((int)streamToEsp.avgMoi);
        streamToEsp.pData += "|";
        Serial.println(streamToEsp.pData);
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
