#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

int counter = 0;

/*
 * The setup function.
 */
void setup() 
{
  Serial.begin(115200);
  Serial.println("one");
  pinMode(6, OUTPUT);
  wdt_setup();
}

/*
 * The main loop.
 */
void loop()
{
  Serial.println("Loop");
  delay(100);
  enterSleep();
}

//just flashes an LED every 8 seconds for now
ISR(WDT_vect)
{
  Serial.println("ISR");
  counter++;
  
  if(counter % 2)
  {
    digitalWrite(6, HIGH);
  }
  else
  {
    digitalWrite(6, LOW);
  }
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

/*
 * Setup of the watchdog timer.
 * WDT is defined on page 63 of http://www.atmel.com/Images/doc7593.pdf
 */
void wdt_setup() 
{
  // disable global interrupts
  cli();
  
  // clear MCU status register
  MCUSR = 0;
  
  // prepare the register to be changed
  WDTCSR |= B00011000;
  
  // Set interrupt enable and thescaler to 8 seconds
  WDTCSR = B01100001;
  
  // enable global interrupts
  sei();
}
