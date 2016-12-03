int pushToTS(const char* server, const int port, String APIKey, float field1, float field2, float field3, float field4) {

  if (client.connect(server, port)) { //   "184.106.153.149" or api.thingspeak.com
    String postStr = APIKey;
    postStr += "&field1=";
    postStr += String(field1);
    postStr += "&field2=";
    postStr += String(field2);
    postStr += "&field3=";
    postStr += String(field3);
    postStr += "&field4=";
    postStr += String(field4);
    postStr += "\r\n\r\n";
    
  
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + APIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    client.stop();

    Serial.print("field1:\t");
    Serial.println(field1);
    Serial.print("field2:\t");
    Serial.println(field2);
    Serial.print("field3:\t");
    Serial.println(field3);
    Serial.print("field4:\t");
    Serial.println(field4);


    return 1;
  }
  else {
   // Serial.println("Failed to connect to TS");
   // client.stop();
    return 0;
  }//else


}
