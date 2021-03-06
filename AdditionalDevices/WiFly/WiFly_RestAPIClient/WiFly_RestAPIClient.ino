// WiFly Pachube Client
// Send data to a Pachube Feed
// (Based on Ethernet's WebClient Example)
// (based upon Sparkfun WiFly Web Client example)
// Sparkfun WiFly library updated and can be found here
// https://github.com/jcrouchley/WiFly-Shield
// Built using Arduino IDE V0.22

#include <SPI.h>
#include <WiFly.h>

// using NewSoftSerial V11 beta
// downloaded from here http://arduiniana.org/2011/01/newsoftserial-11-beta/
// this will be included as Software Serial in Arduino IDE 1.0
#include <SoftwareSerial.h>

// Wifly RN-XV (XBee shaped) module connected
//  WiFly Tx to pin 2 (SoftSerial Rx)
//  WiFly Rx to pin 3 (SoftSerial Tx)
//SoftwareSerial mySerial(2, 3);


// Edit credentials.h to provide your own credentials
#include "Credentials.h"

// Using Pachube API V2
WiFlyClient client("restframework.herokuapp.com", 80);

int count;

void setup() {
  //pinMode(MISO, INPUT);
  //pinMode(MOSI, OUTPUT);
  //pinMode(SCK, OUTPUT);
  pinMode(20, OUTPUT);
  digitalWrite(20, LOW);
  pinMode(24, OUTPUT);
  digitalWrite(24, HIGH);
  //pinMode(8,OUTPUT);
  //digitalWrite(8,HIGH);
  // lots of time for the WiFly to start up and also in case I need to stop the transmit
  //delay(10000);
  SPI.begin();
  Serial.begin(9600);  // nice and fast
  //mySerial.begin(9600);  // default WiFly baud rate - good enough for this

  //WiFly.setUart(&mySerial); // Tell the WiFly library that we are not using the SPIUart
  
  Serial.println("Wifly begin");
  
  WiFly.begin();    // startup the WiFly
  
  Serial.println("Wifly join");
  
  // Join the WiFi network
  if (!WiFly.join(ssid, passphrase)) {
    Serial.println("Association failed.");
    while (1) {
      // Hang on failure.
    }
  }  

}

uint32_t timeLastUpdated;
int i;
//char buff[64];

void loop() {
  if (millis() - timeLastUpdated > TIMETOUPDATE)
  {  // time for the next update
    timeLastUpdated = millis();
    
    // prepare the data to send
    // format (API V2)
    // multiple lines each with <datastreamID>,<datastreamValue>
    // feedID can be the datastream name of the numberic ID
    //sprintf(buff,"0,%d\n1,%d",i++,analogRead(0));
    Serial.println("connecting...");
    if (client.connect()) {
      Serial.println("connected");
      client.print("GET / HTTP/1.1\r\n");  // APIV2
      client.print("Host: restframework.herokuapp.com\r\n");
      client.print("User-Agent: Arduino (WiFly RN-XV)\r\n");
      client.print("Accept: application/json; indent=4\r\n");
      client.print("Connection: keep-alive\r\n");
      //client.println("Content-Type: application/json; indent=4");
      client.println();
    } else {
      Serial.println("connection failed");
    }

    delay(200);

    count = 0;
    
    while (client.connected() && count < 20000)
    {
      if (client.available()) 
      {
        // TODO verify success (HTTP/1.1 200 OK)
        Serial.write(client.read());  // display the result
      }
      count++;
    }
    
    Serial.println();
   
    if (client.connected()) {
      Serial.println("disconnecting.");
      client.stop();
      Serial.println("disconnected.");
    }
  }
}


