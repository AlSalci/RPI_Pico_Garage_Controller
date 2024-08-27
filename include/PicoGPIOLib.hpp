#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include <vector>
#include <iostream>
/*
Author: Alan Salciccioli

Description: This library introduces a set of function to stream line the development of software to be used on
the RPI Pico. Additionally, this library also implements function to allow easy communication with common peripherals such as LCD displays
via the I2C communication protocol
*/


//LCD common commands=================================================
//LCD modes==================
#define COMMAND 0
#define CHARACTER 1
//============================

//LCD dimensions==============
#define MAX_LINES 2
#define MAX_CHARACTERS 16
//============================

//LCD commands=====================
#define clear 0x01 //Clears the display
#define home 0x02 //Sets DDRAM address to 0 and returns display  to original position
#define entry 0x04//Sets cursor move direction
#define display 0x08 //controls backlight of LCD
#define shift 0x10//Moves cursor and shifts display
#define funSet 0x20//Sets Interface data length,number of lines and character font
//=================================

//LCD Flags============================================
//Entry Mode flags----------------------------------
#define ENTRYSHIFT 0x01
#define ENTRYLEFT 0x02
//--------------------------------------------------
//Display and cursor Flags-------------------------------------
#define BLINKON 0x01
#define CURSORON 0x02
#define DISPLAYON 0x04
//--------------------------------------------------
//Cursor Shift Flags-------------------------------------
#define MOVERIGHT 0x04
#define DISPLAYMOVE 0x08
//--------------------------------------------------
//Function Set  Flags-------------------------------------
#define LINES_2 0x08
//--------------------------------------------------
//Backlight  Set  Flags-------------------------------------
#define BACKLIGHT 0x08
//--------------------------------------------------
#define ENABLED 0x04//Determines if LCD should read incomming data
//=======================================================

//====================================================================


//COMMON FUNCTIONS====================================================================

/*
@ Description: This function initilizes the gpio pins present in the vector
@ to act as inputs or outputs in depending on the value of bool inputs. Inputs will be idle low
@ params vector<int> pins-> Vector containing pins to be used as GPIOs
@bool inputs -> Bool to determine if GPIOs should be inputs or outputs
@ return void
*/
void gpio_pin_init(std::vector<int> pins,bool inputs);

/*
@ Description: This function initilizes the I2C protocol and pins 
@ params i2c_inst_t *i2c -> I2C instance
@ int sda_pin -> Pin to be used as Data bus
@int scl_pin -> Pin to be used as Clock Line
@int budrate -> Sets Baudrate 
@ return void
*/
void gpio_I2C_init(i2c_inst_t *i2c,int sda_pin,int scl_pin,int budrate);


/*
@ Description: This function determines if an address is valid for the I2C protocal. Only to be used within busScan Function
@ params uint8_t addr -> Address to Check
@ return void
*/
bool reserved_addr(uint8_t addr);

/*
@ Description: Performs bus scan on I2C data line to determine connected devices. Prints out results in terminal
@params i2c_inst_t -> Instance of I2C to perform scan
@ return void
*/
void busScan(i2c_inst_t *i2c);


/*
@ Description: Used to read data from a register given the address of the device
@params: i2c_inst_t *i2c -> Instance of I2C
@ uint8_t addr -> Address of device to read from
@uint8_t reg -> register to read from
@size_t len -> Number of bytes expected to recieve back
@uint8_t &ref -> Reference to array to store read data in
@ return void
*/
template <size_t length>
void i2cReadByte(i2c_inst_t *i2c,uint8_t addr, uint8_t reg,size_t len, uint8_t (&ref)[length])
{
    
    i2c_write_blocking(i2c,addr,&reg,1,true);
    i2c_read_blocking(i2c, addr, ref, len,false);

}
//===========================================================

//LCD FUNCTIONS=============================================

/*
@ Description: Sends command or character to connected LCD device via I2C
@params: i2c_inst_t *i2c -> Instance of I2C
@ uint8_t addr -> Address of device LCD device
@uint8_t data -> Command/Character to be sent to the LCD
@int mode -> Determines if data should be considered a character or a command
@ return void
*/
void lcdSendCommand(i2c_inst_t *i2c, uint8_t addr,uint8_t data,int mode);

/*
@ Description: Used to send exactly 1 byte of data to device via I2C. Useful for LCD communication
@params: i2c_inst_t *i2c -> Instance of I2C
@ uint8_t addr -> Address of device LCD device
@uint8_t data -> Data to be sent to the device
@ return void
*/
void writeByte(i2c_inst_t *i2c, uint8_t addr, uint8_t data);


/*
@ Description: Toggles enable pin on LCD while sending data in order to split into 4 bits
@params: i2c_inst_t *i2c -> Instance of I2C
@ uint8_t addr -> Address of device LCD device
@uint8_t data -> Data to send to LCD
@ return void
*/
void lcdToggleEnable(i2c_inst_t *i2c,uint8_t addr,uint8_t data);


/*
@ Description: Initilizes LCD to be used by I2C data line
@params: i2c_inst_t *i2c -> Instance of I2C
@ uint8_t addr -> Address of device LCD device
@uint8_t data -> Data to send to LCD
@ return void
*/
void lcd_init(i2c_inst_t *i2c,uint8_t addr);

/*
@ Description: Sets cursor location on the LCD
@params: i2c_inst_t *i2c -> Instance of I2C
@ uint8_t addr -> Address of device LCD device
@int line -> Cursor line position
@int pos -> Cursor column position
@ return void
*/
void lcdSetCursor(i2c_inst_t *i2c,uint8_t addr,int line,int pos);


/*
@ Description: Clears LCD Display
@params: i2c_inst_t *i2c -> Instance of I2C
@ uint8_t addr -> Address of device LCD device
@ return void
*/
void lcdClear(i2c_inst_t *i2c,uint8_t addr);

/*
@ Description: Clears Line on LCD
@params: i2c_inst_t *i2c -> Instance of I2C
@ uint8_t addr -> Address of device LCD device
@ int line -> Clears line on LCD
@ return void
*/
void lcdClearLine(i2c_inst_t *i2c,uint8_t addr,int line);

/*
@ Description: Displays String onto LCD Display
@params: i2c_inst_t *i2c -> Instance of I2C
@ uint8_t addr -> Address of device LCD device
@ std::string arg-> String to be printed
@ int line -> Line to Print on
@ int pos -> Column to start At
@ return void
*/
void lcdPrintString(i2c_inst_t *i2c,uint8_t addr,std::string arg,int line,int pos);
//===========================================================

