#include "WiFly.h"
#include "SPI.h"

#define MAX_WAIT 40
#define MAX_CHAR_LEN 129

//Memory addresses for router info
#define ADDR_SSID 0x00000000
#define ADDR_PW   0x00000080

//Pin connections to Flash IC
#define CE_MEM   24
#define HOLD_MEM 25
#define WP_MEM   26

//Flash IC Operation bytes
#define READ      0x03
#define WREN      0x06
#define WRDI      0x04
#define WRSR      0x01
#define BYTE_PROG 0x02
#define WRITE_AAI 0xAD
#define ERASE_4KB 0x20

//Flash IC needs to take 10 usecs for writing every 2 bytes
#define HW_WR_DELAY 10

#define SECT_4kB_CLR_DELAY 18

//Address byte masks
#define MSB 0x00FF0000 
#define MIB 0x0000FF00
#define LSB 0x000000FF

byte server[] = {148,61,31,209}; 
WiFlyClient client("mobile.cis.gvsu.edu", 80);
boolean infoRecved = false;
char serialBuf[200];
int debugLED = 6;
String data;

//Router Info char arrays
char ssid[MAX_CHAR_LEN];
char passphrase[MAX_CHAR_LEN];

//Sensor Enables
int sensorOn[7] = {0,0,0,1,1,0,0};

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

//Keeps track of length of ssid and passphrase
uint8_t ssid_len = 0;
uint8_t passphrase_len = 0;


uint32_t address;
byte sector;

void setup()
{
  Serial.begin(9600);
  pinMode(debugLED, OUTPUT);
  pinMode(tempPin, INPUT);
  pinMode(lightPin, INPUT);
  
  // Set up pin direction for non-SPI pins that affect Flash IC
  // These pins are all active LOW
  pinMode(CE_MEM, OUTPUT);
  digitalWrite(CE_MEM, HIGH);
  pinMode(HOLD_MEM, OUTPUT);
  digitalWrite(HOLD_MEM, HIGH);
  pinMode(WP_MEM, OUTPUT);
  digitalWrite(WP_MEM, HIGH);
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  
  SPI.begin();
  
  memClearStatusReg();
  
  Serial.println(F("Begin Memory/Wi-Fi Test\n"));
}

void loop()
{
 //Keep waiting for router info before moving on 
 while(readRouterInfo(ssid, passphrase))
 {
   Serial.flush();
   //Display current contents of strings for debugging
   Serial.println(ssid);
   Serial.println(passphrase);
   
   delay(250);
 }
 
   Serial.flush();
   //Display current contents of strings for debugging
   Serial.println(ssid);
   Serial.println(passphrase);
   
   delay(250);
 
 //Clear sector of Flash IC containing ssid and password, then write new ssid and password
 //----Passing char array--------------------------------------------------------------
  address = ADDR_SSID;
  sector = 0x00;
  
  memClearSector();
  
  memWriteData(ssid);

  ssid_len = strlen(ssid);
 
  address = ADDR_PW;
  
  memWriteData(passphrase);
  
  passphrase_len = strlen(passphrase);

 //Read router info from memory, collect ADC samples, and communicate with server over Wi-Fi
 while(1)
 {
   //Clear router info for test purposes
   memset(&ssid[0], 0, MAX_CHAR_LEN*sizeof(char));
   memset(&passphrase[0], 0, MAX_CHAR_LEN*sizeof(char));
   
   address = ADDR_SSID;
   memReadData(ssid, ssid_len);
   address = ADDR_PW;
   memReadData(passphrase, passphrase_len);
   
   Serial.println("");
   Serial.println(ssid);
   Serial.println(passphrase);
   
  if(sensorOn[0]) {readSound();}
  if(sensorOn[1]) {readVib();}
  if(sensorOn[2]) {readLevel();}
  if(sensorOn[3]) {readTemp();}
  if(sensorOn[4]) {readLight();}
  if(sensorOn[5]) {readWater();}
  if(sensorOn[6]) {readMotion();}
  
  Serial.print(F("Sound:"));
  Serial.println(soundVal);
  Serial.print(F("Vibration:"));
  Serial.println(vibVal);
  Serial.print(F("Level:"));
  Serial.println(levelVal);
  Serial.print(F("Temp:"));
  Serial.println(tempVal);
  Serial.print(F("Light:"));
  Serial.println(lightVal);
  Serial.print(F("Water:"));
  Serial.println(waterVal);
  Serial.print(F("Motion:"));
  Serial.println(motionVal);
  
  delay(1000);
  
  wifi_connect();
  
  sendData();
 }
}

/*
 * Description: Reads in the router ssid and password from a serial connection. 
 *              Must be passed pointers to the ssid and passphrase char arrays
 *              as arguments.
 *  
 * Arguments: rtr_ssid - pointer to char array that stores router ssid
 *
 *            rtr_passphrase - pointer to char array that stores router password
 *
 * Returns: 0 if success, 1 if failure
 */
char readRouterInfo(char* rtr_ssid, char* rtr_passphrase)
{
 char count = 0;
 
 while(!Serial.available() && count < MAX_WAIT)
 {
  delay(250);
  count++; 
 }
 
 if(count == MAX_WAIT)
 {
  return 1; 
 }
 else
 { 
   if(Serial.available())
   {
     rtr_ssid[0] = Serial.read();
   
     for(count = 1; Serial.available() && count < MAX_CHAR_LEN - 1 && rtr_ssid[count-1] != ','; count++)
     {
       rtr_ssid[count] = Serial.read();
     }
     
     if(rtr_ssid[count-1] == ',')
     {
       rtr_ssid[count-1] = '\0';
     }
   }
   else
   {
     return 1;
   } 
  
   if(Serial.available())
   {
     rtr_passphrase[0] = Serial.read();
     
     for(count = 1; Serial.available() && count < MAX_CHAR_LEN - 1 && rtr_passphrase[count-1] != ','; count++)
     {
       rtr_passphrase[count] = Serial.read();
     }
     
     if(rtr_passphrase[count-1] == ',')
     {
       rtr_passphrase[count-1] = '\0';
     }
   }
   else
   {
     return 1; 
   }
   
   return 0;
 } 
}

/*
 * Write char array to starting address in Flash IC
 */
void memWriteData(char* transfer_data)
{
  //Exit write function if nothing to write
  if(strlen(transfer_data) <= 0)
  {
    return;  
  }
  
  //If the data written contains an even number of bytes
  else if(strlen(transfer_data) > 1)
  {
    //Send initialization bytes for writing
    memWrite();
    
    //Write initial byte pair
    SPI.transfer(transfer_data[0]);
    SPI.transfer(transfer_data[1]);
    digitalWrite(CE_MEM, HIGH);
    writeDelay();
    
    //Write the remaining byte pairs, if any
    for(int i = 1; i < ((strlen(transfer_data))>>1); i++)
    {
      digitalWrite(CE_MEM, LOW);
      SPI.transfer(WRITE_AAI);
      SPI.transfer(transfer_data[2*i]);
      SPI.transfer(transfer_data[2*i+1]);
      digitalWrite(CE_MEM, HIGH);
      writeDelay();
    }
    //Send end byte
    memWriteEndAAI();
    
    if(strlen(transfer_data)%2 != 0)
    {
      memWriteEnable();
    
      digitalWrite(CE_MEM, LOW);
      //Write the last byte 
      SPI.transfer(BYTE_PROG);
      SPI.transfer( ((address + (strlen(transfer_data) - 1)) & MSB) >> 16 );
      SPI.transfer( ((address + (strlen(transfer_data) - 1)) & MIB) >> 8 );
      SPI.transfer( (address + (strlen(transfer_data) - 1)) & LSB);
      SPI.transfer( transfer_data[strlen(transfer_data) - 1] );
      digitalWrite(CE_MEM, HIGH);
      writeDelay();  
    }
  }
  //If only one byte of data to write
  else if(strlen(transfer_data) == 1)
  {
    //Enable writing
    memWriteEnable();
    
    digitalWrite(CE_MEM, LOW);
    //Write the last byte 
    SPI.transfer(BYTE_PROG);
    SPI.transfer( (address & MSB) >> 16 );
    SPI.transfer( (address & MIB) >> 8 );
    SPI.transfer( address & LSB);
    SPI.transfer( transfer_data[strlen(transfer_data) - 1] );
    digitalWrite(CE_MEM, HIGH);
    writeDelay();
  }
}

/*
 * Send AAI operation byte and starting address to Flash IC
 */
void memWrite()
{
  memWriteEnable();
  
  //memClearStatusReg();
  
  digitalWrite(CE_MEM, LOW);
  // Send the write AAI operation byte 
  SPI.transfer(WRITE_AAI);
  
  // Send the starting address
  SPI.transfer((address & MSB)>>16);
  SPI.transfer((address & MIB)>>8);
  SPI.transfer(address & LSB); 
}

/*
 * Software delay to allow Flash IC time to finish writing
 */
void writeDelay()
{
  delayMicroseconds(HW_WR_DELAY); 
}

/*
 * Send end write byte to Flash IC
 */
void memWriteEndAAI()
{
  digitalWrite(CE_MEM, LOW);
  SPI.transfer(WRDI);
  digitalWrite(CE_MEM, HIGH);
}

/*
 * Send clear operation to erase 4 KB sector in Flash IC
 */
void memClearSector()
{ 
  memWriteEnable();
  
  digitalWrite(CE_MEM, LOW);
  SPI.transfer(ERASE_4KB);
  SPI.transfer(0x00);
  SPI.transfer((sector<<4));
  SPI.transfer(0x00); 
  digitalWrite(CE_MEM, HIGH);
  
  //IC needs 18ms to finish clear
  delay(SECT_4kB_CLR_DELAY);
}

/*
 * Used to initialize reads from a given location on the Flash IC
 */
void memRead()
{
  //Select flash IC
  digitalWrite(CE_MEM, LOW);
  
  // Send the read operation byte 
  SPI.transfer(READ);
  
  // Send the starting address
  SPI.transfer((address & MSB)>>16);
  SPI.transfer((address & MIB)>>8);
  SPI.transfer(address & LSB);
}

void memClearStatusReg()
{
  //Send write enable byte
  digitalWrite(CE_MEM, LOW);  
  SPI.transfer(WRSR);
  SPI.transfer(0x00);
  digitalWrite(CE_MEM, HIGH);
}

void memWriteEnable()
{
  //Select flash IC
  digitalWrite(CE_MEM, LOW);
  
  //Send write enable byte
  SPI.transfer(WREN);
  digitalWrite(CE_MEM, HIGH);
}

/*
 * Read char array at starting address in Flash IC
 */
void memReadData(char* received_data, uint8_t len)
{
  //Send initialization bytes for reading
  memRead();
  
  for(int i = 0; i < len; i++)
  {
    received_data[i] = SPI.transfer(0x00);
  }
  
  digitalWrite(CE_MEM, HIGH);
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
  Serial.println(F("Connecting"));
  if(client.connect())
  {
    data="";
    data+=F("temp=");
    data+= tempVal;
    data+=F("&light=");
    data+=lightVal;
    
    client.println(F("POST /funware/devices/new_record HTTP/1.1"));
    client.println(F("Host: mobile.cis.gvsu.edu"));
    client.println(F("User-Agent: Arduino (WiFly RN-XV)"));
    client.println(F("Content-Type: text/html"));
    client.println(F("Connection: close"));
    client.print(F("Content-length: "));
    client.println(data.length());
    client.println();
    client.println(data);
    client.println();
  }
  else
    Serial.println(F("Connection Failed"));
    
   if (client.connected())
   {
     Serial.println(F("Disconnecting"));
     client.stop();
     Serial.println(F("Disconnected"));
   }
}


//-----------------------------------
//Method to establish wifi connection
boolean wifi_connect(){
  
  Serial.println(F("WiFly begin"));
  WiFly.begin();
  Serial.println(F("WiFly join"));
  if(!WiFly.join(ssid, passphrase))
  {
    Serial.println(F("Association failed"));
    return false;
  }
  return true;
}
