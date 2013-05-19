
// (Based on Ethernet's WebClient Example)

#include "WiFly.h"


#include "Credentials.h"


byte server[] = { 66, 249, 89, 104 }; // Google

//Client client(server, 80);

Client client("google.com", 80);

void setup() {
  
  Serial.begin(9600);

  Serial.println("one");

  WiFly.begin();
  
  Serial.println("two");
  /*
  if (!WiFly.join(ssid, passphrase)) {
    Serial.println("Association failed.");
    while (1) {
      Serial.println("Could not join network");
    }
  }  
  */
  char id[18] = {'\n'};
  //Mac Addr=00:06:66:30:88:fb
  WiFly.getDeviceID(id);
  
  Serial.print("ID: ");
  Serial.println(id);
/*
  Serial.println("connecting...");

  if (client.connect()) {
    Serial.println("connected");
    client.println("GET /search?q=arduino HTTP/1.0");
    client.println();
  } else {
    Serial.println("connection failed");
  }
  */
}

void loop() {
  /*
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    for(;;)
      ;
  }
  */
}


