/* LED Blink, Teensyduino Tutorial #1
   http://www.pjrc.com/teensy/tutorial.html
 
   This example code is in the public domain.
*/

const int temp = A2;
const int light = A3;

// the setup() method runs once, when the sketch starts

void setup() {
  //intialize serail
  Serial.begin(38400);
  pinMode(temp, INPUT);
  pinMode(light, INPUT);
}

// the loop() methor runs over and over again,
// as long as the board has power

void loop() {
  
  int tempVal = analogRead(temp);
  int lightVal = analogRead(light);
  Serial.print("temp(f): ");
  Serial.println(tempVal);
  Serial.print("light: ");
  Serial.println(lightVal);

  delay(1000);                  // wait for a second
}
