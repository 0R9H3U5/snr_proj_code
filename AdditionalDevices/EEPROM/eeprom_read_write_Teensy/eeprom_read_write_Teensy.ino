/*
 * EEPROM Write
 *
 * Stores values read from analog input 0 into the EEPROM.
 * These values will stay in the EEPROM when the board is
 * turned off and may be retrieved later by another sketch.
 */

#include <EEPROM.h>

#define SIZE 4

// the current address in the EEPROM (i.e. which byte
// we're going to write to next)
uint16_t addr = 0;
uint16_t wr_val = 0;
uint8_t rd_val;

int count = 0;

char temp[SIZE];
char mode;

boolean flag = false;

void setup()
{
  Serial.begin(38400);
}

void loop()
{ 
  /* Get address of EEPROM to manipulate */
  while(flag == false)
  {
    flag = true;
    addr = 0;
    
    Serial.println("Enter address (0 - 4095)");
    
    //Wait for user to enter data
    while(!Serial.available());
    
    count = 0;
    
    //Read user entry
    while(Serial.available() && count < SIZE)
    {
      temp[count] = Serial.read();
      count++;
    }
    
    //Clear out any remaining characters entered by user
    Serial.flush();
    
    //display entry info
    /*
    Serial.println(count);
    
    
    for(int i = 0; i < count; i++)
    {
       Serial.println(temp[i]); 
    }
    */
    
    //Convert ASCII chars to int values
    for(int i = 0; count > 0; count--)
    {
      //pow() rounds down for values > 2 for i; have to add 1 to get desired value
     if(i < 2)
      addr += (temp[count - 1] - '0') * (uint16_t) pow(10, i);
     else
      addr += (temp[count - 1] - '0') * ( (uint16_t) pow(10, i) + 1 );
      
      i++;
    } 
    
    Serial.print("addr: ");
    Serial.println(addr);
    
    //Verify entry from user is in acceptable range
    if (addr >= 4096)
    {
      Serial.println("Invalid address; must be between 0 - 4095");
      flag = false;
    }
  }
 
  //Reset flag
  flag = false;
  
  /* Get mode */
  while(flag == false)
  {
    flag = true;
    mode = '\0';
    
    Serial.println("Enter mode (r - READ, w - WRITE)");
    
    //Wait for user to enter data
    while(!Serial.available());
    
    count = 0;
    
    //Read user entry
    if(Serial.available())
    {
      mode = Serial.read();
    }
     
    //Clear out any remaining characters entered by user
    Serial.flush(); 
    
    //Make entry lowercase
    if(mode < 'a')
    {
      mode += 32; 
    }
    
    //Verify entry from user is valid
    if(mode != 'r' && mode != 'w')
    {
      Serial.println("Invalid entry; must be either r for READ or w for WRITE");
      flag = false;
    }
  }
  
  //Reset flag
  flag = false;
  
  if(mode == 'w')
  {
    /* Get value to write to EEPROM */
    while(flag == false)
    {
      flag = true;
      wr_val = 0;
      
      Serial.println("Enter value to store (0 - 255)");
      
      //Wait for user to enter data
      while(!Serial.available());
      
      count = 0;
      
      //Read user entry
      while(Serial.available() && count < SIZE)
      {
        temp[count] = Serial.read();
        count++;
      }
      
      //Clear out any remaining characters entered by user
      Serial.flush();
      
      //display entry info
      /*
      Serial.println(count);
      
      for(int i = 0; i < count; i++)
      {
         Serial.println(temp[i]); 
      }
      */
      
      //Convert ASCII chars to int values
      for(int i = 0; count > 0; count--)
      {
        //pow() rounds down for values > 2 for i; have to add 1 to get desired value
       if(i < 2)
        wr_val += (temp[count - 1] - '0') * (uint16_t) pow(10, i);
       else
        wr_val += (temp[count - 1] - '0') * ( (uint16_t) pow(10, i) + 1 );
       
        i++;
      } 
     
      Serial.print("wr_val: ");
      Serial.println(wr_val);
      
      //Verify entry from user is in acceptable range
      if (wr_val >= 256)
      {
        Serial.println("Invalid entry; must be between 0 - 255");
        flag = false;
      }
      else
      {
        // write the value to the appropriate byte of the EEPROM.
        // these values will remain there when the board is
        // turned off.
        EEPROM.write(addr, (uint8_t) wr_val);
        
        delay(40);
      } 
    }
  }
  
  //Reset flag
  flag = false;

  rd_val = EEPROM.read(addr);
  
  //Display value read from EEPROM
  Serial.print(addr);
  Serial.print("\t");
  Serial.print(rd_val, DEC);
  Serial.println();
  
  delay(100);
}
