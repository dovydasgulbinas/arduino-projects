
#include <CmdMessenger.h>  // CmdMessenger
CmdMessenger cmdMessenger = CmdMessenger(Serial);


  
void attachCommandCallbacks()// All callbacks must be added here!
{
  cmdMessenger.attach(1, response1); 
  cmdMessenger.attach(2, response2);
  cmdMessenger.attach(OnUnknownCommand);// default command ,ust be placed at the end!
}

void OnUnknownCommand() // Called when a received command has no attached function
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
  
  cmdMessenger.printLfCr(); // Adds newline to every command
  attachCommandCallbacks();
  cmdMessenger.sendCmd(0, "Arduino has started!");  //boot message

}


void loop()
{


  
  cmdMessenger.feedinSerialData();//must be called always
}


