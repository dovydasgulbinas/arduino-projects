/************************* MQTT CONTROL *********************************/
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
