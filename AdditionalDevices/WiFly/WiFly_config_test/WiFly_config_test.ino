/*
  Timer
 
  This program takes analog input from a soleniod monitoring
  module on pin 9 (max 5V, 40mA), and records and transmits
  its "on" duration.
 
  Used in conjunction with a PC running a monitoring program,
  on the same Wi-Fi network with preconfigured settings, this
  program allows the Arduino UNO to communicate these timed
  durations with the PC, for further calculation and storage.
 
  This program is Intellectual Property of GSMAC.
  Created 26 Jan 2011
  Modified 5 Apr 2011
  -- Added functionality for WiFly GSX Module to send data
     over Wi-Fi network
     
  By Gary Willette
 
   
 The circuit:
 * Soleniod monitoring module attached to digital input 9
 * Pin 9 connected to ground via a 10kOhm resistor (pulldown)
 * Sparkfun WiFly shield attached to UNO
 */
 
// include the WiFly code to communicate with SpiSerial:
#include "WiFly.h"

//SET THESE PARAMETERS FOR WI-FI NETWORK
const char WEP_KEY [] = "enter_key_here";
const char AUTH_LEVEL [] = "1"; // 0=OPEN 1=WEP 2=WPA1 3=Mixed WPA1&WPA2-PSK 4=WPA2-PSK 6=AD HOC
const char SSID [] = "enter_SSID_here";
const char DHCP [] = "0"; //0=static (DHCP disabled) 1=DHCP enabled
const char WIFLY_IP [] = "192.168.1.208";
const char BACKUP_IP [] = "192.168.1.209";
const char WIFLY_PORT [] = "2000";
const char PC_IP [] = "192.168.1.7";
const char PC_PORT [] = "2000";
const char GATEWAY_IP [] = "192.168.1.1";
const char NETMASK [] = "255.255.255.0";
const char DNS_SERVER [] = "192.168.1.1";
const char ANTENNA_OPTION [] = "0";  //0=internal, 1=external
const char UART_MODE [] = "2"; //2=make TCP connection when there is data to send
const char LINKMON [] = "5"; //number of AP re-scans until reassociation

//define constants to be used
const int dataSendInterval = 10000; // interval of time to send data (5 seconds)
const int Delay = 100; //interval to wait between wi-fi parameter sets
const int buttonPin = 6;       // select the input pin for the button output
const int ledPin = 5;

//data variables
unsigned long currentFillTime = 0;
unsigned long afterFill = 0;
unsigned long beforeFill = 0;
unsigned long lastDataSend = 0;
unsigned long totalFillTime = 0;
unsigned long oldFillTime1 = 0;
unsigned long oldFillTime2 = 0;
int transmissionNumber = 0;

// Variables will change:
int ledState = LOW;          // the current state of the output pin
int buttonState = LOW;       // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int buttonPush = 0;          // will prevent writing vars while button is held
long lastDebounceTime = 0;   // the last time the output pin was toggled
int debounceDelay = 40;      // the debounce time; increase if the output flickers

//displays on terminal everything in SPI Serial buffer
void readFromWifly(){
  delay(100);
  while(SpiSerial.available() > 0) {
    Serial.print(SpiSerial.read());
  }
  Serial.print("\n");
  delay(100);
}

void setup() {
  pinMode(buttonPin, INPUT);  // declare the sensor pin as input
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
  Serial.println("Attempting to connect to SPI UART");
  SpiSerial.begin();
  Serial.println("Connected to SPI UART");
 
  // Exit command mode if we haven't already
  SpiSerial.println(""); 
  SpiSerial.println("exit");
  readFromWifly();
  // Enter command mode
  SpiSerial.print("$$$");
  delay(100);

  // Reboot to get device into known state
  Serial.println("Rebooting");
  SpiSerial.println("reboot");
  readFromWifly();
  delay(3000);
 
  // Enter command mode
  Serial.println("\nEntering command mode.");
  SpiSerial.print("$$$");
  readFromWifly();
  delay(500);

  // Set authorization level to <auth_level>
  SpiSerial.print("set w a ");
  SpiSerial.println(AUTH_LEVEL);
  Serial.print("Set wlan to authorization level ");
  Serial.println(AUTH_LEVEL);
  readFromWifly();
  delay(500);
 
  //disable DHCP
  SpiSerial.print("set ip dhcp ");
  SpiSerial.println(DHCP);
  Serial.print("Set WiFly DHCP to: ");
  Serial.println(DHCP); 
  readFromWifly();
  delay(500);
 
  //set WiFly IP Address
  SpiSerial.print("set ip address ");
  SpiSerial.println(WIFLY_IP);
  Serial.print("Set WiFly IP to: ");
  Serial.println(WIFLY_IP); 
  readFromWifly();
  delay(500);
 
  //set Backup WiFly IP Address 
  SpiSerial.print("set ip backup ");
  SpiSerial.println(BACKUP_IP);
  Serial.print("Set Backup IP to: ");
  Serial.println(BACKUP_IP); 
  readFromWifly();
  delay(500);
 
  //set WiFly remote port
  SpiSerial.print("set ip local_port ");
  SpiSerial.println(WIFLY_PORT);
  Serial.print("Set Wifly local port to: ");
  Serial.println(WIFLY_PORT); 
  readFromWifly();
  delay(500);
 
  //set PC IP Address
  SpiSerial.print("set ip host ");
  SpiSerial.println(PC_IP);
  Serial.print("Set PC IP to: ");
  Serial.println(PC_IP); 
  readFromWifly();
  delay(500);
 
  //Set PC remote port
  SpiSerial.print("set ip remote_port ");
  SpiSerial.println(PC_PORT);
  Serial.print("Set PC remote port to: ");
  Serial.println(PC_PORT); 
  readFromWifly();
  delay(500);
 
  //set Gateway IP Address
  SpiSerial.print("set ip gateway ");
  SpiSerial.println(GATEWAY_IP);
  Serial.print("Set gateway IP to: ");
  Serial.println(GATEWAY_IP); 
  readFromWifly();
  delay(500);

  //set Netmask Address
  SpiSerial.print("set ip netmask ");
  SpiSerial.println(NETMASK);
  Serial.print("Set netmask to: ");
  Serial.println(NETMASK); 
  readFromWifly();
  delay(500);
 
  //Set DNS Address
  SpiSerial.print("set dns address ");
  SpiSerial.println(DNS_SERVER);
  Serial.print("Set dns addresss to: ");
  Serial.println(DNS_SERVER); 
  readFromWifly();
  delay(500);

  // Set passphrase to <auth_phrase>
  SpiSerial.print("set wlan key ");
  SpiSerial.println(WEP_KEY);
  Serial.print("Set security phrase to ");
  Serial.println(WEP_KEY);
  readFromWifly();
  delay(500);
 
  //set antenna to internal or external
  SpiSerial.print("set wlan ext_antenna ");
  SpiSerial.println(ANTENNA_OPTION);
  Serial.print("Set wlan ext_antenna: ");
  Serial.println(ANTENNA_OPTION); 
  readFromWifly();
  delay(500);
 
  //setup to make tcp connection when data from uart is received
  //and close connection after data is sent
  SpiSerial.print("set uart mode ");
  SpiSerial.println(UART_MODE);
  Serial.print("Set UART mode: ");
  Serial.println(UART_MODE); 
  readFromWifly();
  delay(500);
   
  //set access point link monitor (if AP goes down, scan x # of times before
  //re-associating)
  SpiSerial.print("set wlan linkmon ");
  SpiSerial.println(LINKMON);
  Serial.print("set Access point rescans to ");
  Serial.println(LINKMON);
  readFromWifly();
 
  // Join wifi network <ssid>
  SpiSerial.flush();
  Serial.print("Joining '");
  Serial.print(SSID);
  Serial.println("'");
  SpiSerial.print("join ");
  SpiSerial.println(SSID);
  delay(5000);
  readFromWifly();
 
  //ping PC IP Address (to check for connection
  SpiSerial.print("ping ");
  SpiSerial.println(PC_IP);
  Serial.print("\nping ");
  Serial.println(PC_IP);
  readFromWifly();
  delay(2000);
 
  //save config
  SpiSerial.println("save");

  //show network details
  SpiSerial.println("show net");
  readFromWifly();
  SpiSerial.println("get uart");
  readFromWifly();
  SpiSerial.println("exit");
}

void loop()
{ 
  
}
