//TO DO:
//*Change all of the methods that reference the old _printArray[] and redisign them to take _segment[]
//
//


#include <CmdMessenger.h>  // CmdMessenger
CmdMessenger cmdMessenger = CmdMessenger(Serial);

#define CLK_A (13)
#define DATA_A (12)
#define LT_A (11)

#define CLK_B (10)
#define DATA_B (9)
#define LT_B (8)

#define BUTTON_LINE (2)
#define DEBUG 1
#define A 0
#define B 1
#define C 2
#define D 3
#define E 4

long _now = 0, _btn_now = 0, _btn_scan_period = 250;
//TEST VARS
long _sN = 0, _sC = 250;
byte _tcnt = 0; // tick counter tracks what to display on the sector
byte _c = 0;

boolean _disp_test  = true;
//
byte _printArray[5] = {138, 138, 138, 138, 138}; //it the initial value of the array after first push it will be displayed
boolean time_to_trigger( long now, long *lastNow, long callPeriod);

void test_sector( byte clk, byte data, byte lt, byte *tick, long now, long *lastNow, long callPeriod);

/// new build
boolean line_is_hot(uint8_t line = BUTTON_LINE);
void arm_sector(byte secId, byte segment[] , byte clk, byte data, byte lt);


///////////////////not yet used
byte _segment[25] = {
  0, 1, 2, 3, 4,
  2, 3, 4, 5, 6,
  9, 8, 7, 9, 7,
  6, 7, 0, 0, 0,
  8, 9, 8, 9, 1,
};
byte _prev_buttons[25] = {//Stores the last state of the button // JUST TO BE EXPLICIT WHAT STATE THEY ARE NOW
  LOW, LOW, LOW, LOW, LOW,
  LOW, LOW, LOW, LOW, LOW,
  LOW, LOW, LOW, LOW, LOW,
  LOW, LOW, LOW, LOW, LOW,
  LOW, LOW, LOW, LOW, LOW,
};


uint8_t button_click (uint8_t line = BUTTON_LINE, byte segment[] = _segment , byte clk = CLK_A, byte data = DATA_A , byte lt = LT_A );
void arm_sector(byte secId = 1 , byte segment[] = _segment , byte clk = CLK_A, byte data = DATA_A , byte lt = LT_A);
void disarm_sector(byte secId = 1 , byte segment[] = _segment , byte clk = CLK_A, byte data = DATA_A , byte lt = LT_A);
void arm_cell(byte secId , byte cellId, byte segment[] = _segment , byte clk = CLK_A, byte data = DATA_A , byte lt = LT_A);
/////////////////////


void setup() {
  initSerial();
  pinMode(CLK_A, OUTPUT);
  pinMode(DATA_A, OUTPUT);
  pinMode(LT_A, OUTPUT);
  pinMode(BUTTON_LINE, INPUT);
  print_byte_sector("before: ", 1, _segment);
  arm_sector(); // must be called for the first time if not no buton presses will be registere
 print_byte_sector("after-disarm: ", 1, _segment);

}

void loop() {
  _now = millis();


  if (_disp_test) {
    test_sector( CLK_A, DATA_A, LT_A, &_tcnt, _now, &_sN, _sC);
    //for release methods refet to 05_sketch
  }

  else {
    if (line_is_hot()) {
      _c++;
      if (_c == 1) {
        delay(100); // will wait for 100 ms after receiving a button press this works a debounce time
        Serial.println("HOT 1");
      }
    }
    else {
      _c = 0;
    }

    if (_c > 1) { // button debounding has stopped and we can check which button has been pressed
      Serial.println("HOT 2");
      Serial.print("BUTTON CODE:\t");
      byte code = button_click();
      arm_sector();
      Serial.println(code, HEX);
    }
  } // else _disptest

  cmdMessenger.feedinSerialData();//must be called always
}//looop


