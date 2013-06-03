/**
  Reads input on pin 2
  Uses onboard LED to display that motion was detected
  
  By: Tyler DeVoogd
*/

String write_md_pin = "C";
String motion_detect_mode = "M";
byte ACK = 0x06;
byte NACK = 0x15;
int motion_signal = 0;
volatile int state = LOW;

void setup()
{
  pinMode(2, INPUT);
  pinMode(6, OUTPUT);
  
  attachInterrupt(motion_signal, stateChange, LOW);
  Serial.begin(9600);  // Open serial connections
  
  setMotionDetectMode(); // Configure MD pin to no longer be RST
}

void loop()
{  
}

void setMotionDetectMode()
{
   byte read_data = NACK;
   
   Serial.print(write_md_pin);  // Send command to write
   Serial.read();  // Read old pin mode value
   
   //while(read_data != ACK)  //While not acknowledged
   //{
     Serial.print(motion_detect_mode); // Send md pin mode
     read_data = Serial.read();  // Read ACK or NACK
   //}
}

void stateChange()
{ 
      digitalWrite(13, HIGH);
      
      delay(2);
      
      digitalWrite(13, LOW);
}
