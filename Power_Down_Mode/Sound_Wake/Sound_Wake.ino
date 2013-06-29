#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>

const int ledPin = 6;
const int wakeupPin = 2;
volatile int ledState = LOW;
int counter = 0;
volatile int ledVal = 0;

const int micWakePin = 18;

void wakeUp()
{
  Serial.println("in wakeUp()");
  ledVal++;
  if(ledVal % 2)
    digitalWrite(ledPin, HIGH);
  else
    digitalWrite(ledPin, LOW);
    
  //enterSleep();
}

void setup()
{
  Serial.begin(115200);
  Serial.println("begin init");
  
  pinMode(micWakePin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(wakeupPin, INPUT);
  attachInterrupt(micWakePin, wakeUp, CHANGE); //LOW, RISING, FALLING, CHANGE
  ledVal=0;
  digitalWrite(ledPin, ledVal);
  
  Serial.println("done init");
  
  //enterSleep();
}

void enterSleep(void)
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   /* EDIT: could also use SLEEP_MODE_PWR_SAVE for lowest power consumption. */
  sleep_enable();
  
  /* Now enter sleep mode. */
  sleep_mode();
  
  /* The program will continue from here after the WDT timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */
  
  /* Re-enable the peripherals. */
  power_all_enable();
}

void loop()
{
  Serial.println("Loop");
  delay(100);
  enterSleep();
}
