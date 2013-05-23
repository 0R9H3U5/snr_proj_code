// Joe Conklin
// 4-16-2013
//----------------------------------- 

#include "WiFly.h"
//#include <string.h> 

//Server Info
byte server[] = {148,61,31,209}; 
Client client("mobile.cis.gvsu.edu", 80);
char passphrase[20] = "gWe9kHfmKT";
char ssid[20] = "BrownSpruce";
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
 wifi_connect();
}

//-----------------------------------
//Main loop
void loop() {
  
  readTemp();
  readLight();
  Serial.print("Temp:");
  Serial.println(tempVal);
  Serial.print("Light:");
  Serial.println(lightVal);
  sendData();
  
}

void readTemp()
{
  tempVal = analogRead(tempPin);
}

void readLight()
{
  lightVal = analogRead(lightPin);
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
void wifi_connect(){
  
  Serial.println("WiFly begin");
  WiFly.begin();
  Serial.println("WiFly join");
  if(!WiFly.join(ssid, passphrase))
  {
    Serial.println("Association failed");
  }
}
