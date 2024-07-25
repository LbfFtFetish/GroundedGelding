//LCD SPI library
#include <LCDWIKI_SPI.h> 
//Can Com libraries
#include <mcp_can.h>

//  CAN settings 
byte BussMessage[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
long unsigned int FromAddress =  0x000;
unsigned char Messagelen = 0;
int CanFrame = 0;
int DataLength = 8;
#define CAN0_INT 2 
//Set CS to pin 
MCP_CAN CAN0(10);
int CanSelectPin = 2;
boolean CanSelectPinVal = HIGH;

//  LCD settings 
int LCD_CS=11;
int LCD_RST=8;
int LCD_RS=9;
int SDI_MOSI_LCD=51;
int SCK_LCD=52;
int LED=5;
int SDO_MISO_LCD=50;
LCDWIKI_SPI mylcd(ILI9488,LCD_CS,LCD_RS,SDO_MISO_LCD,SDI_MOSI_LCD,LCD_RST,SCK_LCD,LED); 
#define BLACK   0x0000
#define RED     0xF800
#define GREEN   0x07E0
#define LineHight 25 
#define CharWidthSize3  15
#define CharWidthSize4 15
#define ScreenWidth  480
#define ScreenHight 320
#define ScreenLine1 (LineHight*0)
#define ScreenLine2 (LineHight*1) 
#define ScreenLine3 (LineHight*2) 
#define ScreenLine4 (LineHight*3) 
#define ScreenLine5 (LineHight*4) 
#define ScreenLine6 (LineHight*5) 
#define ScreenLine7 (LineHight*6) 
#define ScreenLine8 (LineHight*7)   
int LcdSelectPin = 11;
boolean LcdSelectPinVal = HIGH;

//  User Control Settings
int ScanPausePin = 3;
boolean PausePinVal = HIGH;

// Screen Control
int LineCounter = 3;
int DataCounter = 0;
char LineNumberString[28];
char LineData[28];
char LineDataPoint[28];
void GetSample();
void MaybePause();
void StartCan();
void StartLcd();

void setup() 
{
  Serial.begin(9600);
  Serial.println(" ");
  Serial.println("Setup:");
  StartCan();
  SPI.end();
  StartLcd();
  }

void loop() 
{    
Serial.println("Top Of Loop");
GetSample();
MaybePause();
}

void GetSample() 
{
 Serial.println("Get Sample");
 if(!digitalRead(CAN0_INT))
 {
   Serial.println("Found Pending Txn");
   CAN0.readMsgBuf(&FromAddress,&Messagelen,BussMessage);
   //  Can commands do not seem to call SPI.end();
   //  SPI changes made in LCD.SPI to use modern SPI.begin(XXXXXXX);  may not be required
   //  BussMessage display is in proof of concept form only.   
   SPI.end();
   sprintf(LineData,"%lu",FromAddress);
   Serial.println(LineData);
   strcat(LineData,":");
   Serial.println(LineData);
   sprintf(LineDataPoint,"%u",BussMessage[1]);
   strcat(LineData,LineDataPoint);
   strcat(LineData,":");
   Serial.println(LineData);
   sprintf(LineDataPoint,"%u",BussMessage[2]);
   strcat(LineData,LineDataPoint);
   strcat(LineData,":");
   Serial.println(LineData);
   sprintf(LineDataPoint,"%u",BussMessage[3]);
   strcat(LineData,LineDataPoint);
   strcat(LineData,":");
   Serial.println(LineData);
   sprintf(LineDataPoint,"%u",BussMessage[4]);
   strcat(LineData,LineDataPoint);
   strcat(LineData,":");
   Serial.println(LineData);
   sprintf(LineDataPoint,"%u",BussMessage[5]);
   strcat(LineData,LineDataPoint);
   strcat(LineData,":");
   Serial.println(LineData);
   sprintf(LineDataPoint,"%u",BussMessage[6]);
   strcat(LineData,LineDataPoint);
   strcat(LineData,":");
   Serial.println(LineData);
   sprintf(LineDataPoint,"%u",BussMessage[7]);
   strcat(LineData,LineDataPoint);
   strcat(LineData,":");
   Serial.println(LineData);
   mylcd.Print_String(LineData,0,(LineCounter*LineHight));
 }
  else
 {
   Serial.println("No Pending Txn");
   mylcd.Print_String("                           ",0,(LineCounter*LineHight));
   mylcd.Print_String("No Pending Txn             ",0,(LineCounter*LineHight));
 }
    if(LineCounter==11) 
    {
     LineCounter=3;
    }
   else LineCounter++;
}

void MaybePause()
{
  Serial.println("Maybe Pause"); 
  PausePinVal = digitalRead(ScanPausePin);
  if(PausePinVal==LOW)
  {
   mylcd.Set_Draw_color(RED);
   mylcd.Fill_Rectangle(0,ScreenLine2+10,ScreenWidth,ScreenLine4-1); 
   mylcd.Set_Text_Size(4);
   mylcd.Set_Text_Back_colour(RED);
   mylcd.Set_Text_colour(GREEN);
   mylcd.Print_String("Start Sampler",(CharWidthSize4*5),ScreenLine3-(LineHight/2));
   while(PausePinVal==LOW)
   {
    delay(500);
    PausePinVal = digitalRead(ScanPausePin);
   }
   mylcd.Set_Draw_color(GREEN);
   mylcd.Fill_Rectangle(0,ScreenLine2+10,ScreenWidth,ScreenLine4-1); 
   mylcd.Set_Text_Back_colour(GREEN);
   mylcd.Set_Text_colour(RED);
   mylcd.Print_String("Stop Sampler",(CharWidthSize4*5),ScreenLine3-(LineHight/2));
   mylcd.Set_Text_Size(3);
   mylcd.Set_Text_Back_colour(BLACK);
   }
}

void StartCan()
{
 pinMode(CanSelectPin,OUTPUT);
 Serial.println("Init CAN mcp_any 20kBPS at 8MHz.");
 while(CAN0.begin(MCP_ANY,CAN_20KBPS,MCP_8MHZ) != CAN_OK)
   {
   Serial.println("Error Initializing MCP2515.");
   delay(500);
  } 
 Serial.println("MCP2515 Initialized Successfully.");
 Serial.println("MCP2515 Set Mode NORMAL.");
 CAN0.setMode(MCP_LISTENONLY);                     
 Serial.println("MCP2515 Set Pin 2 To Interrupt.");
 pinMode(CAN0_INT,INPUT);                            
 Serial.println("CAN Rear Receiver Setup: Complete.");
}

void StartLcd()
{
 Serial.println("Init LCD");
 mylcd.Init_LCD();
 mylcd.Set_Text_colour(RED);
 mylcd.Set_Text_Back_colour(BLACK);
 mylcd.Set_Rotation(1);
 mylcd.Set_Draw_color(GREEN);
 mylcd.Set_Text_Mode(0);
 mylcd.Set_Text_Size(3);
 Serial.println("Clear Screen"); 
 mylcd.Fill_Screen(BLACK);
 mylcd.Print_String("External Indicator Monitor ",0,ScreenLine1);
 mylcd.Set_Draw_color(GREEN);
 mylcd.Fill_Rectangle(0,ScreenLine2+10,ScreenWidth,ScreenLine4-1); 
 mylcd.Set_Text_Size(4);
 mylcd.Set_Text_Back_colour(GREEN);
 mylcd.Print_String("Stop Sampler",(CharWidthSize4*5),ScreenLine3-(LineHight/2));
 mylcd.Set_Text_Size(3);
 mylcd.Set_Text_Back_colour(BLACK);
}

