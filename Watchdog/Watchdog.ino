#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

int counter = 0;

/*
 * The setup function.
 */
void setup() 
{
  Serial.begin(115200);
  pinMode(6, OUTPUT);
  wdt_setup();
}

/*
 * The main loop.
 */
void loop()
{
  Serial.println("Loop");
  delay(3000);
}

//just flashes an LED every 8 seconds for now
ISR(WDT_vect)
{
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

/*
 * Setup of the watchdog timer.
 * WDT is defined on page 63 of http://www.atmel.com/Images/doc7593.pdf
 */
void wdtSetup() 
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
