/* ZILOG - Motion Activated LED 
  
  Pin# | Name      | Description
  ---------------------------------  
    1  | GND       | Conect to Ground
    2  | VDD       | Connect to 3.3v
    3  | RXD/DLY   | Conect to Ground
    4  | !TXD/SNS  | Conect to Ground
    5  | !MD/!RST  | Connect to MC pin2
    6  | LG        | Conect to 3.3v
    7  | !SLP/DBG  | Conect to 3.3v
    8  | GND       | Conect to Ground
*/


int motionDetect = P2;
int led = 13;

void setup() 
{
  pinMode( motionDetect, INPUT );
  pinMode( led, OUTPUT );
  delay(100);
}

void loop() 
{
  if ( digitalRead( motionDetect ) ) 
  {
    /* turn the LED on */
    digitalWrite( led, LOW ); 
  }
  else
  {
    /* turn the LED off */
    digitalWrite( led, HIGH ); 
  }

  delay(25);
}
