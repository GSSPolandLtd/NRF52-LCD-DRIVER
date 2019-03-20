#include <stdint.h>
#include <string.h>
#include "nrf.h"
#include "boards.h"
#include "nrf_log.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

#define LCD_RS 5
#define LCD_E 6
#define BASE 12
#define LCD_DB0 BASE
#define LCD_DB1 BASE + 1
#define LCD_DB2 BASE + 2
#define LCD_DB3 BASE + 3
#define LCD_DB4 BASE + 4
#define LCD_DB5 BASE + 5
#define LCD_DB6 BASE + 6
#define LCD_DB7 BASE + 7

#define _DISPLAYCONTROL    0x08
#define _DISPLAYON    0x0C
#define _CURSORON     0x0A
#define _BLINKON      0x09

#define _FUNCTIONSETCONTROL 0x30
#define _4BITMODE 0x20
#define _8BITMODE 0x10
#define _2LINEDISPLAY 0x08

#define _CLEARDISPLAY     0x01
#define _RETURNHOME    0x02

#define NUMPIN  8
#define UPPERBITS 0xF0

uint8_t _displayCmd=0x0F;
uint8_t _functionSetCmd=0x38;

bool _4bitmodeEnabled=false;
enum RSCONTROL {CMD_CTRL,DATA_CTRL};


void init_GPIO();
void init_lcd();
void initializeLCD();
void enablepaulse();
bool enable4bitMode();
void enable2linDisplayMode();
void no2linDisplayMode();
void sendGeneralCmd(uint8_t CMD);

void LCDWrite8bitData(char value[sizeof(char)]);
void LCDWrite8bitCmd(uint8_t value);

void LCDWrite4bitData(unsigned char data);
void LCDWrite4bitCmd(unsigned char cmd);

void clear();
void home();
void cursor();
void noCursor();


void print(char data[sizeof(char)]);
void setCursor(int col,int row);