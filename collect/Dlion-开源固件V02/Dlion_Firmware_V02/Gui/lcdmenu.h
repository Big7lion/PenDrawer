#ifndef __LCDMENU_H
#define __LCDMENU_H 			 
#include "touch.h"
#include "guix.h"
#include "gui.h"
#include "fontupd.h"
#include "text.h"
#include "piclib.h"
#include "temperature.h"
#include "gcodeplayer.h"
#include "button.h"
#include "filelistbox.h"
#include "gcodeplayer.h"
#include "stepper.h"
#include "flash.h"

#define U8_TYPE 0
#define U16_TYPE 1
#define U32_TYPE 2
#define INT_TYPE 3
#define FLOAT_TYPE 4
#define DOUBLE_TYPE 5


#define FONT_SIZE 16


#define USER_DATA_ADDR   (u32)5242880 //5*1024*1024 

//数值输入结构体.
__packed typedef struct 
{
	void *address;
	u8 type;
	double up_limit;
	double low_limit;
	bool judge;
	u32 store_address;
}_data_input;

#define ICOS_SD_CARD      	0			
#define ICOS_FLASH_DISK		1				
#define ICOS_LOCATION		0				 

extern float heater_0_temp;
extern float bed_temp;


typedef void (*menuFunc_t)();
extern menuFunc_t currentMenu;
extern bool redraw_screen;

void lcd_update(void);
void menu_action_gcode(char* pgcode);

void start_screen (void);
void main_screen (void);
void preheat_screen (void);
void gecodelist_screen (void);
void print_screen (void);

#endif

















