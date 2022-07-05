#ifndef __UTILS_H
#define __UTILS_H
#include <stdint.h>
#include <Clock_Handle.h>

void up_down_number(uint8_t *number, int8_t max, int8_t min, uint8_t plus);
uint8_t check_leap_year(uint16_t year);
uint8_t get_max_date(uint8_t mon, uint16_t year);

#endif

