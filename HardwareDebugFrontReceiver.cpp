#include <mcp_can.h>
#include <SPI.h>

#define CAN0_INT 2                            
MCP_CAN CAN0(10);

long unsigned int MessageAddr =  0x000;
unsigned char Messagelen = 0;
long unsigned int FrontBuss = 0x001;
int CanFrame = 0;
int DataLength = 8;
byte BussMessage[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
byte ReturnMessage[8] = {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
long unsigned int ReturnAddr =  0x000;

void setup()
{
 Serial.begin(9600);
 Serial.println("CAN Front Receiver Setup Start.");
 Serial.println("Init CAN mcp_any 20kBPS at 8Hz.");
 while(CAN0.begin(MCP_ANY, CAN_20KBPS, MCP_8MHZ) != CAN_OK)
  {
    Serial.println("Error Initializing MCP2515.");
   delay(5000);
  } 
 Serial.println("MCP2515 Initialized Successfully.");
 Serial.println("MCP2515 Set Mode NORMAL.");
 CAN0.setMode(MCP_NORMAL);                     
 Serial.println("MCP2515 Set Pin 2 To Interrupt.");
 pinMode(CAN0_INT,INPUT);                            
 Serial.println("CAN Front Receiver Setup: Complete.");
}

void loop()
{
 delay(1000);
 Serial.println("CAN Front Receiver Checking Interrupt.");
 if(!digitalRead(CAN0_INT)) 
   {CAN0.readMsgBuf(&MessageAddr,&Messagelen,BussMessage);      
   if(MessageAddr==FrontBuss) 
   {
     Serial.println("CAN Front Receiver Input Found.");
     CAN0.sendMsgBuf(ReturnAddr,CanFrame,DataLength,ReturnMessage);
     Serial.println("CAN Front Receiver Return Sent.");
    }
  }
}
