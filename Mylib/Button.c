#include "Button.h"
__weak void btn_pressing_callback(Button *btn){}
__weak void btn_pressed_short_callback(Button *btn){}
__weak void btn_press_timeout_callback(Button *btn){}
__weak void btn_release_callback(Button *btn){}
__weak void btn_press_timeout_repeat_callback(Button *btn){};
	
	
	
void button_handle(Button *btn)
{
	//--------------------xu li nhieu------------------------- 
	uint8_t sta = HAL_GPIO_ReadPin(btn->port,btn->pin);
	if(sta != btn->btn_filter)
	{
		btn->is_debouncing = 1;
		btn->btn_filter = sta;
		btn->time_debounce = HAL_GetTick();
	}
	//-----------------tin hieu xac lap------------------------
	if(btn->is_debouncing && (HAL_GetTick() - btn->time_debounce >= 15))
	{
		btn->btn_current = btn->btn_filter;
		//----------------xu li nhan nha---------------------
		if(btn->btn_current != btn->btn_last)
		{
			if(btn->btn_current == 0)//nhan phim
			{
				btn->long_press = 1;
				btn->flag_repeat = 1;
				btn->t_start_press = HAL_GetTick();
				btn->t_start_repeat = btn->t_start_press;
				btn_pressing_callback(btn);
			}
			else //nha phim
			{
				if(HAL_GetTick() - btn->t_start_press <= 1000)
				{
					btn_pressed_short_callback(btn);
				}
				btn->long_press = 0;
				btn_release_callback(btn);
				btn->flag_repeat = 0;
			}
			btn->btn_last = btn->btn_current;
		}
		btn->is_debouncing = 0;
	}
//-----------------------xu li nhan giu--------------------------------	
	if((btn->long_press)&& (HAL_GetTick() - btn->t_start_press >= 3000))
	{
		btn_press_timeout_callback(btn);
		btn->long_press = 0;
	}
//-----------------------xu li nhan giu goi lien tuc------------------- 
	if(( btn->flag_repeat) && HAL_GetTick() - btn->t_start_repeat >= 200)
	{
		btn_press_timeout_repeat_callback(btn); 
		btn->t_start_repeat = HAL_GetTick(); 
	}	
}
void button_init(Button *btn,GPIO_TypeDef *port,uint16_t pin)
{
	btn->btn_current = 1;
	btn->btn_last = 1;
	btn->btn_filter = 1;
	btn->time_debounce = 0;
	btn->is_debouncing = 0;
	btn->t_start_press = 0;
	btn->long_press = 0;
	btn->port = port;
	btn->pin = pin;
}
