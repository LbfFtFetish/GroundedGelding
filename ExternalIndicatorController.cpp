//gather required libraries
#include <mcp_can.h>

//Set interrupt to pin 2
#define CAN0_INT 2 
//Set CS to pin 10
MCP_CAN CAN0(10);     

//Define buss message
byte FrontBussMessage[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
byte RearBussMessage[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
byte ReturnBussMessage[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//Map out the neighborhood
long unsigned int FromAddress =  0x000;
long unsigned int ControllerAddress = 0x001;
long unsigned int FrontReceiverAddress = 0x002;
long unsigned int RearReceiverAddress = 0x003;

//Define Pin Settings
int LeftTurnPin = 9;
int RightTurnPin = 8;
int BrakePin = 7;
int ReversePin = 6;
int HeadLightsPin = 5;
int HighLightsPin = 4;
int HornPin = 3;
int WindShieldWashPin = A0;
int WindShieldWipePin = A1;
int HazardPin = A2;

//Define Pin Values
boolean LeftTurnVal = LOW;
boolean RightTurnVal = LOW;
boolean BrakeVal = LOW;
boolean ReverseVal = LOW;
boolean HeadLightsVal = LOW;
boolean HighLightsVal = LOW;
boolean HornVal = LOW;
boolean WindShieldWashVal = LOW;
boolean WindShieldWipeVal = LOW;
boolean HazardVal = LOW;

//Define CAN Settings
unsigned char Messagelen = 0;
int CanFrame = 0;
int DataLength = 8;

//Function declarations
int SampleUserInput();
int SendFrontMessage();
int SendRearMessage();

//On Setup
void setup()
{
while(CAN0.begin(MCP_ANY, CAN_20KBPS, MCP_8MHZ) != CAN_OK)
{
  delay(1000);
}
CAN0.setMode(MCP_NORMAL); 
}

void loop()
{
  SampleUserInput();
  SendFrontMessage();
  SendRearMessage();
  delay(250);
}

int SampleUserInput()
{
  LeftTurnVal = digitalRead(LeftTurnPin);
  RightTurnVal = digitalRead(RightTurnPin);
  BrakeVal = digitalRead(BrakePin);
  ReverseVal = digitalRead(ReversePin);
  HeadLightsVal = digitalRead(HeadLightsPin);
  HighLightsVal = digitalRead(HighLightsPin);
  HornVal = digitalRead(HornPin);
  WindShieldWashVal = digitalRead(WindShieldWashPin);
  WindShieldWipeVal = digitalRead(WindShieldWipePin);
  HazardVal = digitalRead(HazardPin);
  return 0;
}

int SendFrontMessage()
{
//Build the message for the front end
// FrontBussMessage[8] = {TurnVal,LightVal,HornVal,WiperVal,FREE,FREE,FREE,FREE};
//     TurnVal:    LightVal: HornVal: WiperVal:
//     0x00=Off    0x00=Off  0x00=Off 0x00=Off
//     0x01=Left   0x01=On   0x01=On  0x01=Wipe
//     0x02=Right  0x02=High          0x02=Wash
//     0x03=Hazard
//
//Build Turn Val
if(HazardVal==HIGH) FrontBussMessage[1] = 0X03;
else if (RightTurnVal==HIGH) FrontBussMessage[1] = 0X02; 
else if (LeftTurnVal==HIGH) FrontBussMessage[1] = 0X01;
else FrontBussMessage[1] = 0X00; 
//Build Light Val
if(HazardVal==HIGH) FrontBussMessage[2] = 0X01;
else if (HeadLightsVal==HIGH) FrontBussMessage[2] = 0X01;
else if (HighLightsVal==HIGH) FrontBussMessage[2] = 0X02;
else FrontBussMessage[2] = 0X00;  
//Build Horn Val
if(HornVal==HIGH) FrontBussMessage[3] = 0X01;
else FrontBussMessage[3] = 0X00;
//Build Wiper Val
if(WindShieldWipeVal==HIGH) FrontBussMessage[4] = 0X01;
else if (WindShieldWashVal==HIGH) FrontBussMessage[4] = 0x02;
else FrontBussMessage[4] = 0X00;
//  Flush Any Pending Responce
 if(!digitalRead(CAN0_INT)) 
 {
   CAN0.readMsgBuf(&ControllerAddress,&Messagelen,ReturnBussMessage);
  }
//  Send request while no responce 
while(digitalRead(CAN0_INT) != LOW)
 {
   CAN0.sendMsgBuf(FrontReceiverAddress,CanFrame,DataLength,FrontBussMessage);
   delay(10);
  }
return 0;
}

int SendRearMessage()
{
//Build the message for the rear end
// RearBussMessage[8] = {TurnVal,LightVal,BrakeVal,ReverseVal,FREE,FREE,FREE,FREE};
//     TurnVal:    LightVal: BrakeVal: ReverseVal
//     0x00=Off    0x00=Off  0x00=Off  0x00=Off   
//     0x01=Left   0x01=On   0x01=On   0x01=On
//     0x02=Right           
//     0x03=Hazard
//
//Build Turn Val
if(HazardVal==HIGH) RearBussMessage[1] = 0X03;
else if (RightTurnVal==HIGH) RearBussMessage[1] = 0X02; 
else if (LeftTurnVal==HIGH) RearBussMessage[1] = 0X01;
else RearBussMessage[1] = 0X00; 
//Build Light Val
if(HeadLightsVal==HIGH) RearBussMessage[2] = 0X01;
else if (HighLightsVal==HIGH) RearBussMessage[2] = 0X01;
else RearBussMessage[2] = 0X00;  
//Build Brake Val
if(BrakeVal==HIGH) RearBussMessage[3] = 0X01;
else RearBussMessage[3] = 0X00;
//Build Brake Val
if(ReverseVal==HIGH) RearBussMessage[4] = 0X01;
else RearBussMessage[4] = 0X00;
//  Flush Any Pending Responce
 if(!digitalRead(CAN0_INT)) 
 {
   CAN0.readMsgBuf(&ControllerAddress,&Messagelen,ReturnBussMessage);
  }
//  Send request while no responce 
while(digitalRead(CAN0_INT) != LOW)
 {
   CAN0.sendMsgBuf(RearReceiverAddress,CanFrame,DataLength,RearBussMessage);
   delay(10);
  }
return 0;
}