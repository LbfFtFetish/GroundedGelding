//gather required libraries
#include <mcp_can.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//  LCD setup
//GND-> ground 
//VCC-> +5V 
//SDA-> A4 
//SCL-> A5 
LiquidCrystal_I2C lcd(0x27,20,4);
char OutMessageAddrExt[5];
char InMessageAddrExt[5];
char LineDataPoint[13];
char OutLineData[13];
char InLineData[13];

//Set interrupt to pin 2
#define CAN0_INT 2 
//Set CS to pin 10
MCP_CAN CAN0(10);     

//Define buss message
byte FrontBussMessage[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
byte RearBussMessage[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
byte MessageOnBuss[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//Map out the neighborhood
long unsigned int MessageAddress = 0x000;
long unsigned int ControllerAddress = 0x001;
long unsigned int FrontReceiverAddress = 0x002;
long unsigned int RearReceiverAddress = 0x003;

//Define Pin Settings
int InputHighPin=9;
int LeftTurnPin = 8;
int RightTurnPin = 7;
int BrakePin = 6;
int ReversePin = 5;
int HeadLightsPin = 4;
int HighLightsPin = 3;
int HornPin = A0;
int WindShieldWashPin = A1;
int WindShieldWipePin = A2;
int HazardPin = A3;

//Define Pin Values
boolean InputeHighVal = HIGH;
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
void ClearScreen();
void DrawMenu();
void DrawData();
void ParseInMsgLine();
void ParseInAddrLine();
void ParseRearMsgLine();
void ParseFrontMsgLine();
int MaybeReturnMessage();
void InHang();
void OutHang();

//On Setup
void setup()
{
 lcd.init();
 lcd.backlight();
 lcd.setCursor(0,0);
 lcd.print("LCD setup complete!");
 OutHang();
 lcd.setCursor(0,1);
 lcd.print("Init CAN 20k at 8Hz");
 OutHang();
 while(CAN0.begin(MCP_ANY, CAN_20KBPS, MCP_8MHZ) != CAN_OK)
  {
   lcd.setCursor(0,0);
   lcd.print("MCP2515 Init Error.");
   lcd.setCursor(0,1);
   lcd.print("                   ");
   OutHang();
  } 
 lcd.setCursor(0,2);
 lcd.print("MCP2515 Init OK.   ");
 OutHang();
 CAN0.setMode(MCP_NORMAL);
 pinMode(CAN0_INT,INPUT);
 pinMode(InputHighPin,OUTPUT),
 digitalWrite(InputHighPin,HIGH);   
 lcd.setCursor(0,3);
 lcd.print("Welcome aboard!    ");
 OutHang();
 ClearScreen();
 DrawMenu();
}

void loop()
{
  SampleUserInput();
  SendFrontMessage();
  SendRearMessage();
  DrawData();
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
   CAN0.readMsgBuf(&MessageAddress,&Messagelen,MessageOnBuss);
  }
ParseFrontMsgLine();
//  Send request while no responce 
int GotResponce=0;
while(GotResponce==0)
 {
   CAN0.sendMsgBuf(FrontReceiverAddress,CanFrame,DataLength,FrontBussMessage);
   InHang();
   CAN0.readMsgBuf(&MessageAddress,&Messagelen,MessageOnBuss);
   GotResponce=MaybeReturnMessage();
   InHang();
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
   CAN0.readMsgBuf(&MessageAddress,&Messagelen,MessageOnBuss);
  }
ParseRearMsgLine();
//  Send request while no responce 
while(digitalRead(CAN0_INT) != LOW)
 {
   CAN0.sendMsgBuf(RearReceiverAddress,CanFrame,DataLength,RearBussMessage);
   InHang();
  }
return 0;
}

void ClearScreen()
{
 lcd.setCursor(0,0);
 lcd.print("                   ");
 lcd.setCursor(0,1);
 lcd.print("                   ");
 lcd.setCursor(0,2);
 lcd.print("                   ");
 lcd.setCursor(0,3);
 lcd.print("                   ");
}

void DrawMenu()
{
 lcd.setCursor(0,0);
 lcd.print("In Adr:");
 lcd.setCursor(0,1);
 lcd.print("In Msg:");
 lcd.setCursor(0,2);
 lcd.print("OutAdr:");
 sprintf(OutMessageAddrExt,"%lu",ControllerAddress);
 lcd.print(OutMessageAddrExt);
 lcd.setCursor(0,3);
 lcd.print("OutMsg:");
}

void DrawData()
{
 lcd.setCursor(7,0);
 lcd.print(InMessageAddrExt);
 lcd.setCursor(7,1);
 lcd.print(InLineData);
 lcd.setCursor(7,2);
 lcd.print(OutMessageAddrExt);
 lcd.setCursor(7,3);
 lcd.print(OutLineData);
}

void ParseInMsgLine()
{
  int FilledCells=0;
  const int ScreenCells=13;
  for(FilledCells=0;FilledCells>ScreenCells;FilledCells++)
  {
    sprintf(LineDataPoint,"%u",MessageOnBuss[FilledCells]);
    strcat(InLineData,LineDataPoint);
  }
}

void ParseInAddrLine()
{
 sprintf(InMessageAddrExt,"%lu",MessageAddress);
}

void ParseFrontMsgLine()
{
  int FilledCells=0;
  const int ScreenCells=13;
  for(FilledCells=0;FilledCells>ScreenCells;FilledCells++)
  {
    sprintf(LineDataPoint,"%u",FrontBussMessage[FilledCells]);
    strcat(InLineData,LineDataPoint);
  }
}

void ParseRearMsgLine()
{
  int FilledCells=0;
  const int ScreenCells=13;
  for(FilledCells=0;FilledCells>ScreenCells;FilledCells++)
  {
    sprintf(LineDataPoint,"%u",RearBussMessage[FilledCells]);
    strcat(InLineData,LineDataPoint);
  }
}

int MaybeReturnMessage()
{
  int OurMessage=0;
  if(MessageAddress==ControllerAddress)
  {
     OurMessage=1;
  }
  return OurMessage;
}

void InHang()
{
 delay(10);
}

void OutHang()
{
 delay(1000);
}
