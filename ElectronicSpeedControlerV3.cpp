#include <Wire.h>
#include <Arduino.h>

//Inputs
int ThrottleValue=0;
int ThrottleValueMap=0;
int ThrottlePin=A1;
int ReadyToStart=0;

//OutPuts
int Igbt1Pin = 2;
int Igbt2Pin = 3;
int Igbt3Pin = 4;
int Igbt4Pin = 5;
int Igbt5Pin = 6;
int Igbt6Pin = 7;

// Phase step Time Internals
unsigned long PhaseStepTimer=0;
unsigned long PhaseStepTimerStart=0;
unsigned long PhaseStepTimerStop=0;
unsigned long PhaseStepMicros=0;
unsigned long PhaseStepTimerPause=0;

//Functions
void DefinePins();
void CheckThrottle();
// Phase Map 1
void EnterPhase1m1();
void EnterPhase2m1();
void EnterPhase3m1();
void EnterPhase4m1();
void EnterPhase5m1();
void EnterPhase6m1();
// Phase Map 2
void EnterPhase1m2();
void EnterPhase2m2();
void EnterPhase3m2();
void EnterPhase4m2();
void EnterPhase5m2();
void EnterPhase6m2();
// Phase Map 3
void EnterPhase1m3();
void EnterPhase2m3();
void EnterPhase3m3();
void EnterPhase4m3();
void EnterPhase5m3();
void EnterPhase6m3();
// Phase Map 4
void EnterPhase1m4();
void EnterPhase2m4();
void EnterPhase3m4();
void EnterPhase4m4();
void EnterPhase5m4();
void EnterPhase6m4();
//Phase Map 5
void EnterPhase1m5();
void EnterPhase2m5();
void EnterPhase3m5();
void EnterPhase4m5();
void EnterPhase5m5();
void EnterPhase6m5();
// Phase Map 6
void EnterPhase1m6();
void EnterPhase2m6();
void EnterPhase3m6();
void EnterPhase4m6();
void EnterPhase5m6();
void EnterPhase6m6();
// Phase Map 7
void EnterPhase1m7();
void EnterPhase2m7();
void EnterPhase3m7();
void EnterPhase4m7();
void EnterPhase5m7();
void EnterPhase6m7();
// Phase Map 8
void EnterPhase1m8();
void EnterPhase2m8();
void EnterPhase3m8();
void EnterPhase4m8();
void EnterPhase5m8();
void EnterPhase6m8();
// Phase Map 9
void EnterPhase1m9();
void EnterPhase2m9();
void EnterPhase3m9();
void EnterPhase4m9();
void EnterPhase5m9();
void EnterPhase6m9();
// Phase Map 10
void EnterPhase1m10();
void EnterPhase2m10();
void EnterPhase3m10();
void EnterPhase4m10();
void EnterPhase5m10();
void EnterPhase6m10();
//
void AllOff();
void PhaseStepTime();

void setup()
{
 DefinePins();
 AllOff();
 while (ReadyToStart==0)
 {
  CheckThrottle();
  if(ThrottleValueMap==0)
  {
    ReadyToStart=1;
  }
 }
}

void loop() 
{
 CheckThrottle();
 if(ThrottleValueMap==0)
  {
   AllOff();
   PhaseStepTime();
  }
 else if(ThrottleValueMap==1)
 {
   EnterPhase1m1();
   EnterPhase2m1();
   EnterPhase3m1();
   EnterPhase4m1();
   EnterPhase5m1();
   EnterPhase6m1();
  }
 else if(ThrottleValueMap==2)
 {
   EnterPhase1m2();
   EnterPhase2m2();
   EnterPhase3m2();
   EnterPhase4m2();
   EnterPhase5m2();
   EnterPhase6m2();
  }
 else if(ThrottleValueMap==3)
 {
   EnterPhase1m3();
   EnterPhase2m3();
   EnterPhase3m3();
   EnterPhase4m3();
   EnterPhase5m3();
   EnterPhase6m3();
  }
 else if(ThrottleValueMap==4)
 {
   EnterPhase1m4();
   EnterPhase2m4();
   EnterPhase3m4();
   EnterPhase4m4();
   EnterPhase5m4();
   EnterPhase6m4();
  }
  else if(ThrottleValueMap==5)
 {
   EnterPhase1m5();
   EnterPhase2m5();
   EnterPhase3m5();
   EnterPhase4m5();
   EnterPhase5m5();
   EnterPhase6m5();
  }
 else if(ThrottleValueMap==6)
 {
   EnterPhase1m6();
   EnterPhase2m6();
   EnterPhase3m6();
   EnterPhase4m6();
   EnterPhase5m6();
   EnterPhase6m6();
  }
 else if(ThrottleValueMap==7)
 {
   EnterPhase1m7();
   EnterPhase2m7();
   EnterPhase3m7();
   EnterPhase4m7();
   EnterPhase5m7();
   EnterPhase6m7();
  }
 else if(ThrottleValueMap==8)
 {
   EnterPhase1m8();
   EnterPhase2m8();
   EnterPhase3m8();
   EnterPhase4m8();
   EnterPhase5m8();
   EnterPhase6m8();
  }
 else if(ThrottleValueMap==9)
 {
   EnterPhase1m9();
   EnterPhase2m9();
   EnterPhase3m9();
   EnterPhase4m9();
   EnterPhase5m9();
   EnterPhase6m9();
  }
 else if(ThrottleValueMap==10)
 {
   EnterPhase1m10();
   EnterPhase2m10();
   EnterPhase3m10();
   EnterPhase4m10();
   EnterPhase5m10();
   EnterPhase6m10();
  }
}

void PhaseStepTime()
{
  PhaseStepTimerStart=micros();
  PhaseStepMicros=PhaseStepTimerStart;
  while((PhaseStepMicros-PhaseStepTimerStart)<PhaseStepTimer)
      {
         PhaseStepMicros=micros();
      }
}

void CheckThrottle()
{
 ThrottleValue=analogRead(ThrottlePin);
 ThrottleValueMap=map(constrain(ThrottleValue,0,1000),0,1000,0,10);
if(ThrottleValueMap==10)
{
 PhaseStepTimer=4;
}
else
{
 PhaseStepTimer=33;
}
}

void AllOff()
{
 analogWrite(2,0);
 analogWrite(3,0);
 analogWrite(4,0);
 analogWrite(5,0);
 analogWrite(7,0);
 analogWrite(7,0);
 PhaseStepTime();
}

void EnterPhase1m1()
{
//{0,12,23},4KHz
 analogWrite(7,0);
 analogWrite(2,0);
 analogWrite(7,0);
 PhaseStepTime();
 analogWrite(2,12);
 analogWrite(7,12);
 PhaseStepTime();
 analogWrite(2,23);
 analogWrite(7,23);
 PhaseStepTime();
 }

 void EnterPhase1m2()
{
//{0,23,46},4KHz
 analogWrite(7,0);
 analogWrite(2,0);
 analogWrite(7,0);
 PhaseStepTime();
 analogWrite(2,23);
 analogWrite(7,23);
 PhaseStepTime();
 analogWrite(2,46);
 analogWrite(7,46);
 PhaseStepTime();
 }

 void EnterPhase1m3()
{
//{0,35,70},4KHz
 analogWrite(7,0);
 analogWrite(2,0);
 analogWrite(7,0);
 PhaseStepTime();
 analogWrite(2,35);
 analogWrite(7,35);
 PhaseStepTime();
 analogWrite(2,70);
 analogWrite(7,70);
 PhaseStepTime();
 }

 void EnterPhase1m4()
{
  //{0,46,93},4KHz
 analogWrite(7,0);
 analogWrite(2,0);
 analogWrite(7,0);
 PhaseStepTime();
 analogWrite(2,46);
 analogWrite(7,46);
 PhaseStepTime();
 analogWrite(2,93);
 analogWrite(7,93);
 PhaseStepTime();
 }

 void EnterPhase1m5()
{
  //{0,58,117},4KHz
 analogWrite(7,0);
 analogWrite(2,0);
 analogWrite(7,0);
 PhaseStepTime();
 analogWrite(2,58);
 analogWrite(7,58);
 PhaseStepTime();
 analogWrite(2,117);
 analogWrite(7,117);
 PhaseStepTime();
 }

 void EnterPhase1m6()
{
  //{0,70,140},4KHz
 analogWrite(7,0);
 analogWrite(2,0);
 analogWrite(7,0);
 PhaseStepTime();
 analogWrite(2,70);
 analogWrite(7,70);
 PhaseStepTime();
 analogWrite(2,140);
 analogWrite(7,140);
 PhaseStepTime();
 }

 void EnterPhase1m7()
{
  //{0,81,163},4KHz
 analogWrite(7,0);
 analogWrite(2,0);
 analogWrite(7,0);
 PhaseStepTime();
 analogWrite(2,81);
 analogWrite(7,81);
 PhaseStepTime();
 analogWrite(2,163);
 analogWrite(7,163);
 PhaseStepTime();
 }

 void EnterPhase1m8()
{
  //{0,93,187},4KHz
 analogWrite(7,0);
 analogWrite(2,0);
 analogWrite(7,0);
 PhaseStepTime();
 analogWrite(2,93);
 analogWrite(7,93);
 PhaseStepTime();
 analogWrite(2,187);
 analogWrite(7,187);
 PhaseStepTime();
 }

 void EnterPhase1m9()
{
  //{0,105,210},4KHz
 analogWrite(7,0);
 analogWrite(2,0);
 analogWrite(7,0);
 PhaseStepTime();
 analogWrite(2,105);
 analogWrite(7,105);
 PhaseStepTime();
 analogWrite(2,210);
 analogWrite(7,210);
 PhaseStepTime();
 }

 void EnterPhase1m10()
{
// {0,115,230},8KHZ
 analogWrite(7,0);
 analogWrite(2,0);
 analogWrite(7,0);
 PhaseStepTime();
 analogWrite(2,115);
 analogWrite(7,115);
 PhaseStepTime();
 analogWrite(2,230);
 analogWrite(7,230);
 PhaseStepTime();
 }

void EnterPhase2m1()
{
  //{23,26,23},4KHz
 analogWrite(7,0);
 analogWrite(2,23);
 analogWrite(3,23);
 PhaseStepTime();
 analogWrite(2,26);
 analogWrite(3,26);
 PhaseStepTime();
 analogWrite(2,23);
 analogWrite(3,23);
 PhaseStepTime();
 }

 
void EnterPhase2m2()
{
  //{46,52,46},4KHz
 analogWrite(7,0);
 analogWrite(2,46);
 analogWrite(3,46);
 PhaseStepTime();
 analogWrite(2,52);
 analogWrite(3,52);
 PhaseStepTime();
 analogWrite(2,46);
 analogWrite(3,46);
 PhaseStepTime();
 }

 
void EnterPhase2m3()
{
  //{70,78,70},4KHz
 analogWrite(7,0);
 analogWrite(2,70);
 analogWrite(3,70);
 PhaseStepTime();
 analogWrite(2,78);
 analogWrite(3,78);
 PhaseStepTime();
 analogWrite(2,70);
 analogWrite(3,70);
 PhaseStepTime();
 }

 
void EnterPhase2m4()
{
 //{93,104,93},4KHz
 analogWrite(7,0);
 analogWrite(2,93);
 analogWrite(3,93);
 PhaseStepTime();
 analogWrite(2,104);
 analogWrite(3,104);
 PhaseStepTime();
 analogWrite(2,93);
 analogWrite(3,93);
 PhaseStepTime();
 }

 
void EnterPhase2m5()
{
  //{117,130,117},4KHz
 analogWrite(7,0);
 analogWrite(2,117);
 analogWrite(3,117);
 PhaseStepTime();
 analogWrite(2,130);
 analogWrite(3,130);
 PhaseStepTime();
 analogWrite(2,117);
 analogWrite(3,117);
 PhaseStepTime();
 }

 
void EnterPhase2m6()
{
  //{140,156,140},4KHz
 analogWrite(7,0);
 analogWrite(2,140);
 analogWrite(3,140);
 PhaseStepTime();
 analogWrite(2,156);
 analogWrite(3,156);
 PhaseStepTime();
 analogWrite(2,140);
 analogWrite(3,140);
 PhaseStepTime();
 }

 
void EnterPhase2m7()
{
  //{163,182,163},4KHz
 analogWrite(7,0);
 analogWrite(2,163);
 analogWrite(3,163);
 PhaseStepTime();
 analogWrite(2,182);
 analogWrite(3,182);
 PhaseStepTime();
 analogWrite(2,163);
 analogWrite(3,163);
 PhaseStepTime();
 }

 
void EnterPhase2m8()
{
  //{187,208,187},4KHz
 analogWrite(7,0);
 analogWrite(2,187);
 analogWrite(3,187);
 PhaseStepTime();
 analogWrite(2,208);
 analogWrite(3,208);
 PhaseStepTime();
 analogWrite(2,187);
 analogWrite(3,187);
 PhaseStepTime();
 }

 
void EnterPhase2m9()
{
 //{210,234,210},4KHz
 analogWrite(7,0);
 analogWrite(2,210);
 analogWrite(3,210);
 PhaseStepTime();
 analogWrite(2,234);
 analogWrite(3,234);
 PhaseStepTime();
 analogWrite(2,210);
 analogWrite(3,210);
 PhaseStepTime();
 }

 
void EnterPhase2m10()
{
  //{230,256,230},8KHZ
 analogWrite(7,0);
 analogWrite(2,230);
 analogWrite(3,230);
 PhaseStepTime();
 analogWrite(2,256);
 analogWrite(3,256);
 PhaseStepTime();
 analogWrite(2,230);
 analogWrite(3,230);
 PhaseStepTime();
 }

void EnterPhase3m1()
{
  //{23,12,0},4KHz
 analogWrite(2,0);
 analogWrite(3,23);
 analogWrite(4,23);
 PhaseStepTime();
 analogWrite(3,12);
 analogWrite(4,12);
 PhaseStepTime();
 analogWrite(3,0);
 analogWrite(4,0);
 PhaseStepTime();
}

void EnterPhase3m2()
{
  //{46,23,0},4KHz
 analogWrite(2,0);
 analogWrite(3,46);
 analogWrite(4,46);
 PhaseStepTime();
 analogWrite(3,23);
 analogWrite(4,23);
 PhaseStepTime();
 analogWrite(3,0);
 analogWrite(4,0);
 PhaseStepTime();
}

void EnterPhase3m3()
{
  // {70,35,0},4KHz
 analogWrite(2,0);
 analogWrite(3,70);
 analogWrite(4,70);
 PhaseStepTime();
 analogWrite(3,35);
 analogWrite(4,35);
 PhaseStepTime();
 analogWrite(3,0);
 analogWrite(4,0);
 PhaseStepTime();
}

void EnterPhase3m4()
{
  //{93,46,0},4KHz
 analogWrite(2,0);
 analogWrite(3,93);
 analogWrite(4,93);
 PhaseStepTime();
 analogWrite(3,46);
 analogWrite(4,46);
 PhaseStepTime();
 analogWrite(3,0);
 analogWrite(4,0);
 PhaseStepTime();
}

void EnterPhase3m5()
{
  //{117,58,0},4KHz
 analogWrite(2,0);
 analogWrite(3,117);
 analogWrite(4,117);
 PhaseStepTime();
 analogWrite(3,58);
 analogWrite(4,58);
 PhaseStepTime();
 analogWrite(3,0);
 analogWrite(4,0);
 PhaseStepTime();
}

void EnterPhase3m6()
{
  //{140,70,0},4KHz
 analogWrite(2,0);
 analogWrite(3,140);
 analogWrite(4,140);
 PhaseStepTime();
 analogWrite(3,70);
 analogWrite(4,70);
 PhaseStepTime();
 analogWrite(3,0);
 analogWrite(4,0);
 PhaseStepTime();
}

void EnterPhase3m7()
{
  //{163,81,0},4KHz
 analogWrite(2,0);
 analogWrite(3,163);
 analogWrite(4,163);
 PhaseStepTime();
 analogWrite(3,81);
 analogWrite(4,81);
 PhaseStepTime();
 analogWrite(3,0);
 analogWrite(4,0);
 PhaseStepTime();
}

void EnterPhase3m8()
{
  //{187,93,0},4KHz
 analogWrite(2,0);
 analogWrite(3,187);
 analogWrite(4,187);
 PhaseStepTime();
 analogWrite(3,93);
 analogWrite(4,93);
 PhaseStepTime();
 analogWrite(3,0);
 analogWrite(4,0);
 PhaseStepTime();
}

void EnterPhase3m9()
{
  //{210,105,0},4KHz
 analogWrite(2,0);
 analogWrite(3,210);
 analogWrite(4,210);
 PhaseStepTime();
 analogWrite(3,105);
 analogWrite(4,105);
 PhaseStepTime();
 analogWrite(3,0);
 analogWrite(4,0);
 PhaseStepTime();
}

void EnterPhase3m10()
{
  //{230,115,0},8KHZ
 analogWrite(2,0);
 analogWrite(3,230);
 analogWrite(4,230);
 PhaseStepTime();
 analogWrite(3,115);
 analogWrite(4,115);
 PhaseStepTime();
 analogWrite(3,0);
 analogWrite(4,0);
 PhaseStepTime();
}


void EnterPhase4m1()
{
  //{0,12,23},4KHz
 analogWrite(3,0);
 analogWrite(4,0);
 analogWrite(5,0);
 PhaseStepTime();
 analogWrite(4,12);
 analogWrite(5,12);
 PhaseStepTime();
 analogWrite(4,23);
 analogWrite(5,23);
 PhaseStepTime();
}

void EnterPhase4m2()
{
  //{0,23,46},4KHz
 analogWrite(3,0);
 analogWrite(4,0);
 analogWrite(5,0);
 PhaseStepTime();
 analogWrite(4,23);
 analogWrite(5,23);
 PhaseStepTime();
 analogWrite(4,46);
 analogWrite(5,46);
 PhaseStepTime();
}

void EnterPhase4m3()
{
  //{0,35,70},4KHz
 analogWrite(3,0);
 analogWrite(4,0);
 analogWrite(5,0);
 PhaseStepTime();
 analogWrite(4,35);
 analogWrite(5,35);
 PhaseStepTime();
 analogWrite(4,70);
 analogWrite(5,70);
 PhaseStepTime();
}

void EnterPhase4m4()
{
  //{0,46,93},4KHz
 analogWrite(3,0);
 analogWrite(4,0);
 analogWrite(5,0);
 PhaseStepTime();
 analogWrite(4,46);
 analogWrite(5,46);
 PhaseStepTime();
 analogWrite(4,93);
 analogWrite(5,93);
 PhaseStepTime();
}

void EnterPhase4m5()
{
  //{0,58,117},4KHz
 analogWrite(3,0);
 analogWrite(4,0);
 analogWrite(5,0);
 PhaseStepTime();
 analogWrite(4,58);
 analogWrite(5,58);
 PhaseStepTime();
 analogWrite(4,117);
 analogWrite(5,117);
 PhaseStepTime();
}

void EnterPhase4m6()
{
  //{0,70,140},4KHz
 analogWrite(3,0);
 analogWrite(4,0);
 analogWrite(5,0);
 PhaseStepTime();
 analogWrite(4,70);
 analogWrite(5,70);
 PhaseStepTime();
 analogWrite(4,140);
 analogWrite(5,140);
 PhaseStepTime();
}

void EnterPhase4m7()
{
  //{0,81,163},4KHz
 analogWrite(3,0);
 analogWrite(4,0);
 analogWrite(5,0);
 PhaseStepTime();
 analogWrite(4,81);
 analogWrite(5,81);
 PhaseStepTime();
 analogWrite(4,163);
 analogWrite(5,163);
 PhaseStepTime();
}

void EnterPhase4m8()
{
  //{0,93,187},4KHz
 analogWrite(3,0);
 analogWrite(4,0);
 analogWrite(5,0);
 PhaseStepTime();
 analogWrite(4,93);
 analogWrite(5,93);
 PhaseStepTime();
 analogWrite(4,187);
 analogWrite(5,187);
 PhaseStepTime();
}

void EnterPhase4m9()
{
  //{0,105,210},4KHz
 analogWrite(3,0);
 analogWrite(4,0);
 analogWrite(5,0);
 PhaseStepTime();
 analogWrite(4,105);
 analogWrite(5,105);
 PhaseStepTime();
 analogWrite(4,210);
 analogWrite(5,210);
 PhaseStepTime();
}

void EnterPhase4m10()
{
  //{0,115,230},8KHZ
 analogWrite(3,0);
 analogWrite(4,0);
 analogWrite(5,0);
 PhaseStepTime();
 analogWrite(4,115);
 analogWrite(5,115);
 PhaseStepTime();
 analogWrite(4,230);
 analogWrite(5,230);
 PhaseStepTime();
}

void EnterPhase5m1()
{
  //{23,26,23},4KHz
 analogWrite(4,0);
 analogWrite(5,23);
 analogWrite(7,23);
 PhaseStepTime();
 analogWrite(5,26);
 analogWrite(7,26);
 PhaseStepTime();
 analogWrite(5,23);
 analogWrite(7,23);
 PhaseStepTime();
}

void EnterPhase5m2()
{
  //{46,52,46},4KHz
 analogWrite(4,0);
 analogWrite(5,46);
 analogWrite(7,46);
 PhaseStepTime();
 analogWrite(5,52);
 analogWrite(7,52);
 PhaseStepTime();
 analogWrite(5,46);
 analogWrite(7,46);
 PhaseStepTime();
}

void EnterPhase5m3()
{
  //{70,78,70},4KHz
 analogWrite(4,0);
 analogWrite(5,70);
 analogWrite(7,70);
 PhaseStepTime();
 analogWrite(5,78);
 analogWrite(7,78);
 PhaseStepTime();
 analogWrite(5,70);
 analogWrite(7,70);
 PhaseStepTime();
}

void EnterPhase5m4()
{
  //{93,104,93},4KHz
 analogWrite(4,0);
 analogWrite(5,93);
 analogWrite(7,93);
 PhaseStepTime();
 analogWrite(5,104);
 analogWrite(7,104);
 PhaseStepTime();
 analogWrite(5,93);
 analogWrite(7,93);
 PhaseStepTime();
}

void EnterPhase5m5()
{
  //{117,130,117},4KHz
 analogWrite(4,0);
 analogWrite(5,117);
 analogWrite(7,117);
 PhaseStepTime();
 analogWrite(5,130);
 analogWrite(7,130);
 PhaseStepTime();
 analogWrite(5,117);
 analogWrite(7,117);
 PhaseStepTime();
}

void EnterPhase5m6()
{
  //{140,156,140},4KHz
 analogWrite(4,0);
 analogWrite(5,140);
 analogWrite(7,140);
 PhaseStepTime();
 analogWrite(5,156);
 analogWrite(7,156);
 PhaseStepTime();
 analogWrite(5,140);
 analogWrite(7,140);
 PhaseStepTime();
}

void EnterPhase5m7()
{
  //{163,182,163},4KHz
 analogWrite(4,0);
 analogWrite(5,163);
 analogWrite(7,163);
 PhaseStepTime();
 analogWrite(5,182);
 analogWrite(7,182);
 PhaseStepTime();
 analogWrite(5,163);
 analogWrite(7,163);
 PhaseStepTime();
}

void EnterPhase5m8()
{
  //{187,208,187},4KHz
 analogWrite(4,0);
 analogWrite(5,187);
 analogWrite(7,187);
 PhaseStepTime();
 analogWrite(5,208);
 analogWrite(7,208);
 PhaseStepTime();
 analogWrite(5,187);
 analogWrite(7,187);
 PhaseStepTime();
}

void EnterPhase5m9()
{
  //{210,234,210},4KHz
 analogWrite(4,0);
 analogWrite(5,210);
 analogWrite(7,210);
 PhaseStepTime();
 analogWrite(5,234);
 analogWrite(7,234);
 PhaseStepTime();
 analogWrite(5,210);
 analogWrite(7,210);
 PhaseStepTime();
}

void EnterPhase5m10()
{
  //{230,256,230},8KHZ
 analogWrite(4,0);
 analogWrite(5,230);
 analogWrite(7,230);
 PhaseStepTime();
 analogWrite(5,256);
 analogWrite(7,256);
 PhaseStepTime();
 analogWrite(5,230);
 analogWrite(7,230);
 PhaseStepTime();
}

void EnterPhase6m1()
{
  //{23,12,0},4KHz
 analogWrite(5,0);
 analogWrite(7,23);
 analogWrite(7,23);
 PhaseStepTime();
 analogWrite(7,12);
 analogWrite(7,12);
 PhaseStepTime();
 analogWrite(7,0);
 analogWrite(7,0);
 PhaseStepTime();
 }

 void EnterPhase6m2()
{
  //{46,23,0},4KHz
 analogWrite(5,0);
 analogWrite(7,46);
 analogWrite(7,46);
 PhaseStepTime();
 analogWrite(7,23);
 analogWrite(7,23);
 PhaseStepTime();
 analogWrite(7,0);
 analogWrite(7,0);
 PhaseStepTime();
 }

 void EnterPhase6m3()
{
  //{70,35,0},4KHz
 analogWrite(5,0);
 analogWrite(7,70);
 analogWrite(7,70);
 PhaseStepTime();
 analogWrite(7,35);
 analogWrite(7,35);
 PhaseStepTime();
 analogWrite(7,0);
 analogWrite(7,0);
 PhaseStepTime();
 }

 void EnterPhase6m4()
{
  //{93,46,0},4KHz
 analogWrite(5,0);
 analogWrite(7,93);
 analogWrite(7,93);
 PhaseStepTime();
 analogWrite(7,46);
 analogWrite(7,46);
 PhaseStepTime();
 analogWrite(7,0);
 analogWrite(7,0);
 PhaseStepTime();
 }

 void EnterPhase6m5()
{
  //{117,58,0},4KHz
 analogWrite(5,0);
 analogWrite(7,117);
 analogWrite(7,117);
 PhaseStepTime();
 analogWrite(7,58);
 analogWrite(7,58);
 PhaseStepTime();
 analogWrite(7,0);
 analogWrite(7,0);
 PhaseStepTime();
 }

 void EnterPhase6m6()
{
  //{140,70,0},4KHz
 analogWrite(5,0);
 analogWrite(7,140);
 analogWrite(7,140);
 PhaseStepTime();
 analogWrite(7,70);
 analogWrite(7,70);
 PhaseStepTime();
 analogWrite(7,0);
 analogWrite(7,0);
 PhaseStepTime();
 }

 void EnterPhase6m7()
{
  //{163,81,0},4KHz
 analogWrite(5,0);
 analogWrite(7,163);
 analogWrite(7,163);
 PhaseStepTime();
 analogWrite(7,81);
 analogWrite(7,81);
 PhaseStepTime();
 analogWrite(7,0);
 analogWrite(7,0);
 PhaseStepTime();
 }

 void EnterPhase6m8()
{
  //{187,93,0},4KHz
 analogWrite(5,0);
 analogWrite(7,187);
 analogWrite(7,187);
 PhaseStepTime();
 analogWrite(7,93);
 analogWrite(7,93);
 PhaseStepTime();
 analogWrite(7,0);
 analogWrite(7,0);
 PhaseStepTime();
 }

 void EnterPhase6m9()
{
  //{210,105,0},4KHz
 analogWrite(5,0);
 analogWrite(7,210);
 analogWrite(7,210);
 PhaseStepTime();
 analogWrite(7,105);
 analogWrite(7,105);
 PhaseStepTime();
 analogWrite(7,0);
 analogWrite(7,0);
 PhaseStepTime();
 }

 void EnterPhase6m10()
{
  //{230,115,0},8KHZ
 analogWrite(5,0);
 analogWrite(7,230);
 analogWrite(7,230);
 PhaseStepTime();
 analogWrite(7,115);
 analogWrite(7,115);
 PhaseStepTime();
 analogWrite(7,0);
 analogWrite(7,0);
 PhaseStepTime();
 }