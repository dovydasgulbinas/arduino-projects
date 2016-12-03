
// Commonly used constants
#define CLEAR 0xFF
#define SYNCER  0x00FEFEFE // this is a constant used for initializing structures when sending via serial port
//Commands
#define START_CONVERSIONS 0xE8
#define SYNC0 0xFE
#define SYNC1 0xFF
#define POWER_UP 0xA0
//Register Read/Write adresses
#define CONFIG_REGISTER B00000
#define CURRENT_DC_OFFS B00001
#define CURRENT_GAIN B00010
//...
#define CYCLE_COUNT B00101
#define INST_CURRENT B00111
#define INST_VOLTAGE B01000
#define INST_POWER B01001
//...
#define STATUS_REGISTER B01111

//########################## U T I L S #####################################

void serial_printer(char * label, uint32_t value){
        Serial.print(label);
        Serial.print("\t");
        Serial.println(value, HEX);
}

void print_byte_arr(char * label, uint8_t * arr, uint8_t length){ // * in front of address will return a value
        Serial.print(label);
        for(uint8_t i =0; i<length; i++) {
                uint8_t value = *(arr+i);
                if(value != 0) {
                        Serial.print(value, HEX);
                        Serial.print(" ");
                }
                else{// value is == 0
                        Serial.print("00");
                        Serial.print(" ");
                }
        }
        Serial.println("");
}

void halt_until_any_key(char * label) {
        while(Serial.available()) { //read everything from serial before we do anytnig
                Serial.read();
        }
        Serial.print("\t");
        Serial.println(label);
        Serial.println("\t Press any key to continue...");

        while(!Serial.available()) {
                yield();
        }
        Serial.println("Key press!");

        while(Serial.available()) { //clear key press after the buffer
                Serial.read();
        }

        //return;
}

void halt_until_any_key() {
        while(Serial.available()) { //read everything from serial before we do anytnig
                Serial.read();
        }

        Serial.println("\t Press any key to continue...");

        while(!Serial.available()) {
                yield();
        }
        Serial.println("Key press!");

        while(Serial.available()) { //clear key press after the buffer
                Serial.read();
        }

        //return; may cause issues?

}



//###########################################

uint8_t cmd_to_byte(uint8_t cmd, boolean wr){ // if wr == true then we will write to register else read
        uint8_t result = 0;
        result = cmd<<1;
        if(wr) { // we are writing to the register
                result |=B01000000; //this time we write to the register therefore we must set MSB-1 bit to 1
                result &= B01111110;
        }
        else{ // we are reding from the register
                result &=B00111110;
        }
        return result;
}
//######################################## OBSOLETE DOWN
void write_array_register(uint8_t cmd, uint8_t reg_data[],uint8_t CS){  //msb bit is stored in the [1]-st element of an array
        uint8_t tx_arr[4] = {0};
        uint8_t tmp_arr[4] = {0};

        tx_arr[0] = cmd<<1;
        tx_arr[0] |=B01000000; //this time we write to the register therefore we must set MSB-1 bit to 1
        tx_arr[0] &= B01111110;

        for (int i = 1; i < 4; i++) { // prepare byte array for transfer
                tx_arr[i] = reg_data[i-1];
        }

        print_byte_arr("WR DATA: ",tx_arr,4);//--

        // SPI.beginTransaction(SPIset);
        // digitalWrite(CS, LOW);
        // for (uint8_t i = 0; i < 4; i++) { //we will send 4 SYNC1 to initialize connection with cs5460
        //         SPI.transfer(tx_arr[i]);
        // }
        // digitalWrite(CS, HIGH);
        // SPI.endTransaction();

}


uint8_t * read_register(uint8_t cmd, uint8_t CS){ //has a global object SPIset // this method uses static variable which could fuck up a thing or two given the static nature of method calls
        uint8_t tx_arr[4] = {SYNC0}; //SYNC0 must be strobed when getting data from the register
        tx_arr[0] = cmd<<1;
        tx_arr[0] &= B00111110; //masking the first two bits and the last bit. MSB bit must always be 0, MSB-1 bit must be 0 when reading, LSB bit must always be 0
        static uint8_t rx_arr[4] = {0}; // only static arrays can be returned as a pointer

        SPI.beginTransaction(SPIset); //parse SPI settings
        digitalWrite(CS, LOW);

        for(uint8_t i =0; i<4; i++) {
                rx_arr[i] = SPI.transfer(tx_arr[i]); // push and get the bytes in MSB oder
        }
        digitalWrite(CS, HIGH);
        SPI.endTransaction();

        return rx_arr;
}

uint8_t * read_register(uint8_t cmd){ //has a global object SPIset // this method uses static variable which could fuck up a thing or two given the static nature of method calls
        uint8_t tx_arr[4] = {SYNC0}; //SYNC0 must be strobed when getting data from the register
        tx_arr[0] = cmd<<1;
        tx_arr[0] &= B00111110; //masking the first two bits and the last bit. MSB bit must always be 0, MSB-1 bit must be 0 when reading, LSB bit must always be 0
        static uint8_t rx_arr[4] = {0}; // only static arrays can be returned as a pointer

        SPI.beginTransaction(SPIset); //parse SPI settings

        for(uint8_t i =0; i<4; i++) {
                rx_arr[i] = SPI.transfer(tx_arr[i]); // push and get the bytes in MSB oder
        }
        SPI.endTransaction();

        return rx_arr;
}

//######################################## OBSOLETE UP

uint32_t read_register_(uint8_t cmd){//~~~
        union {
                uint32_t val;
                struct {
                        uint8_t lsb;
                        uint8_t mbb;
                        uint8_t mb;
                        uint8_t msb;
                };
        } s_out, s_in; //slave_out, slave_in

        s_in.val = SYNCER;
        s_in.msb = cmd_to_byte(cmd, false);

        SPI.beginTransaction(SPIset); //parse SPI settings
        s_out.msb = SPI.transfer(s_in.msb);
        s_out.mb = SPI.transfer(s_in.mb);
        s_out.mbb = SPI.transfer(s_in.mbb);
        s_out.lsb = SPI.transfer(s_in.lsb);
        SPI.endTransaction();

        return s_out.val;
}

void spi_transfer_uint32(uint32_t data, boolean HW_CS_ENABLED, uint8_t CS ){  // if HW_CS_ENABLED true then CS can be any value
        SPI.beginTransaction(SPIset);

        if(HW_CS_ENABLED) {
                SPI.write32(data);
        }
        else{
                digitalWrite(CS, LOW);
                SPI.write32(data);
                digitalWrite(CS, HIGH);
        }
        SPI.endTransaction();
}

void spi_transfer_uint32(uint32_t data){ // assuming HW CS is enabled therefore the is no need of sen
        SPI.beginTransaction(SPIset);
        SPI.write32(data);
        SPI.endTransaction();
}

void write_register_(uint8_t cmd, uint32_t reg_value){ //this method only does writing and does not receive any data sent from duplex communication
        union {
                uint32_t val;
                struct {
                        uint8_t lsb;
                        uint8_t mbb;
                        uint8_t mb;
                        uint8_t msb;
                };
        } s_in; //slave_out, slave_in
        s_in.val = reg_value;
        s_in.msb = cmd_to_byte(cmd,true);

        serial_printer("@write_register_", s_in.val);//--
        spi_transfer_uint32(s_in.val);
}



// Derrivative methods

void init_port(uint8_t CS) {
        SPI.beginTransaction(SPIset);
        digitalWrite(CS, LOW);
        for (uint8_t i = 0; i < 3; i++) { //we will send 4 SYNC1 to initialize connection with cs5460
                SPI.transfer(SYNC1);
        }
        SPI.transfer(SYNC0); // sequence of SYNC1 must be followed by one SYNC0
        digitalWrite(CS, HIGH);
        SPI.endTransaction();
}

void push_w_command(uint8_t cmd, uint8_t CS){
        SPI.beginTransaction(SPIset);
        digitalWrite(CS, LOW);
        SPI.transfer(cmd);
        digitalWrite(CS, HIGH);
        SPI.endTransaction();
}

void start_conversions(uint8_t CS) {
        push_w_command(START_CONVERSIONS, CS);
        return;
}


void set_mask_register(){
        return;
}

void clear_status_register(){
        uint8_t reg_data[3] = {CLEAR,CLEAR,CLEAR};
        write_array_register(STATUS_REGISTER,reg_data,CS_pin);
        return;
}

void enable_interrupts(){
        return;
}

void get_status_register(){
        return;
}

void set_conf_register(){
        return;
}


void set_calibration_registers(){
        return;
}


void init_chip(boolean use_hw_cs){

        if(use_hw_cs) { // we will use dedicated cs_pin
                SPI.setHwCs(use_hw_cs); // double check what state CS pin is at by default
        }
        else{
                pinMode(CS_pin, OUTPUT);
                digitalWrite(CS_pin, HIGH);
        }

        Serial.println("Initializing via SPI..."); //--
        SPI.begin();
        init_port();
//++clear_status_register();
//++set_mask_register();
//++enable_interrupts();
        start_conversions();
}
