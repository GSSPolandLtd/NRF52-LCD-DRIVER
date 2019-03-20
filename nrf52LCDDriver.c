#include <stdint.h>
#include <string.h>
#include "nrf.h"
#include "boards.h"
#include "nrf_log.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf52LCDDriver.h"


void main()
{
    initializeLCD();
    

    print("GSS POLAND");
    setCursor(2,1);
    print("Welcome!!");
}
void initializeLCD(){
    init_GPIO();
    init_lcd();
}
void init_GPIO()
{
   
    nrf_gpio_cfg_output(LCD_RS);
    nrf_gpio_cfg_output(LCD_E);

    nrf_gpio_cfg_output(LCD_DB0);
    nrf_gpio_cfg_output(LCD_DB1);
    nrf_gpio_cfg_output(LCD_DB2);
    nrf_gpio_cfg_output(LCD_DB3);
    nrf_gpio_cfg_output(LCD_DB4);
    nrf_gpio_cfg_output(LCD_DB5);
    nrf_gpio_cfg_output(LCD_DB6);
    nrf_gpio_cfg_output(LCD_DB7);
}
void init_lcd()
{
    enable4bitMode();
    enable2linDisplayMode();
    sendGeneralCmd(0x06);
    sendGeneralCmd(0x0F);
    sendGeneralCmd(0x80);
    clear();
     
}
void setCursor(int col,int row){
 //row from [1-15] , column from [64-72]
 if (row==1){
    col+=64;
 }
 if(_4bitmodeEnabled){
  LCDWrite4bitCmd(0x80|col);
}else{

  LCDWrite8bitCmd(0x80|col);
}

}
void clear(){
if(_4bitmodeEnabled){
  LCDWrite4bitCmd(_CLEARDISPLAY);
  }
  else{
  LCDWrite8bitCmd(_CLEARDISPLAY);
  }
}
void sendGeneralCmd(uint8_t CMD){
if(_4bitmodeEnabled){
  LCDWrite4bitCmd(CMD);
  }
  else{
  LCDWrite8bitCmd(CMD);
  }
}

void home(){
if(_4bitmodeEnabled){
  LCDWrite4bitCmd(_RETURNHOME);
  }
  else{
  LCDWrite8bitCmd(_RETURNHOME);
  }
}

void cursor(){
if(_4bitmodeEnabled){
  _displayCmd |=_CURSORON;
  LCDWrite4bitCmd(_displayCmd);
   }
  else{
  _displayCmd |=_CURSORON;
  LCDWrite8bitCmd(_displayCmd);
  }
}

void noCursor(){
if(_4bitmodeEnabled){
_displayCmd =(_displayCmd ^ _CURSORON)|_DISPLAYCONTROL;
LCDWrite4bitCmd(_displayCmd);
 }
  else{
  _displayCmd =(_displayCmd ^ _CURSORON)|_DISPLAYCONTROL;
  LCDWrite8bitCmd(_displayCmd);
  }
}

void display(){
if(_4bitmodeEnabled){
_displayCmd |=_DISPLAYON;
LCDWrite4bitCmd(_displayCmd);
 }
  else{
  _displayCmd |=_DISPLAYON;
  LCDWrite8bitCmd(_displayCmd);
  }
}

void noDisplay(){
if(_4bitmodeEnabled){
_displayCmd =(_displayCmd ^ _DISPLAYON)|_DISPLAYCONTROL;
LCDWrite4bitCmd(_displayCmd);
 }
  else{
  _displayCmd =(_displayCmd ^ _DISPLAYON)|_DISPLAYCONTROL;
  LCDWrite8bitCmd(_displayCmd);
  }
}

void blink(){
if(_4bitmodeEnabled){
_displayCmd |=_BLINKON;
LCDWrite4bitCmd(_displayCmd);
 }
  else{
  _displayCmd |=_BLINKON;
  LCDWrite8bitCmd(_displayCmd);
  }
}

void noBlink(){
if(_4bitmodeEnabled){
_displayCmd =(_displayCmd ^ _BLINKON)|_DISPLAYCONTROL;
LCDWrite4bitCmd(_displayCmd);
 }
  else{
  _displayCmd =(_displayCmd ^ _BLINKON)|_DISPLAYCONTROL;
  LCDWrite8bitCmd(_displayCmd);
  }
}

bool enable4bitMode(){

  _functionSetCmd &=_4BITMODE;
  LCDWrite4bitCmd(_functionSetCmd);
  _4bitmodeEnabled=true;
  return _4bitmodeEnabled;
}

void enable2linDisplayMode(){
if(_4bitmodeEnabled){
  _functionSetCmd |=_2LINEDISPLAY;
  LCDWrite4bitCmd(_functionSetCmd);
   }
  else{
  _functionSetCmd |=_2LINEDISPLAY;
  LCDWrite8bitCmd(_functionSetCmd);
  }
}

void no2linDisplayMode(){
if(_4bitmodeEnabled){
_functionSetCmd =(_functionSetCmd ^ _2LINEDISPLAY);
LCDWrite4bitCmd(_functionSetCmd);
   }
  else{
_functionSetCmd =(_functionSetCmd ^ _2LINEDISPLAY);
  LCDWrite8bitCmd(_functionSetCmd);
  }
}
void enablepaulse()
{
    nrf_gpio_pin_clear(LCD_E);
    nrf_delay_ms(5);
    nrf_gpio_pin_set(LCD_E);
    nrf_delay_ms(5);
    nrf_gpio_pin_clear(LCD_E);
    nrf_delay_ms(5);
}
/*
*Write commands & Characters 
*/
void print(char data[sizeof(char)]){
if(_4bitmodeEnabled){
  int stringLength = strlen(data);
   for (int ch = 0; ch < stringLength; ch++) {
      LCDWrite4bitData(data[ch]);
   }
 }
  else{
  LCDWrite8bitData(data);
  }


}
void LCDWrite8bitData(char value[sizeof(char)])
{
    nrf_gpio_pin_set(LCD_RS);
    int stringLength = strlen(value);
    for (int ch = 0; ch < stringLength; ch++) {
        for (int i = 0; i < NUMPIN; i++) {
            nrf_gpio_pin_write(BASE + i, (value[ch] >> i) & 0x01);
        }
        enablepaulse();
    }
}

void LCDWrite8bitCmd(uint8_t value)
{
    nrf_gpio_pin_clear(LCD_RS);

    for (int i = 0; i < NUMPIN; i++) {
        nrf_gpio_pin_write(BASE + i, (value >> i) & 0x01);
    }
    enablepaulse();
}

void write4bitData(unsigned char data,unsigned char control)
{
    //nrf_gpio_pin_set(LCD_RS);
    nrf_gpio_pin_write(LCD_RS,control);
    data &=UPPERBITS;
    for (int i = 0; i < NUMPIN; i++) {
        nrf_gpio_pin_write(BASE + i, (data >> i) & 0x01);
    }
    enablepaulse();
 }
void LCDWrite4bitCmd(unsigned char cmd)
{
   write4bitData(cmd & UPPERBITS,CMD_CTRL);//send 0 for command and get upper 4 bit
   write4bitData((cmd << 4),CMD_CTRL);//send 0 for command
   if (cmd < 4){
    nrf_delay_ms(2);
   }
}
void LCDWrite4bitData(unsigned char data)
{
   write4bitData(data & UPPERBITS,DATA_CTRL);//send 0 for command and get upper 4 bit
   write4bitData((data << 4),DATA_CTRL);//send 0 for command
   nrf_delay_ms(2);
}

