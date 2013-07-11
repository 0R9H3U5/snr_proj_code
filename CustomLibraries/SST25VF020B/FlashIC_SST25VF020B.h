/******************************************************************************************
 * File: FlashIC_SST25VF020B.h
 * Author: Tyler DeVoogd
 * Date: 6/14/2013
 * Revision: 1.0
 * 
 * Platform: Arduino IDE 1.0.3
 * 
 * Description: This header file defines the macros and declares the functions used for 
 *				interfacing the Teensy++ 2.0 with the Microchip SST25VF020B 2 Mb flash 
 *				memory IC. The functions in this file are used for the sensor skin 
 *				prototype and do not encompass the full features of the flash IC; only 
 *				the features needed for the sensor skin prototype have been implemented.
*******************************************************************************************/
 
#ifndef __MEM_FLASH_IC_H__
	#define __MEM_FLASH_IC_H__

	#include <SPI.h>
	#include <Arduino.h>
	
	//Router info sector and block locations
	#define ROUTER_INFO_SECT	0x00	//Sector for 4 kB sector clear
	#define ROUTER_INFO_BLK		0x00	//Block for 32 kB block clear

	//ssid macros
	#define SSID_ADDR		0x000000
	#define SSID_LEN_MAX    128

	//password macros
	#define PW_ADDR			0x000080
	#define PW_LEN_MAX		128

	//state information block macros
	#define STATE_INFO_BLOCK_ADDR	0x8000 //Beginning address of first block, 32kB
	#define STATE_INFO_BLOCK_BLK    0x01	//Block for 32 kB block clear
	#define STATE_INFO_BLOCK_SIZE   46		//Number of bytes per state information block
	#define STATE_INFO_BLOCK_MAX	255     //Max number of state information blocks allowed in memory
	
	//Size of information inside state information block	
	#define SIZE_NAME			1
	#define SIZE_SNSR_FLAG		2
	#define SIZE_WIFI			1
	#define SIZE_SEQ			5
	#define SIZE_NEXT			5
	#define SIZE_LIGHT			4
	#define SIZE_MIC			10
	#define SIZE_VIB			10
	#define SIZE_LVL			1
	#define SIZE_TEMP			4
	#define SIZE_WAKE			2
	#define SIZE_TIMER			1
	#define SIZE_SMPL			1
	
	//specific information from state block offsets
	#define OFFSET_NAME			0
	#define OFFSET_SNSR_FLAG    OFFSET_NAME + SIZE_NAME
	#define OFFSET_WIFI			OFFSET_SNSR_FLAG + SIZE_SNSR_FLAG
	#define OFFSET_SEQ			OFFSET_WIFI + SIZE_WIFI
	#define OFFSET_NEXT			OFFSET_SEQ + SIZE_SEQ	
	#define OFFSET_LIGHT		OFFSET_NEXT + SIZE_NEXT
	#define OFFSET_MIC			OFFSET_LIGHT + SIZE_LIGHT
	#define OFFSET_VIB			OFFSET_MIC + SIZE_MIC
	#define OFFSET_LVL			OFFSET_VIB + SIZE_VIB
	#define OFFSET_TEMP			OFFSET_LVL + SIZE_LVL
	#define OFFSET_WAKE			OFFSET_TEMP + SIZE_TEMP
	#define OFFSET_TIMER		OFFSET_WAKE	+ SIZE_WAKE
	#define OFFSET_SMPL			OFFSET_TIMER + SIZE_TIMER
	
	//SPI instruction definitions for Flash IC
	#define READ      0x03		//Read (33MHz Max)
	#define WREN      0x06		//Write enable
	#define WRDI      0x04		//Write disable
	#define BYTE_PROG 0x02		//Byte program
	#define WRITE_AAI 0xAD		//Write Auto Address Increment mode
	#define ERASE_4K  0x20		//4kB sector clear
	#define ERASE_32K 0x52		//32 kB block clear
	#define ERASE_64K 0xD8		//64 kB block clear
	#define RDSR      0x05		//Read status register
	#define EWSR      0x50		//Enable writing to status register
	#define WRSR      0x01		//Write status register

	//Pin definitions on the Teensy for the Flash IC using the Arduino IDE
	#define CE_MEM   24			//Chip enable for SPI
	#define HOLD_MEM 25			//Hold pin
	#define WP_MEM   26			//Write protection pin

	//Memory clear time delays, in ms
	#define HW_CLR_CHIP_DELAY	50	//Full chip clear
	#define HW_CLR_BLK_DELAY	25	//Block clear
	#define HW_CLR_SECT_DELAY	25	//Sector clear

	//Memory write time delay, in us
	#define HW_WR_DELAY 		10
	
	//Address byte masks
	#define MSB 0x00FF0000 	//Most-significant address byte
	#define MIB 0x0000FF00	//Middle address byte
	#define LSB 0x000000FF	//Least-significant address byte

	class FlashICClass
	{
	    public:
//---------------------------Delay functions------------------------------------------
		/*
		 * Software delay to allow Flash IC time to finish writing
		 */
		inline static void writeDelay(void);
		
		/*
		 * Software delay to allow Flash IC time to finish clearing sector
		 */
		inline static void sectorClearDelay(void);
		
		/*
		 * Software delay to allow Flash IC time to finish clearing block
		 */
		inline static void blockClearDelay(void);
		
		/*
		 * Software delay to allow Flash IC time to finish clearing chip
		 */
		inline static void chipClearDelay(void);
	
//---------------------------Read functions-------------------------------------------
	
		/*
		 * Used to initialize reads from a given location on the Flash IC
		 */
		void static memRead(uint32_t addr);
		
		/*
		 * Read char array at starting address in Flash IC
		 */
		void static memReadData(char* received_data, uint8_t len, uint32_t addr);
		
		/*
		 * Read status register of the Flash IC
		 */
		byte static memReadStatusReg(void);
		
//---------------------------Write functions------------------------------------------		
		
		/*
		 * Send write enable instruction to Flash IC
		 */
		void static memWriteEnable(void);
		
		/*
		 * Send AAI operation byte and starting address to Flash IC
		 */
		void static memWrite(uint32_t addr);
		
		/*
		 * Write char array to starting address in Flash IC
		 */
		void static memWriteData(char* transfer_data, uint32_t addr);
		
		/*
		 * Write a given number of bytes from char array to starting address in Flash IC
		 */
		void static memWriteData(char* transfer_data, uint8_t len, uint32_t addr);
		
		/*
		 * Send end write byte to Flash IC
		 */
		void static memWriteEndAAI(void);

//---------------------------Address generation functions-----------------------------	
		
		/*
		 * Generate address for specific state information block
		 */
		uint32_t static memGenerateBlockAddress(byte block);
		
		/*
		 * Generate address for specific information from state information block
		 */
		uint32_t static memGenerateBlockAddressWithOffset(byte block, byte info_offset);
//---------------------------Clear functions------------------------------------------	

		/*
		 * Set flash IC's status register to 0x00, clearing any
		 * write protection or write enable bits
		 */
		void static memClearStatusReg(void);

		/*
		 * Send clear operation to erase 4 kB sector in Flash IC
		 */
		void static memClearSector(byte sector);	

		/*
		 * Send clear operation to erase 32 kB sector in Flash IC
		 */
		void static memClearBlock(byte block); 
	};
	
	extern FlashICClass FlashIC;

#endif