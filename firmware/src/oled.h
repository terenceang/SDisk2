#ifndef __OLED__
#define __OLED__

#include "config.h"
#include <avr/pgmspace.h>
#include <util/delay.h>

// I2C pins at ATMEGA
#ifndef SDA_PIN
  #define SDA_PIN 5 
  #define SDA_PORT PORTC
#endif

#ifndef SCL_PIN
  #define SCL_PIN 4
  #define SCL_PORT PORTC
#endif

// I2C address of the display
#ifndef SSD1306_ADDRESS
  #define SSD1306_ADDRESS 0x3C
#endif

// size of the display
#define SSD1306_LCDWIDTH      128
#define SSD1306_LCDHEIGHT      64

// Commands for controling the display
#define SSD1306_SETCONTRAST   0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

#define MIN_CONTRAST 0
#define MAX_CONTRAST 255

unsigned char oled_contrast;
unsigned char ssd1306_offset;

void ssd1306_command(int c);
void ssd1306_data(unsigned char c);
void ssd1306_init();
void ssd1306_clear();
void ssd1306_char(unsigned char c);
void ssd1306_string(char *string);
void ssd1306_contrast(unsigned char value);
void ssd1306_screenUp();
void ssd1306_screenDown();
void ssd1306_inverse();
void ssd1306_underline();
void ssd1306_overline();

// these are just to maintain the same function scheme used for the other LCD screens

void ssd1306_gotoxy(unsigned char x, unsigned char y);
void ssd1306_put_i(unsigned int value);
void ssd1306_put_l(unsigned long int value);
void ssd1306_put_p(const prog_char *progmem_s);
void ssd1306_icon(int icon);
void logo();

#endif
