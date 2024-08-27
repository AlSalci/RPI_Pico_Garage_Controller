#include "../include/PicoGPIOLib.hpp"
#include "hardware/gpio.h"


//COMMON Functions=======================================================================
void gpio_pin_init(std::vector<int> pins,bool inputs)
{
    for(int count = 0; count < pins.size();count++)
    {
        gpio_init(pins[count]);
        if(inputs)
        {
            gpio_set_dir(pins[count],GPIO_IN);
            gpio_pull_down(pins[count]);
        }
        else {
            gpio_set_dir(pins[count],GPIO_OUT);
        }

    }
}



void gpio_I2C_init(i2c_inst_t *i2c,int sda_pin,int scl_pin,int budrate)
{
    i2c_init(i2c,budrate);
    gpio_set_function(sda_pin,GPIO_FUNC_I2C);
    gpio_set_function(scl_pin,GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);
    bi_decl(bi_2pins_with_func(sda_pin,scl_pin,GPIO_FUNC_I2C));
}


bool reserved_addr(uint8_t addr)
{
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}




void busScan(i2c_inst_t *i2c)
{
std::cout<<"                Bus Scan"<<std::endl;
  std::cout<<"  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F"<<std::endl;
  for(int addr = 0; addr < (1 << 7);++addr)
  {
    if(addr %16 == 0)
    {
     printf("%02x ", addr);
    
    }
  int ret;
  uint8_t data;

  if(reserved_addr(addr))
  {
    ret = PICO_ERROR_GENERIC;

  }
  else
  {
  ret = i2c_read_blocking(i2c,addr,&data,1,false);

  }

if(reserved_addr(addr) == true){
 printf("X");
}
else {

    printf(ret <0 ? "." : "@");


}
   

 printf(addr %16 == 15 ? "\n" : "  ");

}

}

//=========================================================================================


//LCD Commands=======================================

void lcdSendCommand(i2c_inst_t *i2c, uint8_t addr,uint8_t data,int mode)
{
//Sends first nibble of data==========================

  writeByte(i2c,addr,mode | (data & 0xF0)| BACKLIGHT);
  lcdToggleEnable(i2c,addr,mode | (data & 0xF0)| BACKLIGHT);
//====================================================
 

  //Sends Second nibble of data==========================
  writeByte(i2c,addr,mode | ((data <<4)& 0xF0)| BACKLIGHT);
  lcdToggleEnable(i2c,addr,mode | ((data <<4) & 0xF0)| BACKLIGHT);
  //====================================================
  

}


void writeByte(i2c_inst_t *i2c, uint8_t addr, uint8_t data)
{
    i2c_write_blocking(i2c,addr,&data,1,false);
}

void lcdToggleEnable(i2c_inst_t *i2c,uint8_t addr,uint8_t data)
{
  sleep_us(600);
  writeByte(i2c,addr,data | ENABLED);
  
  sleep_us(600);
  writeByte(i2c,addr,data & ~ENABLED);
  
  sleep_us(600);
}


void lcd_init(i2c_inst_t *i2c,uint8_t addr)
{
    lcdSendCommand(i2c,addr,0x03,COMMAND);

    lcdSendCommand(i2c,addr,0x02,COMMAND);
  
    lcdSendCommand(i2c,addr,entry | ENTRYLEFT,COMMAND);
    lcdSendCommand(i2c,addr,funSet | LINES_2,COMMAND);
    lcdSendCommand(i2c,addr,display | DISPLAYON,COMMAND);

}

void lcdSetCursor(i2c_inst_t *i2c,uint8_t addr,int line,int pos)
{
 int val;
  if(line == 0)
  {
    val = 0x80+pos;// Position on line 1

  }
  else {
    val = 0xC0+pos; //Position on line 2
  }
  lcdSendCommand(i2c,addr,val,COMMAND);

}

void lcdClear(i2c_inst_t *i2c,uint8_t addr)
{
    lcdSendCommand(i2c,addr,clear,COMMAND);

}

void lcdClearLine(i2c_inst_t *i2c,uint8_t addr,int line)
{
   
lcdPrintString(i2c,addr,"                              ",line,0);

}

void lcdPrintString(i2c_inst_t *i2c,uint8_t addr,std::string arg,int line,int pos)
{
lcdSetCursor(i2c,addr,line,pos);
for(int count = 0;count< arg.length();count++)
{
    lcdSendCommand(i2c,addr,arg[count],CHARACTER);

}

}
//===================================================

