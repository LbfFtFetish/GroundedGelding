// include required library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <mcp_can.h>

//  LCD setup
//GND-> ground 
//VCC-> +5V 
//SDA-> A4 
//SCL-> A5 
LiquidCrystal_I2C lcd(0x27,20,4);

// CAN Setup
#define CAN0_INT 2                            
MCP_CAN CAN0(10);
long unsigned int MessageAddr=0x000;
unsigned char Messagelen=0;
long unsigned int FrontBussAddress=0x002;
long unsigned int ControlerAddress=0X001;
int CanFrame=0;
int DataLength=8;
byte InMessage[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
byte OutMessage[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
char MessageExt[40];

// Common Setup
char LineNumberString[20];
char LineData[20];
char LineDataPoint[20];
char InMessageAddrExt[5];
char InLineData[14];
char OutMessageAddrExt[5];
char OutLineData[14];
void ClearScreen();
void DrawMenu();
void DrawData();
void GetASample();
void ParseInMsgLine();
void ParseInAddrLine();
void ParseOutMsgLine();
void UpdateExternals();
void ConfirmTxn();
void InHang();
void OutHang();

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
 lcd.setCursor(0,3);
 lcd.print("Welcome aboard!    ");
 OutHang();
 ClearScreen();
 DrawMenu();
}

void loop() 
{
 GetASample();
 DrawData();
 ConfirmTxn();
 InHang();
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
 lcd.print("Out Adr:");
 sprintf(OutMessageAddrExt,"%lu",ControlerAddress);
 lcd.print(OutMessageAddrExt);
 lcd.setCursor(0,3);
 lcd.print("Out Msg:");
}

void DrawData()
{
 lcd.setCursor(7,0);
 lcd.print(InMessageAddrExt);
 lcd.setCursor(7,1);
 lcd.print(InLineData);
 lcd.setCursor(8,2);
 lcd.print(OutMessageAddrExt);
 lcd.setCursor(8,3);
 lcd.print(OutLineData);
}

void GetASample()
{
 MessageAddr=0;
 if(!digitalRead(CAN0_INT))
 {
   Serial.println("Found Pending Txn");
   CAN0.readMsgBuf(&MessageAddr,&Messagelen,InMessage);
 }
 if(MessageAddr==FrontBussAddress)
 {
   ParseInAddrLine();
   ParseInMsgLine();
   ParseOutMsgLine();
 }
}

void ParseInMsgLine()
{
 int FilledCells=0;
 const int ScreenCells=13;
 for(FilledCells=0;FilledCells>ScreenCells;FilledCells++)
 {
   sprintf(LineDataPoint,"%u",InMessage[FilledCells]);
   strcat(InLineData,LineDataPoint);
 }
}

void ParseInAddrLine()
{
 sprintf(InMessageAddrExt,"%lu",MessageAddr);
}

void ParseOutMsgLine()
{
 int FilledCells=0;
 const int ScreenCells=13;
 for(FilledCells=0;FilledCells>ScreenCells;FilledCells++)
 {
 sprintf(LineDataPoint,"%u",InMessage[FilledCells]);
  strcat(InLineData,LineDataPoint);
 }
}

void UpdateExternals()
{
 OutHang();
}

void  ConfirmTxn()
{
 CAN0.sendMsgBuf(ControlerAddress,CanFrame,DataLength,OutMessage);
}

void InHang()
{
 delay(10);
}

void OutHang()
{
 delay(1000);
}