#include <mcp_can.h>
#include <SPI.h>

#define CAN0_INT 2 
MCP_CAN CAN0(10);     

byte AllOn[8] = {0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};
byte AllOff[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
byte BussMessage[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char Messagelen = 0;
long unsigned int MessageAddr =  0x000;
long unsigned int TXNFILER = 0x000;
long unsigned int FrontBuss = 0x001;
long unsigned int RearBuss = 0x002;
int CanFrame = 0;
int DataLength = 8;

void setup()
{
Serial.begin(9600);
Serial.println("CAN Txn Filer Setup:");
pinMode(LED_BUILTIN,OUTPUT);
Serial.println("Pin mode init buit in led output:");
Serial.println("Init CAN mcp_any 20kBPS at 8Hz");
while(CAN0.begin(MCP_ANY, CAN_20KBPS, MCP_8MHZ) != CAN_OK)
{
 Serial.println("Error Initializing MCP2515...");
 delay(5000);
}
 Serial.println("MCP2515 Initialized Successfully!");
 Serial.println("Init CAN mcp normal mode");
 CAN0.setMode(MCP_NORMAL); 
 Serial.println("MCP2515 Set Pin 2 To Interrupt.");
 pinMode(CAN0_INT,INPUT);   
}

void loop()
{
 Serial.println("Flushing Any Pending Response");
 if(!digitalRead(CAN0_INT)) 
 {
   CAN0.readMsgBuf(&MessageAddr,&Messagelen,BussMessage);
 }
 Serial.println("Send Command Front All On");
 while(digitalRead(CAN0_INT) != LOW)
 {
   CAN0.sendMsgBuf(FrontBuss,CanFrame,DataLength,AllOn);
   Serial.println("Waiting For Front All On Resp");
   delay(1000);
  }
 Serial.println("Front All On Response Received");
 CAN0.readMsgBuf(&MessageAddr,&Messagelen,BussMessage);
 Serial.println("Send Command Front All Off");
 while(digitalRead(CAN0_INT) != LOW)
 {
   CAN0.sendMsgBuf(FrontBuss,CanFrame,DataLength,AllOff);
   Serial.println("Waiting For Front All Off Resp");
   delay(1000);
  }
 Serial.println("Front All Off Response Received");
 CAN0.readMsgBuf(&MessageAddr,&Messagelen,BussMessage);

 Serial.println("Send Command Rear All On");
 while(digitalRead(CAN0_INT) != LOW)
 {
   CAN0.sendMsgBuf(RearBuss,CanFrame,DataLength,AllOn);
   Serial.println("Waiting For Rear All On Resp");
   delay(1000);
  }
 Serial.println("Rear All On Response Received");
 CAN0.readMsgBuf(&MessageAddr,&Messagelen,BussMessage);
 Serial.println("Send Command Rear All Off");
 while(digitalRead(CAN0_INT) != LOW)
 {
   CAN0.sendMsgBuf(RearBuss,CanFrame,DataLength,AllOff);
   Serial.println("Waiting For Rear All Off Resp");
   delay(1000);
  }
  Serial.println("Rear All Off Response Received");
 CAN0.readMsgBuf(&MessageAddr,&Messagelen,BussMessage);
}
