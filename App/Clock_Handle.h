#ifndef __CLOCK_HANDLE_H
#define __CLOCK_HANDLE_H
#include <stdint.h>
#include "LiquidCrystal_I2C.h"
#include "DS3231.h"
#include "Button.h"
#include "stdio.h"
#include "Utils.h"
#include "main.h"
  
 
//extern I2C_HandleTypeDef hi2c1;
//extern I2C_HandleTypeDef hi2c2;
 

typedef enum
{
NORMAL_STATE,
SETTING_HOUR_STATE,
SETTING_MIN_STATE,
SETTING_SEC_STATE,	
SETTING_DATE_STATE,
SETTING_MON_STATE,
SETTING_YEAR_STATE,
ALARM_HOUR_STATE,
ALARM_MIN_STATE,	 
}ClockState;
 
void clock_init(I2C_HandleTypeDef *hi2c);
 
void adjust_time(uint8_t plus);
 
void clock_handle(void);
void reset_time_start(void);
void khaibaoDateTime(void);
void adjust_rtc(uint8_t sec,uint8_t min,uint8_t hour,uint8_t day,uint8_t date,uint8_t mon,uint8_t year);
#endif
