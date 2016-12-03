void init_relay(uint8_t relay_pin){
        pinMode(relay_pin, OUTPUT);
        digitalWrite(relay_pin,LOW);
        //return;
}

void turn_device_on(uint8_t rP){
        digitalWrite(rP, LOW);
        //return;
}

void turn_device_off(uint8_t rP){
        digitalWrite(rP, HIGH);
        //return;
}
