/*------------------------------------

SDISK II LCD Firmware

2010.11.11 by Koichi Nishida
2012.01.26 by Fábio Belavenuto
2015.07.02 by Alexandre Suaide

-------------------------------------
*/

/*
2015.07.02 by Alexandre Suaide
Added support for SDHC cards and subdirectories
Removed DSK to NIC conversion
FAT16 and FAT32 disks should have at least 64 blocks per cluster
*/

/*
2012.01.26 by Fábio Belavenuto
Added support for image exchange using a button added in the Brazilian version by Victor Trucco
Added support for a 16x2 LCD
*/

/*
This is a part of the firmware for DISK II emulator by Nishida Radio.

Copyright (C) 2010 Koichi NISHIDA
email to Koichi NISHIDA: tulip-house@msf.biglobe.ne.jp

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "config.h"

#ifndef __SDISK2__
#define __SDISK2__


#define VER "5.5"
#define YEAR "(2022)"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "string.h"

#include "oled.h"
unsigned char oled_contrast;
unsigned char ssd1306_offset;

#include "SPI_routines.h"
#include "SD_routines.h"
#include "FAT32.h"

#define MAXNIC           4

#define enter_is_pressed() bit_is_set(ENTER_PORT,ENTER_BIT)
#define down_is_pressed()  bit_is_set(DOWN_PORT,DOWN_BIT)
#define up_is_pressed()    bit_is_set(UP_PORT,UP_BIT)
#define diskII_disable()   bit_is_set(DISKII_PIN,DISKII_ENABLE)

#define CHECKSUM_CONFIG 0X01AB02CD

struct Sdisk_config_structure
{
	unsigned long checksum;
	unsigned char sd_card_speed;
	unsigned long directory_of_last_mounted_nic;
	unsigned int  id_of_last_mounted_nic;
	unsigned long directory_of_previous_mounted_nic;
	unsigned int  id_of_previous_mounted_nic;
	unsigned char oled_contrast;
	unsigned char nic_mounted;
	unsigned char EMPTY[64]; // just leave empty for future features
	unsigned long nic_dir[MAXNIC];
	unsigned int  nic_id[MAXNIC];
};

void            init(char splash);
void            display_sd_ejected();
char            init_sd(char splash);
void            verify_status();
void            select_nic();
void            find_previous_nic();
void            swap_nic();
unsigned char   is_a_nic(struct dir_Structure *file);
unsigned char   is_a_dir(struct dir_Structure *file);
unsigned int    mount_nic_image(int file_id, struct dir_Structure* file);
void            create_config_file();
void            writeBackSub();
void            writeBackSub2(unsigned char bn, unsigned char sc, unsigned char track);
void            writeBack();
void            cancelRead();
void            buffClear();
void            set_speed();
void            configButtons();
int             main(void);

void            set_contrast();
void            setup();
void            icons(unsigned char i1, unsigned char i2, unsigned char i3);


#endif
