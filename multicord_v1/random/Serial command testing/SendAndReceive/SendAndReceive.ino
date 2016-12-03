
#include <CmdMessenger.h>  // CmdMessenger


CmdMessenger cmdMessenger = CmdMessenger(Serial);




  // All callbacks must be added here!
void attachCommandCallbacks()
{
  cmdMessenger.attach(1, response1); 
  cmdMessenger.attach(2, response2);
  cmdMessenger.attach(OnUnknownCommand);// default command ,ust be placed at the end!
}

// Called when a received command has no attached function
void OnUnknownCommand()
{
  cmdMessenger.sendCmd(0, "ERROR NO CMD");
}


void response1()
{
  cmdMessenger.sendCmd(10, "\t@Response1");
  Serial.println("CYKA NUGGETS!");
}

void response2(){
  cmdMessenger.sendCmd(11, "\t@Response2");
}

void setup()
{
  Serial.begin(115200);
  // Adds newline to every command
  cmdMessenger.printLfCr();
  attachCommandCallbacks();
  //boot message
  cmdMessenger.sendCmd(0, "Arduino has started!");

}


void loop()
{


  //must be called always
  cmdMessenger.feedinSerialData();
}


