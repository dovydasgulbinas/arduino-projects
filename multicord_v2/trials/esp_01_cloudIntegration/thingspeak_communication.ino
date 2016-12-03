
int pushToTSF1(const char* server, const int port, String APIKey, float field1) {

  if (client.connect(server, port)) { //   "184.106.153.149" or api.thingspeak.com
    String postStr = APIKey;
    postStr += "&field1=";
    postStr += String(field1);
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
    return 1;
  }
  else {
   // Serial.println("Failed to connect to TS");
   // client.stop();
    return 0;
  }//else


}

