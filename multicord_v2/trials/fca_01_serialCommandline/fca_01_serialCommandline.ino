#define DEBUG 1
#include <CmdMessenger.h>  // CmdMessenger
CmdMessenger cmdMessenger = CmdMessenger(Serial);
#include  <TimerOne.h>
const uint8_t _nSamples = 40;
uint8_t _cc = 0;
int _measurePeriod = 1000000 / _nSamples;
bool _socketEnabled = true;
const byte piSocket = 10;
String ec = "\t>>>>\r\n";

struct powerStruct {
  float RMSCurrent;
  float currentSample[_nSamples];
  uint8_t piPower;
  float power;
  float currentVoltConst;
  int noise;
  float voltage;
  int thPower; // threshold power power below the value will be ignored
} p = {0.0, {0.0}, A0, 0.0, 0.066, 0, 220.1, 17};


// All callbacks must be added here!
void attachCommandCallbacks()
{
  cmdMessenger.attach(0, send_status); //
  cmdMessenger.attach(1, send_power);
  cmdMessenger.attach(2, disable_socket);
  cmdMessenger.attach(3, set_voltage); //
  cmdMessenger.attach(4, set_thPower); //

  cmdMessenger.attach(OnUnknownCommand);// default command ,ust be placed at the end!
}

// Called when a received command has no attached function
void OnUnknownCommand()
{
  //cmdMessenger.sendCmd(0, "ERROR NO CMD");
}

void send_status() {
#if DEFINE
  Serial.print(ec);
  Serial.print("RMSCurrent:\t");
  Serial.println(p.RMSCurrent);
  Serial.print("power:\t");
  Serial.println(p.power);
  Serial.print("noise:\t");
  Serial.println(p.noise);
  Serial.print("currentVoltConst:\t");
  Serial.println(p.currentVoltConst);
  Serial.print("voltage:\t");
  Serial.println(p.voltage);
  Serial.print("thPower:\t");
  Serial.println(p.thPower);
  Serial.print("_socketEnabled:\t");
  Serial.println(_socketEnabled);
  Serial.print(ec);
#endif
}

void set_voltage() {
  p.voltage = cmdMessenger.readFloatArg();
}

void set_thPower() {
  p.thPower = cmdMessenger.readInt16Arg();
}

void send_power()
{
  String power = String(p.power);
  //cmdMessenger.sendCmd(2, power);
  Serial.println(power);
}

void disable_socket() {
  String response = "OFF";
  _socketEnabled = cmdMessenger.readBoolArg();
  if (_socketEnabled) { // socket was set to on
    response = "ON";
    //cmdMessenger.sendCmd(3, response);
    digitalWrite(piSocket, LOW);
  }
  else {//socket was set to off
    //cmdMessenger.sendCmd(3, response);
    digitalWrite(piSocket, HIGH); // the relay is normally closed so high on relay will disable it!
  }
}
void get_power() {
  if (_cc < _nSamples) { // measure until 40 measurments
    p.currentSample[_cc] = analogRead(p.piPower) - p.noise; //get analog read value
    p.currentSample[_cc] *= p.currentSample[_cc] ;//square the measurment
    _cc++;
  } //if
  else {
    for (uint8_t i = 0; i < _nSamples; i++) { //sum all current values
      p.RMSCurrent += p.currentSample[i]  ;
    }//for
    p.RMSCurrent /= _nSamples;
    p.RMSCurrent = sqrt(p.RMSCurrent);
    p.power = p.RMSCurrent * p.voltage * p.currentVoltConst;
    if ((int)p.power < p.thPower) { //ignore low power!
      p.power = 0.0;
    }
    _cc = 0;
  } //else
} // get_power

int getNoise(byte analogPin, byte nSamples) { //this function returns mean value o a current sensor
  long sum = 0;
  for (byte i = 0; i < nSamples; i++) {
    sum = sum + analogRead(analogPin);
    delay(1);// makes sure ADC refreshes
  }
  sum = sum / nSamples;
  //Serial.println(sum);
  return sum;
}

void setup()
{
  Serial.begin(115200);
  pinMode(piSocket, OUTPUT );
  digitalWrite(piSocket, LOW);
  pinMode(p.piPower, INPUT);
  p.noise = getNoise(p.piPower, 60); // set noise floor when no current is flowing throug sensor
  cmdMessenger.printLfCr(); // Adds newline to every command
  attachCommandCallbacks();
  //cmdMessenger.sendCmd(0, "Arduino has started!");  //boot message
  Timer1.initialize(_measurePeriod);
  Timer1.attachInterrupt(get_power, _measurePeriod);
}


void loop()
{
  cmdMessenger.feedinSerialData();//must be called always
}


