// Joe Conklin
// 4-16-2013
//----------------------------------- 

#include "WiFly.h"
//#include <string.h> 

//Server Info
byte server[] = {148,61,31,209}; 
Client client("mobile.cis.gvsu.edu", 80);
char passphrase[20];
char ssid[20];
boolean infoRecved = false;
char serialBuf[200];
int debugLED = 6;
String data;

//Sensor Enables
int sensorOn[7] = {0,0,0,0,0,0,0};

//Sensor Pins
//const int soundPin;
//const int vibPin;
//const int levelPin;
const int tempPin = A2;
const int lightPin = A3;
//const int waterPin;
//const int motionPin;

//Sensor Values
int soundVal = 0;
int vibVal = 0;
int levelVal = 0;
int tempVal = 0.0;
int lightVal = 0;
int waterVal = 0;
int motionVal = 0;

//-----------------------------------
//Preliminary setup method
void setup() {
 
 Serial.begin(9600);
 pinMode(debugLED, OUTPUT);
 pinMode(tempPin, INPUT);
 pinMode(lightPin, INPUT);
}

//-----------------------------------
//Main loop
void loop() {
  
  if(sensorOn[0]) {readSound();}
  if(sensorOn[1]) {readVib();}
  if(sensorOn[2]) {readLevel();}
  if(sensorOn[3]) {readTemp();}
  if(sensorOn[4]) {readLight();}
  if(sensorOn[5]) {readWater();}
  if(sensorOn[6]) {readMotion();}
  
  Serial.println("Data");
  //Serial.print("Sound:");
  //Serial.println(soundVal);
  //Serial.print("Vibration:");
  //Serial.println(vibVal);
  //Serial.print("Level:");
  //Serial.println(levelVal);
  Serial.print("Temp:");
  Serial.println(tempVal);
  Serial.print("Light:");
  Serial.println(lightVal);
  //Serial.print("Water:");
  //Serial.println(waterVal);
  //Serial.print("Motion:");
  //Serial.println(motionVal);
  
  delay(1000);
  
  if(infoRecved)
  {
    digitalWrite(debugLED, HIGH);
    sendData();
  }
  
  if(Serial.available())
  {
    serialEvent();
  }
}

//---------------------------------------
// Read Sensors
void readSound()
{
}

void readVib()
{
}

void readLevel()
{
}
void readTemp()
{
  int tempRead = analogRead(tempPin);
  float celsius = 25 + (tempRead - 512) / 11.3;
  tempVal = celsius * 1.8 + 32;
}

void readLight()
{
  lightVal = analogRead(lightPin);
}

void readWater()
{
}

void readMotion()
{
}

void sendData()
{
  Serial.println("Connecting");
  if(client.connect())
  {
    data="";
    data+="temp=";
    data+= tempVal;
    data+="&light=";
    data+=lightVal;
    
    client.println("POST /funware/devices/new_record HTTP/1.1");
    client.println("Host: mobile.cis.gvsu.edu");
    client.println("User-Agent: Arduino (WiFly RN-XV)");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(data.length());
    client.println();
    client.println(data);
    client.println();
  }
  else
    Serial.println("Connection Failed");
    
   if (client.connected())
   {
     Serial.println("Disconnecting");
     client.stop();
     Serial.println("Disconnected");
   }
}


//-----------------------------------
//Method to establish wifi connection
boolean wifi_connect(){
  
  Serial.println("WiFly begin");
  WiFly.begin();
  Serial.println("WiFly join");
  if(!WiFly.join(ssid, passphrase))
  {
    Serial.println("Association failed");
    return false;
  }
  return true;
}

//-----------------------------------
//
void serialEvent()
{
  int i = 0;
  while(Serial.available() > 0)
  {
    serialBuf[i] = Serial.read();
    i++;
  }
  Serial.println(serialBuf);
  parseBuffer();
  infoRecved = wifi_connect();
}

//-----------------------------------
//
void parseBuffer()
{
  int i = 0;
  int j = 0;
  while(serialBuf[i] != ',')
  {
    ssid[j] = serialBuf[i];
    i++;
    j++;
  }
  j=0;
  i++;
  while(serialBuf[i] != ',')
  {
    passphrase[j] = serialBuf[i];
    i++;
    j++;
  }
  j=0;
  i++;
  while (serialBuf[i])
  {
    if(serialBuf[i] != ',')
    {
      sensorOn[j] = serialBuf[i]-48;
      j++;
    }
    i++;
  } 
  
}
