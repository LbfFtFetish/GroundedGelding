#include <mcp_can.h>
#include <SPI.h>
#include <time.h>
#include <stdio.h>

#define CAN0_INT 2                            
MCP_CAN CAN0(10);

byte BussMessage[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
byte ReturnMessage[8] = {0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

long unsigned int ControllerAddress =  0x000;
long unsigned int RearReceiverAddress = 0x002;
long unsigned int MessageAddress =  0x000;

int CanFrame = 0;
unsigned char DataLength = 8;
int LastblinkaCheck =0;
boolean TurnSignalStaus = 0;

//Define Pin Settings
int LeftTurnPin = 3;
int RightTurnPin = 4;
int HeadLightsPin = 5;
int HighLightsPin = 6;
int HornPin = 7;
int WindShieldWashPin = 8;
int WindShieldWipePin = 9;
int HazardPin = A0;


//Define Pin Values
boolean LeftTurnVal = LOW;
boolean RightTurnVal = LOW;
boolean HeadLightsVal = LOW;
boolean HighLightsVal = LOW;
boolean HornVal = LOW;
boolean WindShieldWashVal = LOW;
boolean WindShieldWipeVal = LOW;
boolean HazardVal = LOW;

void CheckForMessage();
void UpdateExternalIndicators();
void ConfirmUpdateReceived();
void ManageBlinkaPace(long unsigned int);
void UpdateBlinka(long unsigned int);
void UpdateLeftBlinka();
void UpdateRightBlinka();
void UpdateHazadBlinka();

void setup()
{
 while(CAN0.begin(MCP_ANY, CAN_20KBPS, MCP_8MHZ) != CAN_OK)
 {
   delay(1000);
 } 
 CAN0.setMode(MCP_NORMAL); 
 pinMode(CAN0_INT,INPUT);  
 pinMode(LeftTurnPin,OUTPUT); 
 pinMode(RightTurnPin,OUTPUT); 
 pinMode(HeadLightsPin,OUTPUT); 
 pinMode(HighLightsPin,OUTPUT); 
 pinMode(HornPin,OUTPUT); 
 pinMode(WindShieldWashPin,OUTPUT); 
 pinMode(WindShieldWipePin,OUTPUT); 
 pinMode(HazardPin,OUTPUT); 
}

void loop()
{
 CheckForMessage();
 if(MessageAddress==RearReceiverAddress) 
 {
   UpdateExternalIndicators();
   ConfirmUpdateReceived();
 }
}

void CheckForMessage()
{
   if(!digitalRead(CAN0_INT)) 
   {
    CAN0.readMsgBuf(&MessageAddress,&DataLength,BussMessage);  
   }
}

void UpdateExternalIndicators()
{
 //Read the message from the indicator controller
 // FrontBussMessage[8] = {TurnVal,LightVal,HornVal,WiperVal,FREE,FREE,FREE,FREE};
 //     TurnVal:    LightVal: HornVal: WiperVal:
 //     0x00=Off    0x00=Off  0x00=Off 0x00=Off
 //     0x01=Left   0x01=On   0x01=On  0x01=Wipe
 //     0x02=Right  0x02=High          0x02=Wash
 //     0x03=Hazard
 //
 //     pinMode(pin, INPUT);           
 //     digitalWrite(pin, HIGH); 
 //Read Message Turn Val 
 if(BussMessage[1]==0X03) ManageBlinkaPace(0X03); 
 if(BussMessage[1]==0X02) ManageBlinkaPace(0X02); 
 if(BussMessage[1]==0X01) ManageBlinkaPace(0X01);
 if(BussMessage[1]==0X00) ManageBlinkaPace(0X00);
 //Read Message Light Val 
 if(BussMessage[2]==0X02)  
 {
   digitalWrite(HeadLightsPin,HIGH); 
   digitalWrite(HighLightsPin,HIGH);
 }
 if(BussMessage[2]==0X01)  
 {
   digitalWrite(HeadLightsPin,HIGH); 
   digitalWrite(HighLightsPin,LOW);
 }
 if(BussMessage[2]==0X00)  
  {
   digitalWrite(HeadLightsPin,LOW); 
   digitalWrite(HighLightsPin,LOW);
  }
 //Read Message Horn Val
 if(BussMessage[3]==0X01)  
 {
   digitalWrite(HornPin,HIGH); 
 }
 if(BussMessage[3]==0X00)  
 {
   digitalWrite(HornPin,LOW); 
 }
 //Read Message Wiper Val
 if(BussMessage[4]==0X02)  
  {
   digitalWrite(WindShieldWashPin,HIGH); 
   digitalWrite(HighLightsPin,HIGH);
  }
 if(BussMessage[4]==0X01)  
  {
   digitalWrite(WindShieldWipePin,HIGH); 
   digitalWrite(WindShieldWashPin,LOW);
  }
 if(BussMessage[4]==0X00)  
  {
   digitalWrite(WindShieldWipePin,LOW); 
   digitalWrite(WindShieldWashPin,LOW);
  }
}

void ConfirmUpdateReceived()
{
  CAN0.sendMsgBuf(ControllerAddress,CanFrame,DataLength,ReturnMessage);
}

void ManageBlinkaPace(long unsigned int TurnVal )
{
  int blinkaCheck = time(0);
  int blinkaDelay = 5000;
  if(((double)(blinkaCheck - LastblinkaCheck) / 1000000 ) > blinkaDelay)
  {
    LastblinkaCheck=blinkaCheck;
    UpdateBlinka(TurnVal);
  }
}

void UpdateBlinka(long unsigned int TurnVal )
{
  if(TurnVal==0X00)
  {
   digitalWrite(LeftTurnPin,LOW);
   digitalWrite(RightTurnPin,LOW);
  }
  if(TurnVal==0X01)
  {
   UpdateLeftBlinka();      
  }
  if(TurnVal==0X02)
  {
   UpdateRightBlinka();
  }
  if(TurnVal==0X03)
  {
   UpdateHazadBlinka();
  }
}

void UpdateLeftBlinka()
{
  digitalWrite(RightTurnPin,LOW);
  if(LeftTurnPin==LOW)
  {
   digitalWrite(LeftTurnPin,HIGH);
  }
  if(LeftTurnPin==HIGH)
  {
   digitalWrite(LeftTurnPin,LOW);
  }
}

void UpdateRightBlinka()
{
  digitalWrite(LeftTurnPin,LOW);
  if(RightTurnPin==LOW)
  {
   digitalWrite(RightTurnPin,HIGH);
  }
  if(RightTurnPin==HIGH)
  {
   digitalWrite(RightTurnPin,LOW);
  }
}

void UpdateHazadBlinka()
{
  if(RightTurnPin==LOW)
  {
   digitalWrite(LeftTurnPin,HIGH);
   digitalWrite(RightTurnPin,HIGH);
  }
  if(RightTurnPin==HIGH)
  {
   digitalWrite(LeftTurnPin,LOW);
   digitalWrite(RightTurnPin,LOW);
  }
}