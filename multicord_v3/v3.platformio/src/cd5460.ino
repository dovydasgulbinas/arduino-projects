
// Commonly used constants
#define CLEAR 0xFFFFFFFF
#define SYNCER  0x00FEFEFE // this is a constant used for initializing structures when sending via serial port
#define INIT_PORT 0xFFFFFFFE // this is a constant used for initilaling serial port the following is sync1 x3, sync0
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
#define MASK_REGISTER B11010


union SMR_struct { // give a name to union// if you dont give union a name you can only use this particular instance by defining stuff after end of the union
        uint32_t sm;
        struct {
                unsigned IC_ : 1;
                unsigned : 1;
                unsigned LSD : 1;
                unsigned IOD : 1;
                unsigned VOD : 1;
                unsigned WDT : 1;
                unsigned ID : 4;
                unsigned : 1;
                unsigned EOOR : 1;
                unsigned EOR : 1;
                unsigned VROR : 1;
                unsigned IROR : 1;
                unsigned PWROR : 1;
                unsigned VOR : 1;
                unsigned IOR : 1;
                unsigned : 1;
                unsigned MATH : 1;
                unsigned CRDY : 1;
                unsigned EDIR : 1;
                unsigned EOUT : 1;
                unsigned DRDY : 1;
        };
};//
typedef union SMR_struct smr; //
smr g_reg_state, g_reg_mask; // global variables used for other methods

union config_register { // give a name to union// if you dont give union a name you can only use this particular instance by defining stuff after end of the union
        uint32_t cr;
        struct {
                unsigned K : 4;
                unsigned iCPU : 1;
                unsigned IHPF : 1;
                unsigned VHPF : 1;
                unsigned RS : 1;
                ///
                unsigned DL : 2;
                unsigned EOD : 1;
                unsigned SI : 2;
                unsigned : 2;
                unsigned EWA : 1;
                ///
                unsigned Gi : 1;
                unsigned PC : 7;
        };
};//
typedef union config_register creg; // you cant pass union as function argument as typedef name // but you can use this name as a short hand notation when making new objects in functions
creg g_conf_reg; // GLOBAL AND IMPORTANT global instance

union four_bytes_in_uint32 {
        uint32_t val;
        struct {
                uint8_t lsb; //LSB
                uint8_t mbb; // MSB-2
                uint8_t mb; // MSB-1
                uint8_t msb; //most significant bit MSB
        };
};
typedef union four_bytes_in_uint32 fbu; // Giving a name to a union or strucute will make it reusable. this means that we can access it everywhere.

//########################## U T I L S #####################################

void serial_printer(char * label, uint32_t value){
        Serial.print(label);
        Serial.print("\t");
        Serial.println(uint32_t(value), HEX);
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

void spi_transfer_uint32(uint32_t data){ // assuming HW CS is enabled therefore the is no need of sen
        SPI.beginTransaction(SPIset);
        SPI.write32(data);
        SPI.endTransaction();
}

void spi_transfer_uint8(uint8_t data){
        SPI.beginTransaction(SPIset);
        SPI.transfer(data);
        SPI.endTransaction();
}

uint32_t read_register(uint8_t cmd){//~~~
        fbu s_out, s_in; //slave_out, slave_in
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

void write_register(uint8_t cmd, uint32_t reg_value){ //this method only does writing and does not receive any data sent from duplex communication
        fbu s_in; //slave_out, slave_in
        s_in.val = reg_value;
        s_in.msb = cmd_to_byte(cmd,true);
        spi_transfer_uint32(s_in.val);
}
// DERRIVATIVE METHODS
boolean data_is_integral(){
 //This methods reads data from a register and compares it with data provided at the argument. This should be used when checking wether all values were sent accordingly
  
}
// COMMAND BASED METHODS

void init_port() {
        spi_transfer_uint32(INIT_PORT);
}

void start_conversions() {
        spi_transfer_uint8(START_CONVERSIONS);
}

void clear_status_register(){
        spi_transfer_uint32(CLEAR);
}

// REGISTER MODIFICATION BASED METHODS

void enable_interrupts(uint8_t SI_val, union config_register *pCreg, boolean send){ //if true it will send bytes via SPI else it will only modify the global structure this is usefull when you want to bundle multiple settings and send them on one go
        pCreg->SI = SI_val;
        if (send) {
          write_register(CONFIG_REGISTER, pCreg->cr);
        }
}

void reset_chip(union config_register *pCreg, boolean send){
  pCreg->RS = 1; // after reset this bit will reset to  0 automatically
  if(send){
    write_register(CONFIG_REGISTER, pCreg->cr);
  }

}

void init_chip(){
        //reg_config.cr = 0;
        SPI.setHwCs(true);         // double check what state CS pin is at by default
        SPI.begin();
        init_port();
//++clear_status_register();
//++set_mask_register();
//++enable_interrupts();
        start_conversions();
}
