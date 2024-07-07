// put include declarations here:
#include <Arduino.h>

// put function declarations here:
void StepSignal(int);

// put your setup code here, to run once:
void setup() 
{
 pinMode(LED_BUILTIN,OUTPUT);
 StepSignal(1);
 Serial.begin(9600);
 Serial.println("Simple Board Test Top");
}

// put your main code here, to run repeatedly:
void loop() 
{
 StepSignal(2);
 Serial.println("Simple Board Test Loop");
}

// put function definitions here:
void StepSignal(int step)
{
  int FlashTime = 1000 / step;
   while (step>0)
   {
     digitalWrite(LED_BUILTIN,HIGH);
     delay(FlashTime);
     digitalWrite(LED_BUILTIN,LOW);
     step--;
    }
}