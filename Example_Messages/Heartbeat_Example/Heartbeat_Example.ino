#include "WiFly.h"

//Server Info
byte server[] = {148,61,31,209}; 
Client client("mobile.cis.gvsu.edu", 80);
char passphrase[20];
char ssid[20];
boolean infoRecved = false;
char serialBuf[200];

void sendData(String data)
{
  Serial.println(data);
  
  Serial.println("Connecting");
  if(client.connect())
  {    
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

void setup()
{
  Serial.begin(115200);
  
  WiFly.begin();
  
  char id[18] = {'\n'};
  WiFly.getDeviceID(id);
  String s;
  s += id;
  
  sendData("bat=100&id=" + s);
}

void loop()
{}


