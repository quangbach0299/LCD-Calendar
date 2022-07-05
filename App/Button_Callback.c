#include "Button.h"

#include "Clock_Handle.h"

Button btn_up,btn_ok,btn_down ;
uint8_t flag_start_press = 0;
extern ClockState clock_state;


//------------------button_handle--------------------------
void btn_press_timeout_repeat_callback(Button *btn)
{
	if(btn == &btn_up)
	{
		 adjust_time(1);
		 
	 }
	else if(btn == &btn_down)
	{
		adjust_time(0);
		 
	}
	 reset_time_start();
}


void btn_pressing_callback(Button *btn)
{
 
	if(btn == &btn_up)
	{
		 adjust_time(1);
	 }
	else if(btn == &btn_down)
	{
		adjust_time(0);
	}
 
	reset_time_start();
} 
 
void btn_pressed_short_callback(Button *btn)
{
   if(btn == &btn_ok)
	{
		switch(clock_state)
		{
			case NORMAL_STATE :
				clock_state = SETTING_HOUR_STATE;
				break;
			case SETTING_HOUR_STATE :
				clock_state = SETTING_MIN_STATE;
				break;
			case SETTING_MIN_STATE :
				clock_state = SETTING_DATE_STATE;
				break;
			case SETTING_DATE_STATE:
				clock_state = SETTING_MON_STATE;
				break;
			case SETTING_MON_STATE:
				clock_state = SETTING_YEAR_STATE;
				break;
			case SETTING_YEAR_STATE:
				clock_state = NORMAL_STATE;
				break;
			case ALARM_HOUR_STATE:
				clock_state = ALARM_MIN_STATE;
				break;
			case ALARM_MIN_STATE:
				clock_state = NORMAL_STATE;
				break;
				default:break;
		}
		 
	}
}
 
void btn_press_timeout_callback(Button *btn)
{
	if(btn == &btn_ok)
		{
		clock_state = ALARM_HOUR_STATE; 
		reset_time_start();
		}
}

void button_callback_init()
{
	button_init(&btn_up,GPIOA,GPIO_PIN_0);
	button_init(&btn_ok,GPIOA,GPIO_PIN_1);
	button_init(&btn_down,GPIOA,GPIO_PIN_2);
	
}

void button_callback_handle()
{
	button_handle(&btn_down);
	button_handle(&btn_up);
	button_handle(&btn_ok);
	
}

