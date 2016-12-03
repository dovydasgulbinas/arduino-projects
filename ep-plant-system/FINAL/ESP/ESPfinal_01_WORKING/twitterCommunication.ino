int tweet(const char* server, int port, const char* tsTwitterKey, String message) {
  if (client.connect(server, port)) { // Opens a TCP socket and checks

    String postStr;
    postStr += "api_key=";
    postStr += tsTwitterKey; //Prepate the postString
    postStr += "&status=";
    postStr += message;

    client.print("POST /apps/thingtweet/1/statuses/update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr + "\r\n\r\n");
    client.stop();
    Serial.println(message);
    return 1;
  }

  else {
    client.stop();
    Serial.print("Failed to relay to Twitter");
    return -1;
  }
}
