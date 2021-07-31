#include "key_app.h"


/* Private variables ---------------------------------------------------------*/
static uint8_t  test_key2_cnt = 0;	

/* global variables ----------------------------------------------------------*/
uint8_t key_no_action_start = 0;
uint8_t key_no_action_cnt = 0;

uint8_t  test_step = 0;								//测试时的步骤   0：显示软件版本后全屏    1：全屏后测试按键 和 喇叭电机
uint8_t  test_key_num = 0;							//测试按键时按键的值



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_process
    @函数功能：   每个按键的功能
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void key_process(void)
{
    if (lcd_mode != mode_9v_power_on_wait)
    {
        bsp_KeyScan10ms();
    	ucKeyCode = bsp_GetKey();	/* 读取键值, 无键按下时返回 KEY_NONE = 0 */
        switch (ucKeyCode)
        {
            case KEY_12_24H_SET_SHORT_UP:
                key_12_24h_short_up_handler();
                break;
            case KEY_12_24H_SET_LONG_FIRST:
                key_12_24h_long_press_handler();
                break;
                         
            case KEY_UP_SHORT_PRESS:
            case KEY_UP_LONG_FIRST:
            case KEY_UP_LONG_REPEAT:    
                key_up_handler();
                break;

            case KEY_DOWN_SHORT_PRESS:
            case KEY_DOWN_LONG_FIRST:
            case KEY_DOWN_LONG_REPEAT:
                key_down_handler();
                break;
            
            case KEY_ALARM1_SHORT_UP:
                key_al1_short_up_handler();
                break;
            case KEY_ALARM1_LONG_FIRST:
                key_al1_long_press_handler();
                break;
            
            case KEY_ALARM2_SHORT_UP:
                key_al2_short_up_handler();
                break;
            case KEY_ALARM2_LONG_FIRST:
                key_al2_long_press_handler();
                break;

            case KEY_SNOOZE_SHORT_UP:
                key_snooze_short_up_handler();
                break;
            case KEY_SNOOZE_LONG_FIRST:
                key_snooze_long_handler();
                break;
            
            case KEY_UP_DOWM_DOUBLE_LONG_FIRST:
	            key_up_down_double_handler();
                break;

            case KEY_ALARM_SET_SHORT_PRESS:             //短按取消闹铃和贪睡
                key_alarm_set_short_press_handler();
                break;   
            case KEY_ALARM_SET_SHORT_UP:                //音量调节模式下返回主界面
                key_alarm_set_short_up_handler();
                break;
            case KEY_ALARM_SET_LONG_FIRST:              //长按进入音量调节模式
                key_alarm_set_long_press_handler();
                break;
            
            default:break;
        }
    }
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_no_action_test
    @函数功能：   按键无操作是对应的功能
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void key_no_action_test(void)
{
	if(key_no_action_start)
	{
		if(!key_no_action_cnt) 																			//按键无动作时间到
		{			
			key_no_action_start = 0;
			#ifdef SHOW_ALARM_TIME_EN
			if(lcd_mode == mode_set_clock || lcd_mode == mode_set_snooze_time || lcd_mode == mode_show_alarm_time)
			#else
			if(lcd_mode == mode_set_clock || lcd_mode == mode_set_snooze_time)
			#endif
			{
				lcd_mode = mode_clock;
			}	
			else if(lcd_mode == mode_set_alarm)
			{				
				/**** 不保存设置的闹钟数据 *****/
				//rtc_alarm[flag_current_seting_alarm].a_ringtime = rtc_alarm[ALARM_TEMP].a_ringtime;
				rtc_alarm[flag_current_seting_alarm].a_day 			= rtc_alarm[ALARM_TEMP].a_day;
				rtc_alarm[flag_current_seting_alarm].a_hour 		= rtc_alarm[ALARM_TEMP].a_hour;
				rtc_alarm[flag_current_seting_alarm].a_minute 	= rtc_alarm[ALARM_TEMP].a_minute;
				rtc_alarm[flag_current_seting_alarm].a_second 	= rtc_alarm[ALARM_TEMP].a_second;
				lcd_mode = mode_clock;
			}
			else if(lcd_mode == mode_test)
			{
				test_step = 1;
				test_key2_cnt = 0;
				test_key_num = 0;
				lcd_show_all();
			}
		}
	}		
}





/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   test_time_set_step
    @函数功能：   设置时钟状态下，调整设置的location 年月日，时分
    @入口参数：   无
    @出口参数：   无
    @备注说明：   设置时钟模式下调用
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void test_time_set_step(void)
{
	key_no_action_start = 0;
	lcd_clear();
	if(set_location == set_clock_day)								//“日”设置完成后，又有一次按键
	{									//更新日历时间		
		lcd_mode = mode_clock;												//直接进入 时钟模式
		return;
	}
	else 
	{		
		set_location ++;															//顺序切换到 设置其他		
	}			
	
	key_no_action_cnt = SETING_FLASH_TIMER;
	key_no_action_start = 1;												//按键无动作计时开始
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_12_24h_short_up_handler
    @函数功能：   短按key_12_24h的功能
    @入口参数：   无
    @出口参数：   无
    @备注说明：   mode_clock模式下，切换12h24h，mode_set_clock调整设置的location
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_12_24h_short_up_handler(void)
{
    if (lcd_mode == mode_test)
    {
    	if (test_step == 1)
    	{
    		test_key2_cnt = 0;
    		lcd_clear();
    		test_key_num = 2;								
    	}
    }
    else if (lcd_mode == mode_clock)												//如果在 时钟模式 
    {
        flag_24hours_en = !flag_24hours_en;                             //切换24h和12h制度
    	alarm_stop_key_function(KEY_12_24H_SET_SHORT_PRESS);
    }
    else if (lcd_mode == mode_set_clock)								//如果在 时钟设置模式
    {
    	test_time_set_step();							
    }
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_12_24h_long_press_handler
    @函数功能：   长按key_12_24h的功能
    @入口参数：   无
    @出口参数：   无
    @备注说明：   mode_clock模式下，长按进入mode_set_clock
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_12_24h_long_press_handler(void)
{
    if(lcd_mode == mode_clock)												//在 时钟模式 下长按键
	{
		lcd_clear();
		lcd_mode = mode_set_clock;											//转换到 时钟设置 模式
		flag_set_flash_en = 1;													//设置位允许闪烁
		set_location = set_clock_hour;									//先设置 时
		key_no_action_cnt = SETING_FLASH_TIMER;
		key_no_action_start = 1;												//按键无动作计时开始								
	}
}





/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_up_handler
    @函数功能：   key_up的功能
    @入口参数：   无
    @出口参数：   无
    @备注说明：   mode_clock模式下，长按进入mode_set_clock
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_up_handler(void)
{
    
	if(lcd_mode == mode_test)
	{
        if(test_step == 1)
        {
            test_key2_cnt = 0;
            lcd_clear();
			test_key_num = 5;
        }
	}
	else if(lcd_mode == mode_clock)												//如果在 时钟模式
	{
		alarm_stop_key_function(KEY_UP_SHORT_PRESS);
	}
	else if(lcd_mode == mode_volum_adjust)
	{
        if (g_alarm_ring_index <= 3)
        {
             g_alarm_ring_index++;
        }
        if (g_alarm_ring_index > 3)
        {
            g_alarm_ring_index = 0;
        }
	}
   

	else	
	{
		#ifdef SHOW_ALARM_TIME_EN
		if(lcd_mode != mode_show_alarm_time)
		#endif
		{
			key_no_action_start = 0;	
			flag_set_flash_en = 0;												//设置时，不允许闪烁		
			set_no_flash_gap = SET_KEY_FLASH_GAP;					//不允许闪烁的时间间隔 600ms					
			switch(lcd_mode)
			{
				case mode_set_clock:		set_clock_add();break;
				case mode_set_alarm:		set_alarm_add();break;
				case mode_set_snooze_time:		
																if(++snooze_time > SNOOZE_TIME_MAX)
																	snooze_time = SNOOZE_TIME_MIN;
																break;
			}
			
			key_no_action_cnt = SETING_FLASH_TIMER;
			key_no_action_start = 1;											//按键无动作计时开始									
		}							
	}
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_down_handler
    @函数功能：   key_dowm的功能
    @入口参数：   无
    @出口参数：   无
    @备注说明：   mode_clock模式下，长按进入mode_set_clock
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_down_handler(void)
{
	if(lcd_mode == mode_test)
	{
		if(test_step == 1)
		{
			test_key2_cnt = 0;
			lcd_clear();
			test_key_num = 3;								
		}
	}
	else if(lcd_mode == mode_clock)												//如果在 时钟模式
	{
		alarm_stop_key_function(KEY_DOWN_SHORT_PRESS);
	}

    else if(lcd_mode == mode_volum_adjust)
	{
        if (g_alarm_ring_index > 0)
        {
            g_alarm_ring_index--;
        }
        else
        {
            g_alarm_ring_index = 3;
        }
	}
    
	else
	{
		#ifdef SHOW_ALARM_TIME_EN
		if(lcd_mode != mode_show_alarm_time)
		#endif
		{
			key_no_action_start = 0;
			flag_set_flash_en = 0;												//设置时，不允许闪烁		
			set_no_flash_gap = SET_KEY_FLASH_GAP;					//不允许闪烁的时间间隔 600ms					
			switch(lcd_mode)
			{
				case mode_set_clock:		set_clock_sub();break;
				case mode_set_alarm:		set_alarm_sub();break;
				case mode_set_snooze_time:		
																if(--snooze_time < SNOOZE_TIME_MIN)
																	snooze_time = SNOOZE_TIME_MAX;
																break;								
			}				
			
			key_no_action_cnt = SETING_FLASH_TIMER;
			key_no_action_start = 1;													//按键无动作计时开始								
		}
	}
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   test_alarm_set_step
    @函数功能：   设置闹钟模式下调整显示的location,并设置当前闹钟的参数
    @入口参数：   _set_alarm当前正在设置的闹钟
    @出口参数：   无
    @备注说明：   mode_clock模式下，长按进入mode_set_clock
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void test_alarm_set_step(uint8_t _set_alarm)
{
	if(flag_current_seting_alarm == _set_alarm)
	{
		key_no_action_start = 0;
		lcd_clear();	
		//if(set_location == set_alarm_ringtime)					//闹铃时长设置完成后，又有一次按键
		if(set_location == set_alarm_day)					//闹铃时长设置完成后，又有一次按键
		{
			lcd_mode = mode_clock;
			if(flag_current_seting_alarm == ALARM_1) 
				flag_alarm1_en = 1;
			else 
				flag_alarm2_en = 1;
		}																		
		else 
		{
			set_location ++;															//切换到下个设置位
		}												
		key_no_action_cnt = SETING_FLASH_TIMER;	
		key_no_action_start = 1;												//按键无动作计时开始	
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   enter_alarm_seting
    @函数功能：   进入闹钟设置模式
    @入口参数：   无
    @出口参数：   无
    @备注说明：   mode_clock模式下，长按进入mode_set_clock
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void enter_alarm_seting(uint8_t _set_alarm)
{
	key_no_action_start = 0;
	/********* 保存原来闹钟的数据到中间变量 *********/
	flag_current_seting_alarm = _set_alarm;
	//rtc_alarm[ALARM_TEMP].a_ringtime 	= rtc_alarm[flag_current_seting_alarm].a_ringtime;
	rtc_alarm[ALARM_TEMP].a_day 			= rtc_alarm[flag_current_seting_alarm].a_day;
	rtc_alarm[ALARM_TEMP].a_hour 			= rtc_alarm[flag_current_seting_alarm].a_hour;
	rtc_alarm[ALARM_TEMP].a_minute 		= rtc_alarm[flag_current_seting_alarm].a_minute;
	rtc_alarm[ALARM_TEMP].a_second 		= rtc_alarm[flag_current_seting_alarm].a_second;		
	/**************************************************/
	lcd_clear();	
	
	lcd_mode = mode_set_alarm;										 	//转换模式到 设置闹钟模式
//	lcd_symbol_deal(symbol_sec,SHOW);				
	flag_set_flash_en = 1;													//设置为允许闪烁
	set_location = set_alarm_hour;									//先设置小时
	key_no_action_cnt = SETING_FLASH_TIMER;
	key_no_action_start = 1;												//按键无动作计时开始		
}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   alarm_stop_key_function
    @函数功能：   闹钟产生时，按下任意键停止闹钟，如果当前是贪睡模式，则关闭贪睡,闹钟未产生时按下KEY_ALARM1 KEY_ALARM2关闭闹钟
    @入口参数：   _key:当前按下的是哪个键
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void alarm_stop_key_function(uint8_t _key)
{
	if(flag_alarm_timeout)
	{
        flag_alarm_timeout = !flag_alarm_timeout;
    }
		
	else if(flag_enter_snooze)
	{
        flag_enter_snooze = !flag_enter_snooze;
    }		
	else	
	{
		switch(_key)
		{		
			case KEY_ALARM1_SHORT_PRESS:	
							#ifdef SHOW_ALARM_TIME_EN	//---------显示闹钟时间
							if(!flag_alarm1_en)													// 闹钟没有开启
							{
								key_no_action_start = 0;
								flag_alarm1_en = 1;												//则 开启闹钟
								lcd_mode = mode_show_alarm_time;					//转到 显示闹钟时间 模式
								flag_show_which_alarm = ALARM_1;					//同时设置要显示的是 闹钟1
								key_no_action_cnt = SHOW_ALARM_TIMER;	
								key_no_action_start = 1;									//按键无动作计时开始									
							}
							else																				//闹钟 已经开启
							{
								flag_alarm1_en = 0;												//则 关闭闹钟
							}
							#else											//---------只进行闹钟的开关
							flag_alarm1_en = !flag_alarm1_en;						//切换闹钟1的开启状态
							#endif
							break;				
			case KEY_ALARM2_SHORT_PRESS:	
							#ifdef SHOW_ALARM_TIME_EN					
							if(!flag_alarm2_en)
							{
								key_no_action_start = 0;
								flag_alarm2_en = 1;
								lcd_mode = mode_show_alarm_time;
								flag_show_which_alarm = ALARM_2;
								key_no_action_cnt = SHOW_ALARM_TIMER;	
								key_no_action_start = 1;					
							}
							else
							{
								flag_alarm2_en = 0;
							}
							#else													
							flag_alarm2_en = !flag_alarm2_en;
							#endif
							break;
            default:break;                			
		}	
	}
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_al1_short_up_handler
    @函数功能：   key_al1短按对应的功能
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_al1_short_up_handler(void)
{
	if(lcd_mode == mode_test)
	{
		if(test_step == 1)
		{
			test_key2_cnt = 0;
			lcd_clear();
			test_key_num = 7;								
		}
	}
	else if(lcd_mode == mode_clock)												//如果在 时钟模式
	{
		alarm_stop_key_function(KEY_ALARM1_SHORT_PRESS);
	}							
	else if(lcd_mode == mode_set_alarm)								//如果在 设置闹钟模式
	{
		test_alarm_set_step(ALARM_1);
	}
}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_al1_long_press_handler
    @函数功能：   key_al1长按对应的功能
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_al1_long_press_handler(void)
{
    enter_alarm_seting(ALARM_1);

}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_al2_short_up_handler
    @函数功能：   key_al2短按对应的功能
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_al2_short_up_handler(void)
{
	if(lcd_mode == mode_test)
	{
		if(test_step == 1)
		{
			test_key2_cnt = 0;
			lcd_clear();
			test_key_num = 6;								
		}
	}
	else if(lcd_mode == mode_clock)												//如果在 时钟模式
	{
		alarm_stop_key_function(KEY_ALARM2_SHORT_PRESS);
	}
	else if(lcd_mode == mode_set_alarm)								//如果在 设置闹钟模式
	{
		test_alarm_set_step(ALARM_2);
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_al2_long_press_handler
    @函数功能：   key_al2长按对应的功能
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_al2_long_press_handler(void)
{
    enter_alarm_seting(ALARM_2);
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_snooze_short_up_handler
    @函数功能：   key_snooze短按对应的功能
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_snooze_short_up_handler(void)
{
	if(lcd_mode == mode_test)
	{
        switch (test_step)
        {
            case 1:
                lcd_clear();
                test_key_num = 4;
                test_key2_cnt++;
                if(test_key2_cnt == 2)
                {
                    test_key2_cnt = 0;
                    test_step = 2;
                    lcd_clear();
                }	
                break;

            case 2:
                test_step = 0;			
                motor_stop();
                ring_stop();
                lcd_mode = mode_clock;
                break;
                
            default:break;    
                
            
            
        }
        #if 0
		if(test_step == 1)
		{
            lcd_clear();
            test_key_num = 4;
			test_key2_cnt++;
			if(test_key2_cnt == 2)
			{
                test_step = 2;
                lcd_clear();
            }											
		}

        
        else if (test_step == 2)
        {
            test_step = 0;			
			motor_stop();
			ring_stop();
            lcd_mode = mode_clock;
            /*
            if (g_cmd == cmd_ring_stop)
            {
                lcd_mode = mode_clock;
            }
            */
        }
        #endif
	}
	else if(lcd_mode == mode_clock)
	{
		if(flag_alarm_timeout)
		{
			enter_snooze();																	//进入贪睡
		}							
	}
	else if(lcd_mode == mode_set_snooze_time)				//设置贪睡时间时再按贪睡键
	{
		lcd_mode = mode_clock;												//返回时钟模式							
	}
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_snooze_long_handler
    @函数功能：   key_snooze长按对应的功能
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_snooze_long_handler(void)
{
	if(lcd_mode == mode_clock)
	{
		key_no_action_start = 0;
		lcd_clear();
		flag_set_flash_en = 1;													//设置位允许闪烁
		lcd_mode = mode_set_snooze_time;								//进入贪睡时长设置模式
		key_no_action_cnt = SETING_FLASH_TIMER;
		key_no_action_start = 1;												//按键无动作计时开始								
	}	
}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_alarm_set_short_up_handler
    @函数功能：   key_alarm_set短按对应的功能
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_alarm_set_short_up_handler(void)
{
    alarm_stop_key_function(KEY_ALARM_SET_SHORT_UP); //短按取消闹铃和贪睡模式
    if(lcd_mode == mode_test)
	{
		if(test_step == 1)
		{
			test_key2_cnt = 0;
			lcd_clear();
			test_key_num = 1;								
		}
	}
    else if (lcd_mode == mode_volum_adjust)
    {
        lcd_clear();
        lcd_mode = mode_clock;
    }
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_alarm_set_long_press_handler
    @函数功能：   key_alarm_set长按对应的功能
    @出口参数：   无
    @备注说明：   进入闹铃声音设置模式
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_alarm_set_long_press_handler(void)
{
    alarm_stop_key_function(KEY_ALARM_SET_LONG_FIRST); //取消闹铃和贪睡模式
    if (lcd_mode == mode_clock)
    {
        lcd_clear();
        lcd_mode = mode_volum_adjust;          //进入音量调节模式
    }
}

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_alarm_set_short_press_handler
    @函数功能：   key_alarm_set短按对应的功能
    @出口参数：   无
    @备注说明：   进入闹铃声音设置模式
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void key_alarm_set_short_press_handler(void)
{
    alarm_stop_key_function(KEY_ALARM_SET_LONG_FIRST); //取消闹铃和贪睡模式
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_up_down_double_handler
    @函数功能：   双按进入测试模式
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/

static void key_up_down_double_handler(void)
{
    if(lcd_mode != mode_test)
    {
    	lcd_mode = mode_test;
    	test_step = 0;
    	lcd_clear();
    	/*** 显示版本  4.0r***/
    	lcd_num_deal(location_num_2,SHOW,num_1);
    	lcd_symbol_deal(symbol_snooze,SHOW);
    	lcd_num_deal(location_num_3,SHOW,num_0);
    	lcd_num_deal(location_num_4,SHOW,num_r);				
    	key_no_action_cnt = KEY_NO_ACTION_2s;
    	key_no_action_start = 1;												//按键无动作计时开始							
    }

}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_add设置时钟
    @函数功能：   每个按键的功能
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void set_clock_add(void)
{
	if(set_location == set_clock_hour)
	{
		if(++calendar.st_hour == 24)	calendar.st_hour = 0;
	}					
	else if(set_location == set_clock_minute)
	{
		if(++calendar.st_minute == 60)	calendar.st_minute = 0;
	}
	else if(set_location == set_clock_12_24)
	{
		flag_24hours_en = !flag_24hours_en;					//切换小时的格式	
	}	
	else if(set_location == set_clock_year)
	{
		if(++calendar.st_year == 2100)	calendar.st_year = 2099;
	}
	else if(set_location == set_clock_month)
	{
		/* 先判断 当前的天数 是否 超过了下一个要设置月的 最大天数值， 超过则 提前设置好最大天数值*/
		if(is_over_max_days(calendar.st_year,((calendar.st_month + 1 == 13) ? 1 : (calendar.st_month + 1)),calendar.st_day))
		{
			set_max_days_of_month(calendar.st_year,((calendar.st_month + 1 == 13) ? 1 : (calendar.st_month + 1)));
		}		
		if(++calendar.st_month == 13)	calendar.st_month = 1;					
	}
	else if(set_location == set_clock_day)
	{
		calendar.st_day++;
		/*** 超过月份的最大天数 ***/
		if( ((calendar.st_month != 2) && (calendar.st_day > day_of_month[calendar.st_month - 1])) ||
				((calendar.st_month == 2) && (calendar.st_day == 29 + (u8)is_leap_year(calendar.st_year))) )
		{
			calendar.st_day = 1;
//			if(++calendar.st_month == 13)	calendar.st_month = 1;	
		}							
	}
	calendar.st_second = 0;							//每次改 秒 归零	
	updata_calendar();
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_down设置时钟
    @函数功能：   每个按键的功能
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void set_clock_sub(void)
{
	if(set_location == set_clock_hour)
	{
		if(--calendar.st_hour == 255)	calendar.st_hour = 23;
	}					
	else if(set_location == set_clock_minute)
	{
		if(--calendar.st_minute == 255)	calendar.st_minute = 59;		
	}	
	else if(set_location == set_clock_12_24)
	{
		flag_24hours_en = !flag_24hours_en;					//切换小时的格式			
	}	
	else if(set_location == set_clock_year)
	{
		if(--calendar.st_year == 1969)	calendar.st_year = 1970;
	}
	else if(set_location == set_clock_month)
	{
		if(is_over_max_days(calendar.st_year,((calendar.st_month - 1 == 0) ? 12 : (calendar.st_month - 1)),calendar.st_day))
		{
			set_max_days_of_month(calendar.st_year,((calendar.st_month - 1 == 0) ? 12 : (calendar.st_month - 1)));
		}			
		if(--calendar.st_month == 0)	calendar.st_month = 12;					
	}
	else if(set_location == set_clock_day)
	{
		if(--calendar.st_day == 0)
		{
//			if(--calendar.st_month == 0) calendar.st_month = 12;		
			if(calendar.st_month != 2)
			{
				calendar.st_day = day_of_month[calendar.st_month - 1];
			}
			else
			{
				calendar.st_day = 28 + (u8)is_leap_year(calendar.st_year);
			}		
		}				
	}
	calendar.st_second = 0;							//每次改变 秒 归零
	updata_calendar();	
}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_up设置闹钟
    @函数功能：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void set_alarm_add(void)
{
	if(set_location == set_alarm_hour)
	{
		if(++rtc_alarm[flag_current_seting_alarm].a_hour == 24)
			rtc_alarm[flag_current_seting_alarm].a_hour = 0;
	}					
	else if(set_location == set_alarm_minute)
	{
		if(++rtc_alarm[flag_current_seting_alarm].a_minute == 60)
			rtc_alarm[flag_current_seting_alarm].a_minute = 0;
	}
	else if(set_location == set_alarm_day)
	{
		if(++rtc_alarm[flag_current_seting_alarm].a_day == 4)
			rtc_alarm[flag_current_seting_alarm].a_day = 0;			
	}	
}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_down设置闹钟
    @函数功能：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void set_alarm_sub(void)
{
	if(set_location == set_alarm_hour)
	{
		if(--rtc_alarm[flag_current_seting_alarm].a_hour == 255)
			rtc_alarm[flag_current_seting_alarm].a_hour = 23;
	}					
	else if(set_location == set_alarm_minute)
	{
		if(--rtc_alarm[flag_current_seting_alarm].a_minute == 255)
			rtc_alarm[flag_current_seting_alarm].a_minute = 59;
	}
	else if(set_location == set_alarm_day)
	{
		if(--rtc_alarm[flag_current_seting_alarm].a_day == 255)
			rtc_alarm[flag_current_seting_alarm].a_day = 3;	
	}	
}



