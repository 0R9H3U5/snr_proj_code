#include <SPI.h>

//Pin connections to Flash IC
#define CE_MEM   24
#define HOLD_MEM 25
#define WP_MEM   26

//Flash IC Operation bytes
#define READ      0x03
#define WREN      0x06
#define WRDI      0x04
#define BYTE_PROG 0x02
#define WRITE_AAI 0xAD
#define ERASE_4KB 0x20
#define RDSR      0x05
#define EWSR      0x50
#define WRSR      0x01

//Flash IC needs to take 10 usecs for writing every 2 bytes
#define HW_WR_DELAY 15

void setup()
{
  // Set up pin direction for non-SPI pins that affect Flash IC
  // These pins are all active LOW
  pinMode(CE_MEM, OUTPUT);
  digitalWrite(CE_MEM, HIGH);
  pinMode(HOLD_MEM, OUTPUT);
  digitalWrite(HOLD_MEM, HIGH);
  pinMode(WP_MEM, OUTPUT);
  digitalWrite(WP_MEM, HIGH);
  
  SPI.begin();
  Serial.begin(9600);
  
  Serial.println(F("Begin Memory Test\n"));
  
  
}

void loop()
{
  byte in = 12;
  byte out = 0;
  
  //Send write enable byte
  digitalWrite(CE_MEM, LOW);  
  SPI.transfer(WREN);
  digitalWrite(CE_MEM, HIGH);
  
  digitalWrite(CE_MEM, LOW); 
  SPI.transfer(RDSR);
  out = SPI.transfer(0x00);
  digitalWrite(CE_MEM, HIGH);
  
  Serial.println("Status Register before WRSR: ");
  Serial.println(out);
  
  //Send write enable byte
  digitalWrite(CE_MEM, LOW);  
  SPI.transfer(WRSR);
  SPI.transfer(0x00);
  digitalWrite(CE_MEM, HIGH);
  
  digitalWrite(CE_MEM, LOW); 
  SPI.transfer(RDSR);
  out = SPI.transfer(0x00);
  digitalWrite(CE_MEM, HIGH);
  
  Serial.println("Status Register after WRSR: ");
  Serial.println(out);
  
  //Send write enable byte
  digitalWrite(CE_MEM, LOW);  
  SPI.transfer(WREN);
  digitalWrite(CE_MEM, HIGH);
  
  digitalWrite(CE_MEM, LOW); 
  SPI.transfer(RDSR);
  out = SPI.transfer(0x00);
  digitalWrite(CE_MEM, HIGH);
  
  Serial.println("Status Register after WREN: ");
  Serial.println(out);
  
  digitalWrite(CE_MEM, LOW);
  SPI.transfer(ERASE_4KB);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00); 
  digitalWrite(CE_MEM, HIGH);
  
  //IC needs 25ms to finish clear
  delay(40);
  
  digitalWrite(CE_MEM, LOW); 
  SPI.transfer(RDSR);
  out = SPI.transfer(0x00);
  digitalWrite(CE_MEM, HIGH);
  
  Serial.println("Status Register after ERASE_4kB: ");
  Serial.println(out);
  
  digitalWrite(CE_MEM, LOW);
  
   // Send the read operation byte 
  SPI.transfer(READ);
  
  // Send the starting address
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  
  out = SPI.transfer(0x00);
  
  digitalWrite(CE_MEM, HIGH);
  
  Serial.print(F("Should be 0xFF: "));
  Serial.println(out);
  Serial.println();
  
  //Send write enable byte
  digitalWrite(CE_MEM, LOW);  
  SPI.transfer(WREN);
  digitalWrite(CE_MEM, HIGH);
  
  digitalWrite(CE_MEM, LOW); 
  SPI.transfer(RDSR);
  out = SPI.transfer(0x00);
  digitalWrite(CE_MEM, HIGH);
  
  Serial.println("Status Register: ");
  Serial.println(out);
  
  digitalWrite(CE_MEM, LOW);
  
  //Write the last byte 
    SPI.transfer(BYTE_PROG);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    SPI.transfer(in);
    digitalWrite(CE_MEM, HIGH);
    delayMicroseconds(HW_WR_DELAY);
  
  digitalWrite(CE_MEM, LOW); 
  SPI.transfer(RDSR);
  out = SPI.transfer(0x00);
  digitalWrite(CE_MEM, HIGH);
  
  Serial.println("Status Register: ");
  Serial.println(out);
    
  Serial.print(F("Written value: "));
  Serial.println(in);
  Serial.println();
    
    //Select flash IC
  digitalWrite(CE_MEM, LOW);
  
  // Send the read operation byte 
  SPI.transfer(READ);
  
  // Send the starting address
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  
  out = SPI.transfer(0x00);
  
  digitalWrite(CE_MEM, HIGH);
  
  Serial.print(F("Written value read as: "));
  Serial.println(out);
  Serial.println(); 
  
  delay(1000);
}

