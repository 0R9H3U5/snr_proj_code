const int ledPin = 6;
const int wakeupPin = 2;
volatile int ledVal = 0;

void wakeUp()
{
  Serial.println("in wakeUp()");
  ledVal++;
  if(ledVal % 2)
    digitalWrite(ledPin, HIGH);
  else
    digitalWrite(ledPin, LOW);
}

void setup()
{
  Serial.begin(38400);
  pinMode(ledPin, OUTPUT);
  pinMode(wakeupPin, INPUT);
  attachInterrupt(6, wakeUp, CHANGE); //LOW,RISING, FALLING,CHANGE
  
  digitalWrite(ledPin, ledVal);
  Serial.println("done init");
}

void loop()
{
  
}
