/******************************************************************************************
 * File: FlashIC_SST25VF020B.c
 * Author: Tyler DeVoogd
 * Date: 6/14/2013
 * Revision: 1.0
 * 
 * Platform: Arduino IDE 1.0.3
 * 
 * Description: This C file defines the functions used for interfacing the Teensy++ 2.0 
 *				with the Microchip SST25VF020B 2 Mb flash memory IC. The
 *				definitions in this file are used for the sensor skin prototype and do
 *				not encompass the full features of the flash IC; only the features needed
 *				for the sensor skin prototype have been implemented.
*******************************************************************************************/
 
#include "FlashIC_SST25VF020B.h"

FlashICClass FlashIC;

//---------------------------Delay functions------------------------------------------
	/*
	 * Software delay to allow Flash IC time to finish writing
	 */
	inline void FlashICClass::writeDelay(void)
	{
	  delayMicroseconds(HW_WR_DELAY); 
	}
	
	/*
	 * Software delay to allow Flash IC time to finish clearing sector
	 */
	inline void FlashICClass::sectorClearDelay(void)
	{
	  delay(HW_CLR_SECT_DELAY);
	}
	
	/*
	 * Software delay to allow Flash IC time to finish clearing block
	 */
	inline void FlashICClass::blockClearDelay(void)
	{
	  delay(HW_CLR_BLK_DELAY);
	}
	
	/*
	 * Software delay to allow Flash IC time to finish clearing chip
	 */
	inline void FlashICClass::chipClearDelay(void)
	{
	  delay(HW_CLR_CHIP_DELAY);
	}
	
//---------------------------Read functions-------------------------------------------
	/*
	 * Used to initialize reads from a given location on the Flash IC
	 */
	void FlashICClass::memRead(uint32_t addr)
	{
	  //Select flash IC
	  digitalWrite(CE_MEM, LOW);
	  
	  // Send the read operation byte 
	  SPI.transfer(READ);
	  
	  // Send the starting address
	  SPI.transfer((addr & MSB)>>16);
	  SPI.transfer((addr & MIB)>>8);
	  SPI.transfer(addr & LSB);
	}

	/*
	 * Read char array at starting address in Flash IC
	 */
	void FlashICClass::memReadData(char* received_data, uint8_t len, uint32_t addr)
	{
	  //Send initialization bytes for reading
	  memRead(addr);
	  
	  for(int i = 0; i < len; i++)
	  {
		received_data[i] = SPI.transfer(0x00);
	  }
	  
	  digitalWrite(CE_MEM, HIGH);
	}
	
	/*
     * Read status register of the Flash IC
	 */
	byte FlashICClass::memReadStatusReg(void)
	{
		byte out;
		
		digitalWrite(CE_MEM, LOW); 
		SPI.transfer(RDSR);
		out = SPI.transfer(0x00);
		digitalWrite(CE_MEM, HIGH);
		
		return out;
	}

//---------------------------Write functions------------------------------------------	

	/*
	 * Send write enable instruction to Flash IC
	 */
	void FlashICClass::memWriteEnable(void)
	{
	  //Select flash IC
	  digitalWrite(CE_MEM, LOW);
	  
	  //Send write enable byte
	  SPI.transfer(WREN);
	  digitalWrite(CE_MEM, HIGH);
	}

	/*
	 * Send AAI operation byte and starting address to Flash IC
	 */
	void FlashICClass::memWrite(uint32_t addr)
	{
	  memWriteEnable();
	  
	  //memClearStatusReg();
	  
	  digitalWrite(CE_MEM, LOW);
	  // Send the write AAI operation byte 
	  SPI.transfer(WRITE_AAI);
	  
	  // Send the starting address
	  SPI.transfer((addr & MSB)>>16);
	  SPI.transfer((addr & MIB)>>8);
	  SPI.transfer(addr & LSB); 
	}
	
	/*
	 * Write char array to starting address in Flash IC
	 */
	void FlashICClass::memWriteData(char* transfer_data, uint32_t addr)
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
		memWrite(addr);
		
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
		  SPI.transfer( ((addr + (strlen(transfer_data) - 1)) & MSB) >> 16 );
		  SPI.transfer( ((addr + (strlen(transfer_data) - 1)) & MIB) >> 8 );
		  SPI.transfer( (addr + (strlen(transfer_data) - 1)) & LSB);
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
		SPI.transfer( (addr & MSB) >> 16 );
		SPI.transfer( (addr & MIB) >> 8 );
		SPI.transfer( addr & LSB);
		SPI.transfer( transfer_data[strlen(transfer_data) - 1] );
		digitalWrite(CE_MEM, HIGH);
		writeDelay();
	  }
	}
	
	/*
	 * Write char array to starting address in Flash IC
	 */
	void FlashICClass::memWriteData(char* transfer_data, uint8_t len, uint32_t addr)
	{
	  //Exit write function if nothing to write
	  if(len <= 0)
	  {
		return;  
	  }
	  
	  //If the data written contains an even number of bytes
	  else if(len > 1)
	  {
		//Send initialization bytes for writing
		memWrite(addr);
		
		//Write initial byte pair
		SPI.transfer(transfer_data[0]);
		SPI.transfer(transfer_data[1]);
		digitalWrite(CE_MEM, HIGH);
		writeDelay();
		
		//Write the remaining byte pairs, if any
		for(int i = 1; i < (len>>1); i++)
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
		
		if(len%2 != 0)
		{
		  memWriteEnable();
		
		  digitalWrite(CE_MEM, LOW);
		  //Write the last byte 
		  SPI.transfer(BYTE_PROG);
		  SPI.transfer( ((addr + (len - 1)) & MSB) >> 16 );
		  SPI.transfer( ((addr + (len - 1)) & MIB) >> 8 );
		  SPI.transfer( (addr + (len - 1)) & LSB);
		  SPI.transfer( transfer_data[len - 1] );
		  digitalWrite(CE_MEM, HIGH);
		  writeDelay();  
		}
	  }
	  //If only one byte of data to write
	  else if(len == 1)
	  {
		//Enable writing
		memWriteEnable();
		
		digitalWrite(CE_MEM, LOW);
		//Write the last byte 
		SPI.transfer(BYTE_PROG);
		SPI.transfer( (addr & MSB) >> 16 );
		SPI.transfer( (addr & MIB) >> 8 );
		SPI.transfer( addr & LSB);
		SPI.transfer( transfer_data[len - 1] );
		digitalWrite(CE_MEM, HIGH);
		writeDelay();
	  }
	}
	
	/*
	 * Send end write byte to Flash IC
	 */
	void FlashICClass::memWriteEndAAI(void)
	{
	  //Select Flash IC
	  digitalWrite(CE_MEM, LOW);
	  
	  SPI.transfer(WRDI);
	  
	  //Deselect Flash IC
	  digitalWrite(CE_MEM, HIGH);
	}

//---------------------------Address generation functions-----------------------------	
	
	/*
	 * Generate address for specific state information block
	 */
	uint32_t  FlashICClass::memGenerateBlockAddress(byte block)
	{
		return memGenerateBlockAddressWithOffset(block, 0);
	}
	
	/*
	 * Generate address for specific information from state information block
	 */
	uint32_t  FlashICClass::memGenerateBlockAddressWithOffset(byte block, byte info_offset)
	{	
		return (STATE_INFO_BLOCK_ADDR + ( (uint32_t) block * STATE_INFO_BLOCK_SIZE ) + info_offset);
	}
	
//---------------------------Clear functions------------------------------------------

	/*
	 * Set flash IC's status register to 0x00, clearing any
	 * write protection or write enable bits
	 */
	void FlashICClass::memClearStatusReg(void)
	{
	  memWriteEnable();
	  
	  //Select Flash IC
	  digitalWrite(CE_MEM, LOW); 
	  
	  //Write 0x00 to register
	  SPI.transfer(WRSR);
	  SPI.transfer(0x00);
	  
	  //Deselect Flash IC
	  digitalWrite(CE_MEM, HIGH);
	}

	/*
	 * Send clear operation to erase 4 KB sector in Flash IC
	 */
	void FlashICClass::memClearSector(byte sector)
	{ 
	  memWriteEnable();
	  
	  //Select Flash IC
	  digitalWrite(CE_MEM, LOW);
	  
	  SPI.transfer(ERASE_4K);
	  
	  //Uses msb through bit 12 of the address passed
	  SPI.transfer((sector >> 4));
	  SPI.transfer((sector << 4));
	  //Least-significant byte not used; sending all 0s
	  SPI.transfer(0x00);

	  //Deselect Flash IC
	  digitalWrite(CE_MEM, HIGH);
	  
	  //IC needs 25ms to finish clear
	  delay(HW_CLR_SECT_DELAY);
	}

	/*
	 * Send clear operation to erase 32 kB sector in Flash IC
	 */
	void FlashICClass::memClearBlock(byte block)
	{
	  memWriteEnable();
	  
	  //Select Flash IC
	  digitalWrite(CE_MEM, LOW);
	  
	  SPI.transfer(ERASE_32K);
	  
	  //Uses msb through bit 15 of address passed
	  SPI.transfer((block >> 1));
	  SPI.transfer((block << 7));
	  //Least-significant byte not used; sending all 0s
	  SPI.transfer(0x00); 
	  
	  //Deselect Flash IC
	  digitalWrite(CE_MEM, HIGH);
	  
	  //IC needs 25ms to finish clear
	  delay(HW_CLR_BLK_DELAY);
	}
	
	