// include required library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

//  LCD setup
//GND-> ground 
//VCC-> +5V 
//SDA-> 20 
//SCL-> 21 
// 16X2 chars
LiquidCrystal_I2C lcd1(0x23,16,2);
LiquidCrystal_I2C lcd2(0x26,16,2);
LiquidCrystal_I2C lcd3(0x27,16,2);

// User Inputs
int ThrottleValue = analogRead(A1);
int ThrottleValueMap=0;
int ThrottlePin=A1;
int ThrottleDataRow=0;
int ThrottleMenuRow=0;

// OutPuts
int Igbt1Pin = 2;
int Igbt2Pin = 3;
int Igbt3Pin = 4;
int Igbt4Pin = 5;
int Igbt5Pin = 6;
int Igbt6Pin = 7;
int PwmVal = 0;
int IgbtOff = 0;
float Igbt1Val = 0;
float Igbt2Val = 0;
float Igbt3Val = 0;
float Igbt4Val = 0;
float Igbt5Val = 0;
float Igbt6Val = 0;
int CurrentPhase = 1;
unsigned long TimerMicros = 0;
unsigned long StartMicros = 0;
unsigned long PhaseTimer = 0;
int ScreenGateData=00;

//Motor Sensors
int Hall1Pin=A2;
int Hall2Pin=A3;
int Hall3Pin=A4;
int MotorTempPin=A5;
int Hall1Val=0;
int Hall2Val=0;
int Hall3Val=0;
int MotorTempVal=0;

// Screen data
int IgbtMenuRow=9;
int IgbtDataRow=12;
int ScreenLine1=0;
int ScreenLine2=1;
int LeftMargin=0;

// Common Setup
void InitScreen1();
void InitScreen2();
void InitScreen3();
void ClearScreen1();
void ClearScreen2();
void ClearScreen3();
void DrawScreen1Menu();
void DrawScreen2Menu();
void DrawScreen3Menu();
void DrawScreen1Data();
void DrawScreen2Data();
void DrawScreen3Data();
void CheckThrottle();
void CheckMotor();
void DefinePins();
//   AC Phase defs
void EnterPhase1();
void EnterPhase2();
void EnterPhase3();
void EnterPhase4();
void EnterPhase5();
void EnterPhase6();
void AllOff();
//
void UpdateScreenData();
void PhaseDelay();

void setup()
{
 DefinePins();
 InitScreen1();
 InitScreen2();
 InitScreen3();
 DrawScreen1Menu();
 DrawScreen2Menu();
 DrawScreen3Menu();
}

void loop() 
{
 CheckMotor();
 CheckThrottle();
 UpdateScreenData();
 if(ThrottleValueMap==0)
  {
   AllOff();
   PhaseDelay();
  }
 
 if(ThrottleValueMap>0)
 {
   EnterPhase1();
   EnterPhase2();
   EnterPhase3();
   EnterPhase4();
   EnterPhase5();
   EnterPhase6();
  }
}


void PhaseDelay()
{
  StartMicros=micros();
  TimerMicros=StartMicros;
  while((TimerMicros-StartMicros)<PhaseTimer)
      {
         TimerMicros=micros();
      }
}

void UpdateScreenData()
{
 DrawScreen1Data();
 DrawScreen2Data();
 DrawScreen3Data();
}

void InitScreen1()
{
 lcd1.init();
 lcd1.backlight();
 ClearScreen1();
}

void InitScreen2()
{ 
 lcd2.init();
 lcd2.backlight();
 ClearScreen2();
}

void InitScreen3()
{
 lcd3.init();
 lcd3.backlight();
 ClearScreen3();
}

void ClearScreen1()
{
 lcd1.setCursor(LeftMargin,ScreenLine1);
 lcd1.print("               ");
 lcd1.setCursor(LeftMargin,ScreenLine2);
 lcd1.print("               ");
}

void ClearScreen2()
{
 lcd2.setCursor(LeftMargin,ScreenLine1);
 lcd2.print("               ");
 lcd2.setCursor(LeftMargin,ScreenLine2);
 lcd2.print("               ");
}

void ClearScreen3()
{
 lcd3.setCursor(LeftMargin,ScreenLine1);
 lcd3.print("               ");
 lcd3.setCursor(LeftMargin,ScreenLine2);
 lcd3.print("               ");
}

void DrawScreen1Menu()
{
 lcd1.setCursor(IgbtMenuRow,ScreenLine1);
 lcd1.print("Phase:");
 lcd1.setCursor(IgbtMenuRow,ScreenLine2);
 lcd1.print("+/-G:");
 lcd1.setCursor(ThrottleMenuRow,ScreenLine1);
 lcd1.print("Thr:");
 lcd1.setCursor(ThrottleMenuRow,ScreenLine2);
 lcd1.print("PWM:");
}

void DrawScreen2Menu()
{
 lcd2.setCursor(IgbtMenuRow,ScreenLine1);
 lcd2.print("Hall2:");
 lcd2.setCursor(IgbtMenuRow,ScreenLine2);
 lcd2.print("Hall3:");
 lcd2.setCursor(ThrottleMenuRow,ScreenLine1);
 lcd2.print("Hall1:");
 lcd2.setCursor(ThrottleMenuRow,ScreenLine2);
 lcd2.print("Temp:");
}

void DrawScreen3Menu()
{
 
}

void DrawScreen1Data()
{
 lcd1.setCursor(15,ScreenLine1);
 lcd1.print(CurrentPhase);
 lcd1.setCursor(14,ScreenLine2);
 lcd1.print("  ");
 lcd1.setCursor(14,ScreenLine2);
 lcd1.print(ScreenGateData);
 lcd1.setCursor(4,ScreenLine2);
 lcd1.print("   ");
 lcd1.setCursor(4,ScreenLine2);
 lcd1.print(PwmVal);
 lcd1.setCursor(4,ScreenLine1);
 lcd1.print("   ");
 lcd1.setCursor(4,ScreenLine1);
 lcd1.print(ThrottleValueMap);
}

void DrawScreen2Data()
{
 lcd2.setCursor(6,ScreenLine1);
 lcd2.print(Hall1Val);
 lcd2.setCursor(6,ScreenLine2);
 lcd2.print(Hall2Val);
 lcd2.setCursor(15,ScreenLine2);
 lcd2.print(Hall3Val);
 lcd2.setCursor(5,ScreenLine2);
 lcd2.print(MotorTempVal);
}

void DrawScreen3Data()
{

}

/*
Motor RPM Map Notes:
RPM= (Frequency * 120) / # of poles in the motor
4-POLE 12-SLOT
#=4 fixed by hardware.
Max. RPM: 	80,000
MapX =RPM  = Frequency
Map0 :   0 = 0
Map1 :1800 = 60 
Map2 :3600 = 120
Map3 :5400 = 180
Map4 :7200 = 240 
Map5 :9000 = 300
Map6 :10800= 360
Map7 :12600= 420
Map8 :14400= 480
Map9 :16200= 540
Map10:18000= 600
Map11:19800= 660
Map12:21600= 720
Map13:23400= 780 
Map14:25200= 840
Map15:27000= 900 
Map16:28800= 960
Map17:30600= 1020
Map18:32400= 1080
Map19:34200= 1140
Map20:36000= 1200
Map21:37800= 1260 
Map21:39600= 1320 
Map22:41400= 1380 
Map23:43200= 1440 
Map24:45000= 1500
Map25:46800= 1560 
Map26:48600= 1620 
Map27:50400= 1680 
Map28:52200= 1740 
Map29:54000= 1800 
Map30:55800= 1860
Map31:57600= 1920 
Map32:59400= 1980
Map33:61200= 2040
Map34:63000= 2100
Map35:64800= 2160 
Map36:66600= 2220 
Map37:68400= 2280
Map38:70200= 2340 
Map39:72000= 2400 
Map40:73800= 2460 
Map41:75600= 2520 
Map42:77400= 2580 
Map43:79200= 2640 
Map44:81000= 2700 

https://en.wikipedia.org/wiki/List_of_Arduino_boards_and_compatible_systems
https://en.wikipedia.org/wiki/Hertz
https://en.wikipedia.org/wiki/Pulse-width_modulation
https://en.wikipedia.org/wiki/Insulated-gate_bipolar_transistor

The hertz (symbol: Hz) is the unit of frequency in the International System 
of Units (SI), often described as being equivalent to one event (or cycle) per second.

1000000 micro s = 1 s
6 steps
StepTime=(S*Ms)\(PhaseStepCount*Hz)
16666=(1000000)\(6*10)=10Hz
1666 =(1000000)\(6*100)=100Hz
166  =(1000000)\(6*1000)=1000Hz
16   =(1000000)\(6*10000)=10000Hz
1.6  =(1000000)\(6*100000)=100000Hz

ThrottleValueMap=0 = 0Hz    =(1000000)\(6*0)    = 0
ThrottleValueMap=1 = 60Hz   =(1000000)\(6*60)   = 2777
ThrottleValueMap=2 = 180Hz  =(1000000)\(6*180)  = 925
ThrottleValueMap=3 = 300Hz  =(1000000)\(6*300)  = 555
ThrottleValueMap=4 = 420Hz  =(1000000)\(6*420)  = 396
ThrottleValueMap=5 = 540Hz  =(1000000)\(6*540)  = 308
ThrottleValueMap=6 = 660Hz  =(1000000)\(6*780)  = 213
ThrottleValueMap=8 = 9000Hz  =(1000000)\(6*660)  = 252
ThrottleValueMap=7 = 78Hz  =(1000000)\(6*900)  = 185
ThrottleValueMap=9 = 1020Hz =(1000000)\(6*1020) = 163
ThrottleValueMap=10= 1380Hz =(1000000)\(6*1380) = 120

*/
void CheckThrottle()
{
 ThrottleValue=analogRead(ThrottlePin);
  if (ThrottleValue<20)
  {
   ThrottleValueMap=0;
   PhaseTimer=0;
   AllOff();
  }
 
  if ((ThrottleValue>10)&&(ThrottleValue<20))
  {
   ThrottleValueMap=1;
   PhaseTimer=1000000;
  }
 
  if ((ThrottleValue>20)&&(ThrottleValue<30))
  {
   ThrottleValueMap=2;
   PhaseTimer=250000;
  }
  
  if ((ThrottleValue>30)&&(ThrottleValue<40))
  {
   ThrottleValueMap=3;
   PhaseTimer=500000;
  }
  
  if ((ThrottleValue>40)&&(ThrottleValue<50))
  {
   ThrottleValueMap=4;
   PhaseTimer=100000;
  }
  
  if ((ThrottleValue>50)&&(ThrottleValue<100))
  {
   ThrottleValueMap=5;
   PhaseTimer=10000;
  }
  
  if ((ThrottleValue>100)&&(ThrottleValue<200))
  {
   ThrottleValueMap=6;
   PhaseTimer=1000;
   }
  
  if ((ThrottleValue>200)&&(ThrottleValue<300))
  {
   ThrottleValueMap=7;
   PhaseTimer=100;
  }
  
  if ((ThrottleValue>300)&&(ThrottleValue<400))
  {
   ThrottleValueMap=8;
   PhaseTimer=10;
  }
  
  if ((ThrottleValue>400)&&(ThrottleValue<500))
  {
   ThrottleValueMap=9;
   PhaseTimer=1;
  } 
  
  if (ThrottleValue>500)
  {
   ThrottleValueMap=10;
   // PhaseTimer=120;
   PhaseTimer= 12;
  }
}

void CheckMotor()
{
 Hall1Val=analogRead(Hall1Pin); 
 Hall2Val=analogRead(Hall2Pin); 
 Hall3Val=analogRead(Hall3Pin); 
 MotorTempVal=analogRead(MotorTempPin);
}

void DefinePins()
{
 pinMode(ThrottlePin,INPUT);
 pinMode(Hall1Pin,INPUT);
 pinMode(Hall2Pin,INPUT);
 pinMode(Hall3Pin,INPUT);
 pinMode(MotorTempPin,INPUT);
 pinMode(Igbt1Pin,OUTPUT);
 analogWrite(Igbt1Pin,IgbtOff);
 pinMode(Igbt2Pin,OUTPUT);
 analogWrite(Igbt2Pin,IgbtOff);
 pinMode(Igbt3Pin,OUTPUT);
 analogWrite(Igbt3Pin,IgbtOff);
 pinMode(Igbt4Pin,OUTPUT);
 analogWrite(Igbt4Pin,IgbtOff);
 pinMode(Igbt5Pin,OUTPUT);
 analogWrite(Igbt5Pin,IgbtOff);
 pinMode(Igbt6Pin,OUTPUT);
 analogWrite(Igbt6Pin,IgbtOff);
}

void EnterPhase1()
{
 CurrentPhase=1;
 ScreenGateData=16;
 PwmVal=64;
 analogWrite(Igbt5Pin,IgbtOff);
 Igbt5Val=IgbtOff;
 analogWrite(Igbt6Pin,IgbtOff);
 Igbt6Val=IgbtOff;
 analogWrite(Igbt1Pin,PwmVal);
 Igbt1Val=PwmVal;
 analogWrite(Igbt6Pin,PwmVal);
 Igbt6Val=PwmVal;
 PhaseDelay();
 }

void EnterPhase2()
{
 CurrentPhase=2;
 ScreenGateData=12;
 PwmVal=128;
 analogWrite(Igbt6Pin,IgbtOff);
 Igbt6Val=IgbtOff;
 analogWrite(Igbt2Pin,PwmVal);
 Igbt2Val=PwmVal;
 PhaseDelay();
 }

void EnterPhase3()
{
 CurrentPhase=3;
 ScreenGateData=23;
 PwmVal=64;
 analogWrite(Igbt1Pin,IgbtOff);
 Igbt1Val=IgbtOff;
 analogWrite(Igbt3Pin,PwmVal);
 Igbt3Val=PwmVal;
 PhaseDelay();
}


void EnterPhase4()
{
 CurrentPhase=4;
 ScreenGateData=34;
 PwmVal=64;
 analogWrite(Igbt2Pin,IgbtOff);
 Igbt2Val=IgbtOff;
 analogWrite(Igbt4Pin,PwmVal);
 Igbt4Val=PwmVal;
 PhaseDelay();
}

void EnterPhase5()
{
 CurrentPhase=5;
 ScreenGateData=45;
 PwmVal=128;
 analogWrite(Igbt3Pin,IgbtOff);
 Igbt3Val=IgbtOff;
 analogWrite(Igbt5Pin,PwmVal);
 Igbt5Val=PwmVal;
 PhaseDelay();
 }

void EnterPhase6()
{
 CurrentPhase=6;
 ScreenGateData=56;
 PwmVal=64;
 analogWrite(Igbt4Pin,IgbtOff);
 Igbt4Val=IgbtOff;
 analogWrite(Igbt6Pin,PwmVal);
 Igbt6Val=PwmVal;
 PhaseDelay();
 }

void AllOff()
{
 CurrentPhase=0;
 ScreenGateData=0;
 PwmVal=0;
 analogWrite(Igbt1Pin,IgbtOff);
 Igbt1Val=IgbtOff;
 analogWrite(Igbt2Pin,IgbtOff);
 Igbt2Val=IgbtOff;
 analogWrite(Igbt3Pin,IgbtOff);
 Igbt3Val=IgbtOff,
 analogWrite(Igbt4Pin,IgbtOff);
 Igbt4Val=IgbtOff;
 analogWrite(Igbt5Pin,IgbtOff);
 Igbt5Val=IgbtOff;
 analogWrite(Igbt6Pin,IgbtOff);
 Igbt6Val=IgbtOff;
 PhaseDelay();
}