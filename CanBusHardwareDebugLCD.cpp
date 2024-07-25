// include required library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

//  LCD setup
//GND-> ground 
//VCC-> +5V 
//SDA-> A4 
//SCL-> A5 
// 20X4 chars
LiquidCrystal_I2C lcd(0x27,20,4);

// Common Setup
void InitScreen();
void ClearScreen();
void DrawMenu();
void DrawTestData();

void setup()
{
 InitScreen();
 ClearScreen();
 DrawMenu();
 DrawTestData();
}

void loop() 
{
 delay(1000);
}

void InitScreen()
{
 lcd.init();
 lcd.backlight();
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

void DrawTestData()
{
 lcd.setCursor(7,0);
 lcd.print("123456789ABCD");
 lcd.setCursor(7,1);
 lcd.print("123456789ABCD");
 lcd.setCursor(7,2);
 lcd.print("123456789ABCD");
 lcd.setCursor(7,3);
 lcd.print("123456789ABCD");
}

void DrawMenu()
{
 lcd.setCursor(0,0);
 lcd.print("In Adr:");
 lcd.setCursor(0,1);
 lcd.print("In Msg:");
 lcd.setCursor(0,2);
 lcd.print("OutAdr:");
 lcd.setCursor(0,3);
 lcd.print("OutMsg:");
}



