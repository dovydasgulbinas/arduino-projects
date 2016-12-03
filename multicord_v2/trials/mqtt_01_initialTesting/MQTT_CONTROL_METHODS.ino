// Ensure the connection to the MQTT server is alive (this will make the first
// connection and automatically reconnect when disconnected).  See the MQTT_connect
// function definition further below.
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }
#if DEBUG
  Serial.print("Connecting to MQTT... ");
#endif
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
#if DEBUG
    Serial.println(mqtt.connectErrorString(ret));//----
    Serial.println("Retrying MQTT connection in 5 seconds...");//----
#endif
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
  }
#if DEBUG
  Serial.println("MQTT Connected!");//----
#endif
}










