#ifndef DS3231_H
#define DS3231_H
#include "main.h"
typedef struct
{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t day;
	uint8_t date;
	uint8_t mon;
	uint8_t year;
	
}DateTime;
//muon thay doi gia tri thi truyen dia chi vao


void rtc_write_reg(uint8_t addr, uint8_t value);
uint8_t rtc_read_reg(uint8_t addr);
void rtc_read_Nreg(uint8_t addr,uint8_t *data,uint8_t count);
void rtc_write_time(DateTime *dt);
void rtc_read_time(DateTime *dt);
uint8_t rtc_read_temp(DateTime *dt);
void rtc_init(I2C_HandleTypeDef *i2c);
uint8_t rtc_read_day_of_week(DateTime *dt);
#endif
