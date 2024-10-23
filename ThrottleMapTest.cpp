#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>

// LCD globals
#define BLACK   0x0000
#define RED     0xF800
uint16_t ScreenId;
int Font3LineHight=30;
int RecDataCol=300;
int LoopCount=0;
MCUFRIEND_kbv MainScreen;

// User Inputs
int ThrottleValue=0;
int ThrottleValueMap=0;
int ThrottlePin=A7;

// Phase Time Internals
unsigned long PhaseTimer=33;
unsigned long PhaseTimerStart=0;
unsigned long PhaseTimerStop=0;
unsigned long PhaseMicros=0;
unsigned long PhaseTimerPause=0;

// Phase step Time Internals
unsigned long PhaseStepTimer=4;
unsigned long PhaseStepTimerStart=0;
unsigned long PhaseStepTimerStop=0;
unsigned long PhaseStepMicros=0;
unsigned long PhaseStepTimerPause=0;

void UserPause();
void SetupLcd();
void DrawSplashScreen();
void UpdateDisplay();
void DrawStaticFields();
void CheckThrottle();
void CalcMapValues();
void PhaseStepTimeTest();
void PhaseTimeTest();

void setup() 
{
 SetupLcd();
 DrawSplashScreen();
 DrawStaticFields();
}

void loop()
{
 CheckThrottle();
 CalcMapValues();
 PhaseStepTimeTest();
 PhaseTimeTest();
 UpdateDisplay();
 UserPause();
}

void SetupLcd()
{
 ScreenId = MainScreen.readID();
 MainScreen.begin(ScreenId);
 MainScreen.setRotation(1);
}

void DrawSplashScreen()
{
 MainScreen.fillScreen(BLACK);
 MainScreen.setCursor(0,0);
 MainScreen.setTextSize(5);
 MainScreen.setTextColor(RED);
 MainScreen.print("Throttle Monitor");
 MainScreen.setCursor(0,100);
 MainScreen.print("Get that data.");
 MainScreen.setCursor(0,200);
 MainScreen.print("Keep It Under:");
 MainScreen.setCursor(125,250);
 MainScreen.print("100!");
 delay(2000);
 MainScreen.fillScreen(BLACK);
}

void DrawStaticFields()
{
 MainScreen.setTextSize(3);
 MainScreen.setCursor(0,Font3LineHight*0);
 MainScreen.print("Throttle Monitor");
 MainScreen.setCursor(0,Font3LineHight*1);
 MainScreen.print("Pot Raw Val:");
 MainScreen.setCursor(0,Font3LineHight*2);
 MainScreen.print("Pot Map Val:");
 MainScreen.setCursor(0,Font3LineHight*3);
 MainScreen.print("Phase Start Val:");
 MainScreen.setCursor(0,Font3LineHight*4);
 MainScreen.print("Phase Stop Val:");
 MainScreen.setCursor(0,Font3LineHight*5);
 MainScreen.print("Phase Pause Val:");
 MainScreen.setCursor(0,Font3LineHight*6);
 MainScreen.print("Step Start Val:");
 MainScreen.setCursor(0,Font3LineHight*7);
 MainScreen.print("Step Stop Val:");
 MainScreen.setCursor(0,Font3LineHight*8);
 MainScreen.print("Step Pause Val:");
 MainScreen.setCursor(0,Font3LineHight*9);
 MainScreen.print("LoopCount Val:");
}

void CheckThrottle()
{
 ThrottleValue=analogRead(ThrottlePin);
}

void CalcMapValues()
{
 ThrottleValueMap=map(constrain(ThrottleValue,0,1000),0,1000,0,10);
}

void UpdateDisplay()
{
 MainScreen.fillRect(RecDataCol,Font3LineHight*1,300,Font3LineHight*9,BLACK);
 MainScreen.setCursor(RecDataCol,Font3LineHight*1);
 MainScreen.print(ThrottleValue);
 MainScreen.setCursor(RecDataCol,Font3LineHight*2);
 MainScreen.print(ThrottleValueMap);
 MainScreen.setCursor(RecDataCol,Font3LineHight*3);
 MainScreen.print(PhaseTimerStart);
 MainScreen.setCursor(RecDataCol,Font3LineHight*4);
 MainScreen.print(PhaseTimerStop); 
 MainScreen.setCursor(RecDataCol,Font3LineHight*5);
 MainScreen.print(PhaseTimerPause);
 MainScreen.setCursor(RecDataCol,Font3LineHight*6);
 MainScreen.print(PhaseStepTimerStart);
 MainScreen.setCursor(RecDataCol,Font3LineHight*7);
 MainScreen.print(PhaseStepTimerStop); 
 MainScreen.setCursor(RecDataCol,Font3LineHight*8);
 MainScreen.print(PhaseStepTimerPause);
 LoopCount++;
 MainScreen.setCursor(RecDataCol,Font3LineHight*9);
 MainScreen.print(LoopCount); 
}

void PhaseTimeTest()
{
  PhaseTimerStart=micros();
  PhaseMicros=PhaseTimerStart;
  while((PhaseMicros-PhaseTimerStart)<PhaseTimer)
      {
         PhaseMicros=micros();
      }
  PhaseTimerStop=micros();
  PhaseTimerPause=PhaseTimerStop-PhaseTimerStart;
}

void PhaseStepTimeTest()
{
  PhaseStepTimerStart=micros();
  PhaseStepMicros=PhaseStepTimerStart;
  while((PhaseStepMicros-PhaseStepTimerStart)<PhaseStepTimer)
      {
         PhaseStepMicros=micros();
      }
  PhaseStepTimerStop=micros();
  PhaseStepTimerPause=PhaseStepTimerStop-PhaseStepTimerStart;
}

void UserPause()
{
  delay(2000);
}