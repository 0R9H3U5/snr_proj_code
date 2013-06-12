char mode;

void setup()
{
  Serial.begin(38400);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  mode = 0;
}

void loop()
{
  if(!mode)
  {
   digitalWrite(4, HIGH);
   delay(500); 
   mode = 1;
   Serial.println(F("Light is ON"));
  }
  else
  {
    digitalWrite(4, LOW);
    delay(500);
    mode = 0;
    Serial.println(F("Light is OFF"));
  }
}
