//Core graphics library
#include <LCDWIKI_GUI.h> 
//Hardware-specific library
#include <LCDWIKI_SPI.h> 

// Screen Pin Fuunction
// 1 : VCC       -  5V or 3.3V for dim backlight
// 2 : GND       -  Power Ground
// 3 : LCD_CS    -  LCD selection control signal , low level active
// 4 : LCD_RST   -  LCD reset control signal, Low level reset 
// 5 : LCD_RS    -  LCD command / data selection control signal. High level: data, low level: command 
// 6 : SDI(MOSI) -  SPI bus write data signal(SD card and LCD screen used together) 
// 7 : SCK       -  SPI bus clock signal(SD card and LCD screen used together) 
// 8 : LED       -  LCD backlight control signal (optional)
// 9 : SDO(MISO) -  SPI bus read data signal (SD card and LCD screen used together) 
// 10: CTP_SCL   -  Capacitive touch screen IIC bus clock signal
// 11: CTP_RST   -  Capacitor touch screen reset control signal, low-level reset 
// 12: CTP_SDA   -  Capacitive touch screen IIC bus data signal 
// 13: CTP_INT   -  Capacitor touch screen IIC bus touch interrupt signal
// 14: SD_CS     -  SD card selection control signal, low level active

//  Screen Pin Layout
//  1.VCC 5V 
//  2.GND GND
//  3.LCD_CS 
int LCD_CS=10;
// 4 : LCD_RST 
int LCD_RST=8;
// 5 : LCD_RS 
int LCD_RS=9;
// 6 : SDI(MOSI) 
int SDI_MOSI_LCD=51;
// 7 : SCK
int SCK_LCD=52;
// 8 : LED
int LED=5;
// 9 : SDO(MISO)
int SDO_MISO_LCD=50;
//          mylcd(ILI9488,10,9,50,51,8,52,5);
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

boolean PauseVal = HIGH;
int PausePin = 3;
int LineCounter = 3;
void ClearScreen();
void GetSample();
void MaybePause();

void setup() 
{
  Serial.begin(9600);
  Serial.println(" ");
  Serial.println("Setup:");
  mylcd.Init_LCD();
  mylcd.Set_Text_colour(RED);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Rotation(1);
  mylcd.Set_Draw_color(GREEN);
  mylcd.Set_Text_Mode(0);
  mylcd.Set_Text_Size(3);
  ClearScreen();
}

void loop() 
{    
Serial.println("Top Of Loop");
GetSample();
MaybePause();
}

void ClearScreen() 
{ 
 Serial.println("clear screen");
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

void GetSample() 
{ 
   Serial.println("get sample");
   mylcd.Print_String("                           ",0,(LineCounter*LineHight));
   mylcd.Print_String("Line Data:123456789ABCDEFG ",0,(LineCounter*LineHight));
   if(LineCounter==11) 
    {
     LineCounter=3;
    }
   else LineCounter++;
}

void MaybePause()
{
   Serial.println("maybe pause");
  PauseVal = digitalRead(PausePin);
  if(PauseVal==LOW)
  {
   mylcd.Set_Draw_color(RED);
   mylcd.Fill_Rectangle(0,ScreenLine2+10,ScreenWidth,ScreenLine4-1); 
   mylcd.Set_Text_Size(4);
   mylcd.Set_Text_Back_colour(RED);
   mylcd.Set_Text_colour(GREEN);
   mylcd.Print_String("Start Sampler",(CharWidthSize4*5),ScreenLine3-(LineHight/2));
   while(PauseVal==LOW)
   {
    delay(500);
    PauseVal = digitalRead(PausePin);
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