/* Includes ------------------------------------------------------------------*/
#include "lcd.h"
/* Private typedef -----------------------------------------------------------*/



/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
code uint8_t num_table[NUM_TABLE_SIZE] = //b7 - b1 : _A - _G
{
	0xfc,		//0  
	0x60,		
	0xda,
	0xf2,
	0x66,
	0xb6,
	0xbe,
	0xe0,
	0xfe,
	0xf6,		//9	
	0x02,		//-
	0x6e,		//H
	0x0a,		//r
};


uint8_t  		lcd_mode = mode_9v_power_on_wait;									//当前LCD 的显示模式
SET_LOCATION	set_location = 0;							//设置模式中 当前要设置的位置
uint8_t			flag_set_flash_en = 0;				// 1：设置位允许闪烁
uint8_t			set_no_flash_gap = 0;					//设置位不允许闪烁的间隔时长
uint8_t			flag_clockdot_show = 1;				// 1：时钟秒点显示				     0：时钟秒点不显示
uint8_t			show_in_bat_5sec = 0;					//在电池供电时，显示时钟 5 秒



/* Private functions ---------------------------------------------------------*/
static void smg1_deal(uint8_t deal,uint8_t num);
static void smg2_deal(uint8_t deal,uint8_t num);
static void smg3_deal(uint8_t deal,uint8_t num);
static void smg4_deal(uint8_t deal,uint8_t num);

/* global variables ----------------------------------------------------------*/




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   lcd_num_deal
    @函数功能：   7段数字处理，显示 或 隐藏具体的数字 
    @入口参数：   where；要处理的位置，取值范围 NUMBER_LOCATION    
                   deal；处理的方式，显示或隐藏  SHOW / HIDE  
                   num ：要显示的数值，取值范围  NUMBER
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void lcd_num_deal(NUMBER_LOCATION which,uint8_t deal,uint8_t num)
{
	switch(which)
	{
		case location_num_1:	smg1_deal(deal,num_table[num]); break;
		case location_num_2:	smg2_deal(deal,num_table[num]); break;
		case location_num_3:	smg3_deal(deal,num_table[num]); break;
		case location_num_4:	smg4_deal(deal,num_table[num]); break;
        default:break;
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   lcd_symbol_deal
    @函数功能：   图标处理 （显示 或 隐藏）
    @入口参数：   which：要处理哪一个符号，取值范围 SYMBOL
                   deal：	SHOW(1) ，显示   HIDE(0) ，不显示
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void lcd_symbol_deal(SYMBOL which,uint8_t deal)
{
	switch(which)
	{
		case symbol_am:	        SYMB_AM_S(deal);break;
		case symbol_bat:		SYMB_BAT(deal);break;
		case symbol_pm:			SYMB_PM(deal);break;
		case symbol_al1:		SYMB_AL1(deal);break;
		case symbol_al2:		SYMB_AL2(deal);break;
		case symbol_snooze:		SYMB_SNOOZE(deal);break;
		case symbol_sec1:		SYMB_SEC1(deal);break;
        case symbol_sec2:       SYMB_SEC2(deal);break;
        case symbol_alarm_out:  LED_SW_CTL(deal);break;
        default:break;
	}        
}







/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   lcd_init
    @函数功能：   lcd_init
    @入口参数：   无
    @出口参数：   无
    @备注说明：   要点亮lcd必须使能背光
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void lcd_init(void)
{
    GPIO_Init(SMG1_A_PIN,OUTPUT);			
	GPIO_Init(SMG1_B_PIN,OUTPUT);	
	GPIO_Init(SMG1_C_PIN,OUTPUT);	
	GPIO_Init(SMG1_D_PIN,OUTPUT);	
	GPIO_Init(SMG1_E_PIN,OUTPUT);	
	GPIO_Init(SMG1_F_PIN,OUTPUT);	
	GPIO_Init(SMG1_G_PIN,OUTPUT);	
	
	GPIO_Init(SMG2_A_PIN,OUTPUT);			
	GPIO_Init(SMG2_B_PIN,OUTPUT);	
	GPIO_Init(SMG2_C_PIN,OUTPUT);	
	GPIO_Init(SMG2_D_PIN,OUTPUT);	
	GPIO_Init(SMG2_E_PIN,OUTPUT);	
	GPIO_Init(SMG2_F_PIN,OUTPUT);	
	GPIO_Init(SMG2_G_PIN,OUTPUT);

	GPIO_Init(SMG3_A_PIN,OUTPUT);			
	GPIO_Init(SMG3_B_PIN,OUTPUT);	
	GPIO_Init(SMG3_C_PIN,OUTPUT);	
	GPIO_Init(SMG3_D_PIN,OUTPUT);	
	GPIO_Init(SMG3_E_PIN,OUTPUT);	
	GPIO_Init(SMG3_F_PIN,OUTPUT);	
	GPIO_Init(SMG3_G_PIN,OUTPUT);	

	GPIO_Init(SMG4_A_PIN,OUTPUT);			
	GPIO_Init(SMG4_B_PIN,OUTPUT);	
	GPIO_Init(SMG4_C_PIN,OUTPUT);	
	GPIO_Init(SMG4_D_PIN,OUTPUT);	
	GPIO_Init(SMG4_E_PIN,OUTPUT);	
	GPIO_Init(SMG4_F_PIN,OUTPUT);	
	GPIO_Init(SMG4_G_PIN,OUTPUT);	
	
	GPIO_Init(SYMB_AM_PIN,OUTPUT);
	GPIO_Init(SYMB_BAT_PIN,OUTPUT);
	GPIO_Init(SYMB_PM_PIN,OUTPUT);
	GPIO_Init(SYMB_AL1_PIN,OUTPUT);
	GPIO_Init(SYMB_AL2_PIN,OUTPUT);
	GPIO_Init(SYMB_SNOOZE_PIN,OUTPUT);
	GPIO_Init(SYMB_SEC1_PIN,OUTPUT);
    GPIO_Init(SYMB_SEC2_PIN,OUTPUT);

    GPIO_Init(LED_SW_CTL_PIN,OUTPUT);    
    lcd_clear();
    
}






/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   lcd_clear
    @函数功能：   lcd_clear
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void lcd_clear(void)
{
	smg1_deal(HIDE,num_table[num_8]);
	smg2_deal(HIDE,num_table[num_8]);
	smg3_deal(HIDE,num_table[num_8]);
	smg4_deal(HIDE,num_table[num_8]);

    SYMB_AM_S(HIDE);	
    SYMB_BAT(HIDE);  
    SYMB_PM(HIDE);	  
    SYMB_AL1(HIDE);	  
    SYMB_AL2(HIDE);	  
    SYMB_SNOOZE(HIDE);
    SYMB_SEC1(HIDE);	
    SYMB_SEC2(HIDE);	

    LED_SW_CTL(HIDE);
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   lcd_show_all
    @函数功能：   lcd_show_all
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void lcd_show_all(void)
{
	smg1_deal(SHOW,num_table[num_8]);
	smg2_deal(SHOW,num_table[num_8]);
	smg3_deal(SHOW,num_table[num_8]);
	smg4_deal(SHOW,num_table[num_8]);

    SYMB_AM_S(SHOW);	
    SYMB_BAT(SHOW);	  
    SYMB_PM(SHOW);	  
    SYMB_AL1(SHOW);	  
    SYMB_AL2(SHOW);	  
    SYMB_SNOOZE(SHOW);
    SYMB_SEC1(SHOW);	
    SYMB_SEC2(SHOW);	

    LED_SW_CTL(HIDE);

}








/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   smg1_deal
    @函数功能：   显示 或 隐藏 smg
    @入口参数：   deal：取值 SHOW(显示)  HIDE(隐藏)  num：	取值 num_table[]，显示时需要显示的数字	
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void smg1_deal(uint8_t deal,uint8_t num)
{
	if(deal == SHOW)				//如果是显示
	{		
		SMG1_A(num & 0x80);			//根据 num 显示 或 不显示 每个笔段
		SMG1_B(num & 0x40);
		SMG1_C(num & 0x20);
		SMG1_D(num & 0x10);
		SMG1_E(num & 0x08);
		SMG1_F(num & 0x04);
		SMG1_G(num & 0x02);	
	}
	else// if(deal == HIDE)		//如果是不显示
	{
		SMG1_A(HIDE);									//直接 不显示 所有笔段
		SMG1_B(HIDE);
		SMG1_C(HIDE);
		SMG1_D(HIDE);
		SMG1_E(HIDE);
		SMG1_F(HIDE);
		SMG1_G(HIDE);	
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   smg2_deal
    @函数功能：   显示 或 隐藏 smg
    @入口参数：   deal：取值 SHOW(显示)  HIDE(隐藏)  num：	取值 num_table[]，显示时需要显示的数字	
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void smg2_deal(uint8_t deal,uint8_t num)
{
	if(deal == SHOW)				//如果是显示
	{
		SMG2_A(num & 0x80);			//根据 num 显示 或 不显示 每个笔段
		SMG2_B(num & 0x40);
		SMG2_C(num & 0x20);
		SMG2_D(num & 0x10);
		SMG2_E(num & 0x08);
		SMG2_F(num & 0x04);
		SMG2_G(num & 0x02);	
	}
	else										//如果是不显示
	{
		SMG2_A(HIDE);									//直接 不显示 所有笔段
		SMG2_B(HIDE);
		SMG2_C(HIDE);
		SMG2_D(HIDE);
		SMG2_E(HIDE);
		SMG2_F(HIDE);
		SMG2_G(HIDE);	
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   smg3_deal
    @函数功能：   显示 或 隐藏 smg
    @入口参数：   deal：取值 SHOW(显示)  HIDE(隐藏)  num：	取值 num_table[]，显示时需要显示的数字	
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void smg3_deal(uint8_t deal,uint8_t num)
{
	if(deal == SHOW)				//如果是显示
	{
		SMG3_A(num & 0x80);			//根据 num 显示 或 不显示 每个笔段
		SMG3_B(num & 0x40);
		SMG3_C(num & 0x20);
		SMG3_D(num & 0x10);
		SMG3_E(num & 0x08);
		SMG3_F(num & 0x04);
		SMG3_G(num & 0x02);	
	}
	else										//如果是不显示
	{
		SMG3_A(HIDE);									//直接 不显示 所有笔段
		SMG3_B(HIDE);
		SMG3_C(HIDE);
		SMG3_D(HIDE);
		SMG3_E(HIDE);
		SMG3_F(HIDE);
		SMG3_G(HIDE);	
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   smg4_deal
    @函数功能：   显示 或 隐藏 smg
    @入口参数：   deal：取值 SHOW(显示)  HIDE(隐藏)  num：	取值 num_table[]，显示时需要显示的数字	
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void smg4_deal(uint8_t deal,uint8_t num)
{
	if(deal == SHOW)				//如果是显示
	{
		SMG4_A(num & 0x80);			//根据 num 显示 或 不显示 每个笔段
		SMG4_B(num & 0x40);
		SMG4_C(num & 0x20);
		SMG4_D(num & 0x10);
		SMG4_E(num & 0x08);
		SMG4_F(num & 0x04);
		SMG4_G(num & 0x02);	
	}
	else										//如果是不显示
	{
		SMG4_A(HIDE);									//直接 不显示 所有笔段
		SMG4_B(HIDE);
		SMG4_C(HIDE);
		SMG4_D(HIDE);
		SMG4_E(HIDE);
		SMG4_F(HIDE);
		SMG4_G(HIDE);	
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   show_year
    @函数功能：   显示年
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void show_year(void)
{
	lcd_num_deal(location_num_1,SHOW,calendar.st_year / 1000);
	lcd_num_deal(location_num_2,SHOW,calendar.st_year % 1000 / 100);
	lcd_num_deal(location_num_3,SHOW,calendar.st_year % 100 / 10);
	lcd_num_deal(location_num_4,SHOW,calendar.st_year % 10);
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   show_month_day
    @函数功能：   显示月日
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void show_month_day(void)
{
	uint8_t temp;

	lcd_symbol_deal(symbol_sec1,SHOW);		
	/**************************************************/
	temp = calendar.st_month / 10;
	lcd_num_deal(location_num_1,(bit)temp,temp);
	lcd_num_deal(location_num_2,SHOW,calendar.st_month % 10);	
	/**************************************************/
	temp = calendar.st_day / 10;
	lcd_num_deal(location_num_3,(bit)temp,temp);
	lcd_num_deal(location_num_4,SHOW,calendar.st_day % 10);	
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   show_hour_minute
    @函数功能：   显示时分
    @入口参数：   time_from ，要显示的数据从哪里来		TIME_FROM	
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void show_hour_minute(TIME_FROM time_from)
{
	uint8_t hour_temp = 0;
	uint8_t minute_temp = 0;	
	uint8_t hour_temp_temp = 0;
	
	uint8_t temp = 0;
	static uint8_t pm = 0;
	static uint8_t am = 0;
	switch(time_from)		
	{
		case from_calendar: 			hour_temp = calendar.st_hour;			
															minute_temp = calendar.st_minute;
															break;
		case from_alarm_1: 				hour_temp = rtc_alarm[ALARM_1].a_hour;
															minute_temp = rtc_alarm[ALARM_1].a_minute;	
															break;
		case from_alarm_2: 				hour_temp	= rtc_alarm[ALARM_2].a_hour;	
															minute_temp = rtc_alarm[ALARM_2].a_minute;
															break;
	}
	/********************* 显示 时 ******************************/
	if(flag_24hours_en)											//如果是24小时制
	{
		hour_temp_temp = hour_temp;
        lcd_symbol_deal(symbol_am,HIDE);							//关闭 am
		lcd_symbol_deal(symbol_pm,HIDE);							//关闭 pm		
	}		
	else																		//如果是12小时制
	{
		if((hour_temp >= 0) && (hour_temp < 12)) 			//am				
		{
			pm = 0;
            am = 1;
			hour_temp_temp = hour_temp;
		}
		else																					//pm
		{
			pm = 1;
            am = 0;
			hour_temp_temp = hour_temp - 12;			
		}
        lcd_symbol_deal(symbol_am,am);								//显示 am
		lcd_symbol_deal(symbol_pm,pm);								//显示 pm
		if(hour_temp_temp == 0) hour_temp_temp = 12;	
	}
	temp = hour_temp_temp / 10;
	lcd_num_deal(location_num_1,(bit)temp,temp);
	lcd_num_deal(location_num_2,SHOW,hour_temp_temp % 10);
	/********************* 显示 分 ******************************/
	lcd_num_deal(location_num_3,SHOW,minute_temp / 10);
	lcd_num_deal(location_num_4,SHOW,minute_temp % 10);
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   show_12_24hours
    @函数功能：   显示24小时制或12小时制
    @入口参数：   time_from ，要显示的数据从哪里来		TIME_FROM	
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/

void show_12_24hours(void)
{
	if(flag_24hours_en)
	{
		lcd_num_deal(location_num_1,SHOW,num_2);
		lcd_num_deal(location_num_2,SHOW,num_4);		
	}
	else
	{
		lcd_num_deal(location_num_1,SHOW,num_1);
		lcd_num_deal(location_num_2,SHOW,num_2);		
	}
	lcd_num_deal(location_num_3,SHOW,num_H);
	lcd_num_deal(location_num_4,SHOW,num_r);			
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   show_alarm_day_style
    @函数功能：   显示闹铃的重复时间
    @入口参数：   无	
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void show_alarm_day_style(void)
{
	lcd_num_deal(location_num_2,SHOW,num_);	//第2个数字 显示 “-”
	/**************************************************/
	switch(rtc_alarm[flag_current_seting_alarm].a_day)
	{
		case day_1_1:
							lcd_num_deal(location_num_1,SHOW,num_1);	//第1个数字 显示 “1”
							lcd_num_deal(location_num_3,SHOW,num_1);	//第3个数字 显示 “1”
							break;
		case day_1_5:
							lcd_num_deal(location_num_1,SHOW,num_1);	//第1个数字 显示 “1”
							lcd_num_deal(location_num_3,SHOW,num_5);	//第3个数字 显示 “5”
							break;
		case day_1_7:
							lcd_num_deal(location_num_1,SHOW,num_1);	//第1个数字 显示 “1”
							lcd_num_deal(location_num_3,SHOW,num_7);	//第3个数字 显示 “7”
							break;
		case day_6_7:
							lcd_num_deal(location_num_1,SHOW,num_6);	//第1个数字 显示 “6”
							lcd_num_deal(location_num_3,SHOW,num_7);	//第3个数字 显示 “7”
							break;
        default:break;                    
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   lcd_clock
    @函数功能：   走时界面
    @入口参数：   无	
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void lcd_clock(void)
{
	show_hour_minute(from_calendar);										//显示时钟						
	lcd_symbol_deal(symbol_sec1,SHOW);			                            //秒点闪烁
	lcd_symbol_deal(symbol_sec2,SHOW);			                            //秒点闪烁
	/************************************************************/
	if(flag_alarm_timeout || flag_enter_snooze)
	{
		if(flag_current_alarmed == ALARM_1)
		{			
			lcd_symbol_deal(symbol_al1,flag_clockdot_show);	//--- 对应闹钟符号闪烁 ---
		}
		else
		{			
			lcd_symbol_deal(symbol_al2,flag_clockdot_show);	
		}
	}
    else
    {
        lcd_symbol_deal(symbol_al1,flag_alarm1_en);					            //第1个闹钟符号处理
        lcd_symbol_deal(symbol_al2,flag_alarm2_en);					            //第2个闹钟符号处理
    }

    if (g_led_alarm_enable)
    {
        lcd_symbol_deal(symbol_alarm_out,(flag_clockdot_show && flag_alarm_timeout)); //flag_alarm_timeout == 1闹铃灯闪烁
    }
    else
    {
        lcd_symbol_deal(symbol_alarm_out,HIDE);
    }
}





/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   lcd_set_clock
    @函数功能：   时钟设置界面
    @入口参数：   无	
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void lcd_set_clock(void)
{
	if(flag_set_flash_en && !flag_clockdot_show)						//在允许设置位闪烁 且 时钟秒点不显示 时，设置位也不显示		
	{
		switch(set_location)
		{
			case set_clock_hour:
				lcd_num_deal(location_num_1,HIDE,0);
				lcd_num_deal(location_num_2,HIDE,0);							//隐藏 时 		
				break;
			case set_clock_minute: 
				lcd_num_deal(location_num_3,HIDE,0);	
				lcd_num_deal(location_num_4,HIDE,0);							//隐藏 分
				break;			
			case set_clock_year: 
			case set_clock_12_24:
				lcd_num_deal(location_num_1,HIDE,0);
				lcd_num_deal(location_num_2,HIDE,0);
				lcd_num_deal(location_num_3,HIDE,0);
				lcd_num_deal(location_num_4,HIDE,0);							//隐藏 年		
				break;
			case set_clock_month: 
				lcd_num_deal(location_num_1,HIDE,0);
				lcd_num_deal(location_num_2,HIDE,0);							//隐藏 月	
				break;	
			case set_clock_day: 
				lcd_num_deal(location_num_3,HIDE,0);
				lcd_num_deal(location_num_4,HIDE,0);							//隐藏 日	
				break;							
		}	
	}
	else
	{
		if(set_location < set_clock_12_24)										//如果是设置 时分
		{
			lcd_symbol_deal(symbol_sec1,SHOW);
            lcd_symbol_deal(symbol_sec2,SHOW);
            show_hour_minute(from_calendar);										//显示 时分
		}
		else if(set_location == set_clock_12_24)
		{
			show_12_24hours();
		}
		else if(set_location == set_clock_year)								//如果是设置 年
		{
			show_year();
		}	
		else																									//如果是设置 月日
		{
			show_month_day();
		}	
	}
}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   lcd_set_alarm
    @函数功能：   设置闹钟界面
    @入口参数：   无	
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void lcd_set_alarm(void)
{	
	if(flag_set_flash_en && !flag_clockdot_show)						//在允许设置位闪烁 且 时钟秒点不显示 时，设置位也不显示	
	{
		switch(set_location)
		{
			case set_alarm_hour:
				lcd_num_deal(location_num_1,HIDE,0);
				lcd_num_deal(location_num_2,HIDE,0);							//隐藏 时 	
				break;
			case set_alarm_minute:
				lcd_num_deal(location_num_3,HIDE,0);	
				lcd_num_deal(location_num_4,HIDE,0);							//隐藏 分
				break;
			case set_alarm_day:
				lcd_num_deal(location_num_1,HIDE,0);
				lcd_num_deal(location_num_2,HIDE,0);							//隐藏 
				lcd_num_deal(location_num_3,HIDE,0);	
				break;
//			case set_alarm_ringtime:
//				lcd_num_deal(location_num_2,HIDE,0);							//隐藏 
//				lcd_num_deal(location_num_3,HIDE,0);	
//				break;
            default:break;
		}		
	}
	else
	{
		if(set_location < set_alarm_day)											//---如果是设置时分
		{
			lcd_symbol_deal(symbol_sec1,SHOW);
            lcd_symbol_deal(symbol_sec2,SHOW);
//			if(flag_current_seting_alarm == ALARM_1)
//				show_hour_minute(from_alarm_1);										//显示 时分
//			else
//				show_hour_minute(from_alarm_2);										//显示 时分			
			show_hour_minute((uint8_t)flag_current_seting_alarm + 1);
		}
		else if(set_location == set_alarm_day)
		{
			show_alarm_day_style();
		}
//		else
//		{
//			show_alarm_ringtime();
//		}	
	}
	/******* 闹钟指示符号不随 设置位 闪烁 ，自己单独闪烁 *******/
	if(flag_current_seting_alarm == ALARM_1)
	{
        lcd_symbol_deal(symbol_al1,flag_clockdot_show);
    }
		
	else
	{
        lcd_symbol_deal(symbol_al2,flag_clockdot_show);
    }
		
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   lcd_set_snooze_time
    @函数功能：   设置贪睡时长界面
    @入口参数：   无	
    @出口参数：   无
    @备注说明：   无 
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void lcd_set_snooze_time(void)
{
	u8 temp = 0;

	if(flag_set_flash_en && !flag_clockdot_show)						//在允许设置位闪烁 且 时钟秒点不显示 时，设置位也不显示
	{
		lcd_num_deal(location_num_2,HIDE,0);
		lcd_num_deal(location_num_3,HIDE,0);		
	}
	else
	{
		temp = snooze_time / 10;
		lcd_num_deal(location_num_2,(bit)temp,temp);
		lcd_num_deal(location_num_3,SHOW,snooze_time % 10);		
	}
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   lcd_test
    @函数功能：   lcd测试模式
    @入口参数：   无	
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void lcd_test(void)
{
	if(test_step == 1 && test_key_num != 0)
	{
		lcd_num_deal(location_num_2,SHOW,num_H);
		lcd_num_deal(location_num_3,SHOW,test_key_num);	
	}
	else if(test_step == 2)
	{
		lcd_num_deal(location_num_1,SHOW,g_current_motor_level);
		lcd_symbol_deal(symbol_sec1,SHOW);
        lcd_symbol_deal(symbol_sec2,SHOW);
		lcd_num_deal(location_num_4,SHOW,g_current_alarm_mode_level);
	}


    if (test_step == 2)
    {
        if (flag_alarm_style_shake)                      //震动的操作
        {
            motor_start(shake_strength[alarm_shake_seted_strength],0);
        }

        if (!flag_alarm_style_shake)
        {
            motor_stop();
        }


        if (flag_alarm_style_ring)							//****闹钟时间到时 铃声的操作
		{
			alarm_tone();												//周期调用
		}  

        if (!flag_alarm_style_ring)
        {
            if (g_cmd != cmd_ring_stop)
            {
                ring_stop();
            }           
        }
    }  
}




uint16_t g_9v_power_on_wait_time = 500; //500ms




void lcd_clock_9v_power_on_wait(void)
{
    if (!g_9v_power_on_wait_time)
    {
#if (BL_CTRL_MOD == BL_PWM_CTRL)
        bl_brightness_set(bl_PWM_duty);    //如果是5V上电则按当前的占空比点亮背光
#elif (BL_CTRL_MOD == BL_IO_CTRL)
        GPIO_Init(BL_CTL_PIN,OUTPUT);
        BL_CTL_GPIO = 1;
#endif
        key_ad_init();
        get_calendar();                    //获取日历 
        lcd_mode = mode_clock;
    }
}   


void lcd_volume_adjust(void)
{
    lcd_num_deal(location_num_3,SHOW,g_alarm_ring_index);	
}


/*********************************************************************************************************************
功	能：
参	数：
返回值：
*********************************************************************************************************************/
void lcd_display(void)
{
    if(flag_dc_5v)	
    {
        switch(lcd_mode)
		{
            case mode_9v_power_on_wait:     lcd_clock_9v_power_on_wait();break;
			case mode_clock: 				lcd_clock();break;
			case mode_set_alarm: 		    lcd_set_alarm();break;
			case mode_set_clock:		    lcd_set_clock();break;
			case mode_set_snooze_time:	    lcd_set_snooze_time();break;           
            case mode_test:					lcd_test();break;
			case mode_volum_adjust:			lcd_volume_adjust();break;											
            default:break;                                                    
		}
    }
}

