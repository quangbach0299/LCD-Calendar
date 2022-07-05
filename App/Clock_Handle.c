#include "Clock_Handle.h"

ClockState clock_state = NORMAL_STATE;
LiquidCrystal_I2C lcd;
DateTime date_time;
uint8_t hour_alarm,min_alarm;
uint32_t time_start_press = 0 ;

 

void reset_time_start()
{
	time_start_press = HAL_GetTick();
}

const char arr_day[][4] ={
{"SUN"},
{"MON"},
{"TUE"},
{"WED"},
{"THU"},
{"FRI"},
{"SAT"} 
};//o thu hai bieu thi so phan tu
// khong can ngoac trong moi phan tu
// 4 phan tu vi them ky tu null \n
  
//Chu nhat -> 0
//Thu 2 -> 1
//Thu 3 -> 2 
//Thu 4 -> 3
//Thu 5 -> 4
//Thu 6 -> 5
//Thu 7 -> 6
//Thu 8 -> 7
 
 

void update_alarm_now()
{
	char line2[16];
	sprintf(line2,"%02d:%02d",hour_alarm,min_alarm); 	
	lcd_set_cursor(&lcd,0,5);
	lcd_printf(&lcd,"ALARM");  
	lcd_set_cursor(&lcd,1,5);
	lcd_printf(&lcd,"%s",line2); 		 
}

void update_time_now()
{
	  rtc_read_time(&date_time);
		uint8_t day_of_week = rtc_read_day_of_week(&date_time);
		lcd_set_cursor(&lcd,0,3);
		lcd_printf(&lcd,"%02d:%02d:%02d",date_time.hour,date_time.min,date_time.sec); //02d la de in ra so 0 dau tien
		lcd_set_cursor(&lcd,1,1);
		lcd_printf(&lcd,"%s %02d/%02d/20%02d",arr_day[day_of_week],date_time.date,date_time.mon,date_time.year);
}


void time_update()
{
		static uint32_t t_update = 0; //khoi tao mot lan duy nhat
		if(HAL_GetTick() - t_update >= 500)
		{
		rtc_read_time(&date_time);
		uint8_t day_of_week = rtc_read_day_of_week(&date_time);
		lcd_set_cursor(&lcd,0,3);
		lcd_printf(&lcd,"%02d:%02d:%02d",date_time.hour,date_time.min,date_time.sec); //02d la de in ra so 0 dau tien
		lcd_set_cursor(&lcd,1,1);
		lcd_printf(&lcd,"%s %02d/%02d/20%02d",arr_day[day_of_week],date_time.date,date_time.mon,date_time.year);
		t_update = HAL_GetTick();
		}
}
//---------------------alarm screen---------------------------
void alarm_screen(void)
   {
	static uint32_t t_blink = 0;
	static uint8_t is_show = 1 ;
	char line2[16];
	if(HAL_GetTick() - t_blink >= 200)
	{
	is_show = !is_show ;
	sprintf(line2,"%02d:%02d",hour_alarm,min_alarm);	
	if(is_show)
	{
		switch(clock_state)
		{	
			case ALARM_HOUR_STATE:
				line2[0] = line2[1] = ' ';
				break;
			case ALARM_MIN_STATE:
				line2[3] = line2[4] = ' ';
			default:break;
		}
	}	
	lcd_clear_display(&lcd);
	lcd_set_cursor(&lcd,0,5);
	lcd_printf(&lcd,"ALARM");  
	lcd_set_cursor(&lcd,1,5);
	lcd_printf(&lcd,"%s",line2); 	
	t_blink = HAL_GetTick();
	} 
}
//---------------------check alarm---------------------------------
void check_alarm(void)
{
	if(hour_alarm == date_time.hour && hour_alarm == date_time.min)
	{
		
	}
}
//----------------------blink--------------------------------------
void setting_blink(void)
{
	static uint32_t t_blink = 0;
	static uint8_t is_show = 1 ;
	char line1[16];
	char line2[16];
	uint8_t day_of_week = rtc_read_day_of_week(&date_time);
	sprintf(line1,"%02d:%02d:%02d",date_time.hour,date_time.min,date_time.sec);
	sprintf(line2,"%s %02d/%02d/20%02d",arr_day[day_of_week],date_time.date,date_time.mon,date_time.year);
	
	if(HAL_GetTick() - t_blink >= 200)
	{
		is_show = !is_show ;
		rtc_read_time(&date_time);
		 if(is_show)
		{
		 switch(clock_state)
		 {
			 case SETTING_HOUR_STATE:
				 line1[0] = line1[1] = ' ';
					break;
			case SETTING_MIN_STATE:
				 line1[3]= line1[4]= ' ';
					break;
			 case SETTING_DATE_STATE:
				 line2[4] = line2[5]= ' ';
					break;
			 case SETTING_MON_STATE:
				 line2[7] = line2[8]= ' ';
			   break;
			 case SETTING_YEAR_STATE:
				 line2[10] = line2[11]= line2[12] = line2[13]= ' ';
			 default:break;
		 }
		}	
	  lcd_set_cursor(&lcd,0,3);
		lcd_printf(&lcd,"%s",line1); //02d la de in ra so 0 dau tien
		lcd_set_cursor(&lcd,1,1);
		lcd_printf(&lcd,"%s",line2); 	
		t_blink = HAL_GetTick();
	} 
}

//---------------------handle clock --------------------------------  
void clock_handle(void)
{
	switch(clock_state)
	{
		case NORMAL_STATE:
		{
			time_update();
			check_alarm();
			break;
		}
		case SETTING_HOUR_STATE:
		case SETTING_MIN_STATE:
		case SETTING_DATE_STATE:
		case SETTING_MON_STATE:
		case SETTING_YEAR_STATE:
		{	 
			setting_blink();
		 if(HAL_GetTick() - time_start_press >= 5000)
		 {
			 clock_state = NORMAL_STATE;
		 }
			break;
		}
		case ALARM_HOUR_STATE:
		case ALARM_MIN_STATE:
		{
			alarm_screen();
			if(HAL_GetTick() - time_start_press >= 5000)
		 {
			 clock_state = NORMAL_STATE;
		 } 
			break;
		}
			default:break;
	}
	
}	
 
//--------------------adjust time-----------------

void adjust_time(uint8_t plus)  
{
	switch(clock_state)
		{
			case SETTING_HOUR_STATE:
				up_down_number(&date_time.hour,23,0,plus);
				rtc_write_time(&date_time);	
			  update_time_now();
				 		
				break;
			case SETTING_MIN_STATE:
				up_down_number(&date_time.min,59,0,plus); 
				update_time_now();		
				break;
			case SETTING_DATE_STATE:
			{
				uint8_t max_date = get_max_date(date_time.mon,2000+date_time.year);//khong can thay doi gia tri mon va year nen khong can con tro
			 	up_down_number(&date_time.date,max_date,1,plus);  
				rtc_write_time(&date_time);	
				update_time_now();	
						
				break;
			}
			case SETTING_MON_STATE:
			{
			  up_down_number(&date_time.mon,12,0,plus);  
				 uint8_t max_date = get_max_date(date_time.mon,2000+date_time.year);
				if(date_time.date > max_date)
				{
					date_time.date = max_date;
				}
				rtc_write_time(&date_time);
				rtc_write_time(&date_time);	
				update_time_now();		
				break;
			}
			case SETTING_YEAR_STATE:
				up_down_number(&date_time.year,99,0,plus);  
			  rtc_write_time(&date_time);	
				update_time_now();		
				break;
			case  ALARM_HOUR_STATE:
				up_down_number(&hour_alarm,23,0,plus);
				rtc_write_reg(0x09,hour_alarm);
				rtc_write_time(&date_time);	
				update_alarm_now();
				break;
			case ALARM_MIN_STATE:
				up_down_number(&min_alarm,59,0,plus);
				rtc_write_reg(0x08,min_alarm);	 
			  rtc_write_time(&date_time);	
			  update_alarm_now();
				break;
				default:break; //dong nay quan trong
			//neu ket qua khong trung voi nhung thu co o ben tren thi thoat switch
		 }
}
//------------------------adjust rtc-----------------------------
void adjust_rtc(uint8_t sec,uint8_t min,uint8_t hour,uint8_t day,uint8_t date,uint8_t mon,uint8_t year)
{
 	 date_time.sec = sec;
 	 date_time.min = min;
 	 date_time.hour = hour;
	 date_time.day = day;
	 date_time.date = date;
	 date_time.mon = mon;
	 date_time.year = year;
	 rtc_write_time(&date_time);
}
//-------------------------clock init-------------------

extern I2C_HandleTypeDef hi2c2;//dag khai bao struct giong khai int a,b,c o dang tham chieu neu can thay doi gia tri va tiet kiem bo nho thi truyen dia chi vao trong ham void
//khong can thay doi gia tri thi truyen vao bien hi2c2

void clock_init(I2C_HandleTypeDef *hi2c)
{
	rtc_init(&hi2c2);
	lcd_init(&lcd,hi2c,LCD_ADDR_DEFAULT);//hi2c dang o dang dia chi
	hour_alarm = rtc_read_reg(0x09);
	min_alarm = rtc_read_reg(0x08);	
}
