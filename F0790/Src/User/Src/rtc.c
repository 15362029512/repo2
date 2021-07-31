/* Includes ------------------------------------------------------------------*/
#include "rtc.h"
/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/* global variables ----------------------------------------------------------*/

uint8_t flag_minute = 0;
uint8_t	flag_second = 0;

code uint8_t day_of_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};			
code uint8_t table_week[] 	 = {0,3,3,6,1,4,6,2,5,0,3,5}; 							//月修正数据表	 

		
_CALENDAR  			calendar = {2018,1,1,12,0,0};										//2018-1-1 12:0:0	;												
#define year		calendar.st_year
#define month		calendar.st_month
#define day			calendar.st_day
#define hour		calendar.st_hour
#define minute	    calendar.st_minute
#define second	    calendar.st_second
#define week		calendar.st_week

_ALARM		rtc_alarm[ALARM_CNT] = 
						{
							day_1_7,7,0,0,
							day_1_7,7,0,0,
						};

uint8_t			flag_current_alarmed;				//当前闹铃的是哪一个闹钟
uint8_t			flag_current_seting_alarm;	//当前设置的是哪一个闹钟

uint8_t 		flag_24hours_en = 1;				//1：时钟24小时制
uint8_t 		flag_alarm1_en = 0;					//1：闹钟1开启
uint8_t 		flag_alarm2_en = 0;					//1：闹钟2开启						
						
uint8_t 		flag_alarm_style_ring = 0;	//1：闹钟响铃方式：声音开
uint8_t 		flag_alarm_style_shake = 0;	//1：闹钟响铃方式：震动开

#ifdef SHOW_ALARM_TIME_EN
uint8_t			flag_show_which_alarm;			//持续显示哪个闹钟的时间5秒
#endif

uint16_t 		flag_enter_snooze = 0;			//flag_sleep_again = 0;						//1：进入贪睡

uint8_t			flag_current_snooze = 0;		//当前贪睡的是哪一个闹钟   
uint8_t 		snooze_time = 9;						//贪睡时长		单位：分钟

uint8_t 		flag_alarm_timeout = 0;			//1：闹钟时间到，闹铃响	

uint16_t        alarmed_time = 0;						//闹钟响铃经过的时间
uint8_t         alarm_ring_step = 0;				//
uint8_t         alarm_shake_step = 0;
/*********************************************************************************************************************/

/*********************************************************************************************************************/



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   updata_calendar
    @函数功能：   更新日历
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/

void updata_calendar(void)
{
	set_rtc(calendar.st_year,
					calendar.st_month,
					calendar.st_day,
					calendar.st_hour,
					calendar.st_minute,
					calendar.st_second);
	get_calendar();
}

				
/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   rtc_init
    @函数功能：   RTC初始化
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void rtc_init(void)
{

	
	
	RTCON = 0xa2;										//RTC时钟使能，毫秒中断关闭，半秒中断使能，时钟写使能
//	RTCIF = 0XFF;
//	RTMSS = 0;											//3.9ms  默认
	Delay_50us(10);									//RTC使能后必须延时300us再写入时间， 否则写入时间可能无效。

	updata_calendar();							
	INT8EN = 1;											//开启RTC中断		
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   is_leap_year
    @函数功能：   判断是平年还是闰年
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/

uint8_t is_leap_year(uint16_t _year)
{ 
	if (_year % 4 == 0) //必须能被4整除
	{ 
		if (_year % 100 == 0) 
		{ 
			if (_year % 400 == 0)
            {
                return 1;//如果以00结尾,还要能被400整除  
            }   
			else 
            {
                return 0;   
            }
		}
        else 
        {
            return 1;  
        } 
	}
    else
    {
        return 0;  
    } 
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   is_over_max_days
    @函数功能：   判断天数是否超出月份的最大天数值
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
uint8_t is_over_max_days(uint16_t _year,uint8_t _month,uint8_t _day)
{
	if (_month == 4 || _month == 6 || _month == 9 || _month == 11)
	{
		if (_day > 30) 
        {
            return 1;
        }
		else
        {
            return 0;
        }
	} 
	else if (_month == 2)
	{
		if (is_leap_year(_year))
		{
			if (_day > 29) 
            {
                return 1;
            }
			else 
            {
                return 0;
            }
	    }
		else
		{
			if (_day > 28)
            {
                return 1;
            }
			else 
            {
                return 0;
            }
		}
	}
	else
	{
		if (_day > 31) 
        {
            return 1;
        }
		else 
        {
            return 0;
        }
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   set_max_days_of_month
    @函数功能：   设置某一个月的最大天数值 
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void set_max_days_of_month(uint16_t _year,uint8_t _month)
{
	if (_month == 4 || _month == 6 || _month == 9 || _month == 11)
	{
		calendar.st_day = 30;
	}
	else if (_month == 2)
	{
		if (is_leap_year(_year))
		{
			calendar.st_day = 29;
		}
		else
		{
			calendar.st_day = 28;
		}
	}
//	else
//	{
//		calendar.st_day = 31;
//	}	
}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   get_calendar
    @函数功能：   获取日历信息(读取RTC寄存器 到 变量) 包括 年月日 时分秒 星期  
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void get_calendar(void)
{
	uint16_t day_remain = 0;
	uint16_t get_year = 1970;
	uint8_t  get_month = 0;
	
	second = RTCS;		
	minute = RTCM;
	hour = RTCH & 0x1f;														//RTCH低5位表示小时
	week = RTCH >> 5;															//RTCH高3位表示星期
	day_remain = RTCDH ;
	day_remain = (day_remain << 8) | RTCDL;				//获取RTCD寄存器里的天数值
	
	/***************循环判断得到年份********************/
	while(day_remain >= 365)			
	{
		if(is_leap_year(get_year))	//如果是闰年
		{
			if(day_remain >= 366) 		//且天数超过一年
			{
                day_remain -= 366;
            }				
			else 
            {
                break;                              //天数未超过一年，直接退出
            }
		}
		else 
        {
            day_remain -= 365;          //如果是平年且超过一年
        }		
		get_year ++;
	}	
	year = get_year;							//得到年								
	
	/***************循环判断得到 月 日******************/
	while(day_remain >= 28)
	{
		if(is_leap_year(year) && get_month == 1)
		{
			if(day_remain >= 29) 
            {
                day_remain -= 29;
            }
			else 
            {
                break;
            }
		}
		else
		{
			if(day_remain >= day_of_month[get_month]) 
            {
                day_remain -= day_of_month[get_month];
            }
			else 
            {
                break;
            }
		}
		get_month ++;
	}
	month = get_month + 1;			//得到月
	day = day_remain + 1;				//得到日
}

																
/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   get_week
    @函数功能：   输入公历日期得到星期(只允许1901-2099年) 
    @入口参数：   year,month,day：公历年月日 
    @出口参数：   星期号	0 ~ 6   ->  周日 ~ 周六
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static uint8_t get_week(uint16_t _year,uint8_t _month,uint8_t _day)
{	
	uint16_t temp2;
	uint8_t yearH,yearL;
	
	yearH =_year/100;	yearL = _year%100; 
	
	if (yearH > 19) 
    {
        yearL += 100;// 如果为21世纪,年份数加100  
    }
	// 所过闰年数只算1900年之后的  
	temp2 = yearL + yearL / 4;
	temp2 = temp2 % 7; 
	temp2 = temp2 + _day + table_week[_month - 1];
	if (yearL % 4 == 0 && _month < 3) 
    {
        temp2--;
    }
	return(temp2 % 7);
}	


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   set_rtc
    @函数功能：   设置日历信息（将变量中的值写入寄存器）
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void set_rtc(uint16_t s_year,uint8_t s_month,uint8_t s_day,uint8_t s_hour,uint8_t s_minute,uint8_t s_second)
{
	uint16_t rtc_day = 0;		
	uint16_t year_number = 1970;
	uint8_t  month_number = 0;
	uint8_t  s_week = 0;
	/*通过要设置的 年月日 计算要写入 RTCD 寄存器的值*/
	for (; year_number < s_year; year_number++)
	{
		if(is_leap_year(year_number)) 
        {
            rtc_day += 366;
        }
		else 
        {
            rtc_day += 365;
        }     
	}	
//	s_month -= 1;
	for (; month_number < s_month - 1; month_number++)	
	{
		rtc_day += day_of_month[month_number];		
		if(is_leap_year(s_year) && (month_number == 1))	//如果是闰年，第2个月
		{
            rtc_day += 1;											  					//多加一天
        }	
	}
	rtc_day += s_day - 1 ;														//得到总的天数	
	s_week = get_week(s_year,s_month,s_day);					//得到星期号
	if (s_week == 0) 
    {
        s_week = 7;
    }
	/************************************************/
	RTC_W_EN;								//RTC时钟寄存器写使能					
	RTCS = s_second;
	RTCM = s_minute;
	RTCH = (s_week << 5) | s_hour;							//week的低3位存储在RTCH的高3位
																							//hour存储在RTCH的低5位	
	RTCDH = (uint8_t)(rtc_day >> 8);
	RTCDL = (uint8_t)rtc_day;

	Delay_50us(1);
	RTC_W_DIS;							//RTC时钟寄存器写禁止
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   alarm_timeout_opera
    @函数功能：   设置日历信息（将变量中的值写入寄存器）
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void alarm_timeout_opera(uint8_t _alarm)
{
	flag_current_alarmed = _alarm;		//
	flag_alarm_timeout = 1;						//闹钟时间到
	alarmed_time = 0;	
	
	alarm_ring_step = 0;							
	alarm_ring_cycle = 2;

	alarm_shake_step = 0;							
	alarm_shake_cycle = 2;		
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   enter_snooze
    @函数功能：   进入贪睡
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/

void enter_snooze(void)
{
	uint8_t snooze_minute_temp = 0, snooze_hour_temp = 0;
	
	flag_alarm_timeout = !flag_alarm_timeout;   		//关闭闹钟
	flag_enter_snooze = 1;													//进入贪睡
	flag_current_snooze = flag_current_alarmed;			//记录当前睡眠的是哪一个闹钟
//	rtc_alarm[ALARM_SNOOZE].a_day = rtc_alarm[flag_current_alarmed].a_day;		//记录当前睡眠的闹钟的工作日类型
	//设置贪睡闹钟：闹钟时间 = 当前时间 + 贪睡时间 (9分钟)							
	snooze_minute_temp = RTCM + snooze_time;
	snooze_hour_temp = (RTCH & 0x1f);
	if (snooze_minute_temp >= 60)
	{
		snooze_minute_temp -= 60;
		snooze_hour_temp += 1; 
	}
	rtc_alarm[ALARM_SNOOZE].a_hour = snooze_hour_temp;
	rtc_alarm[ALARM_SNOOZE].a_minute = snooze_minute_temp;
	rtc_alarm[ALARM_SNOOZE].a_second = RTCS;	
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   snooze_alarm_test
    @函数功能：   测试贪睡闹钟，每秒测试一次
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void snooze_alarm_test(void)
{
	if(flag_enter_snooze)							//如果进入到 贪睡
	{
		if((RTCH & 0x1f) == rtc_alarm[ALARM_SNOOZE].a_hour && RTCM == rtc_alarm[ALARM_SNOOZE].a_minute && RTCS == rtc_alarm[ALARM_SNOOZE].a_second)
		{
			alarm_timeout_opera(flag_current_snooze);
		}
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   alarm_test
    @函数功能：   闹钟测试，每分测试一次
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void alarm_test(void)
{
	if(lcd_mode == mode_clock)
	{
		//------------------ 闹钟1处理 ------------			
		if(flag_alarm1_en)
		{
			if((RTCH & 0x1f) == rtc_alarm[ALARM_1].a_hour && RTCM == rtc_alarm[ALARM_1].a_minute)
			{
                //alarm_timeout_opera(ALARM_1);

				switch(rtc_alarm[ALARM_1].a_day)
				{
					case day_1_1:																
									flag_alarm1_en = !flag_alarm1_en;		//关闭闹钟
									alarm_timeout_opera(ALARM_1);
									break;
					case day_1_5:	
									if((RTCH >> 5 != 6) && (RTCH >> 5 != 7))
									{
										alarm_timeout_opera(ALARM_1);									
									}
									break;
					case day_1_7:
									alarm_timeout_opera(ALARM_1);		
									break;
					case day_6_7:	
									if((RTCH >> 5 == 6) || (RTCH >> 5 == 7))
									{
										alarm_timeout_opera(ALARM_1);									
									}
									break;
                    default:break;                
				}	
			}
		}	
		//------------------ 闹钟2处理 ------------			
		if(flag_alarm2_en)
		{
			if((RTCH & 0x1f) == rtc_alarm[ALARM_2].a_hour && RTCM == rtc_alarm[ALARM_2].a_minute)
			{
			  //alarm_timeout_opera(ALARM_2);
				switch(rtc_alarm[ALARM_2].a_day)
				{
					case day_1_1:
									flag_alarm2_en = !flag_alarm2_en;		//关闭闹钟
									alarm_timeout_opera(ALARM_2);
									break;
					case day_1_5:	
									if((RTCH >> 5 != 6) && (RTCH >> 5 != 7))
									{
										alarm_timeout_opera(ALARM_2);								
									}
									break;
					case day_1_7:
									alarm_timeout_opera(ALARM_2);
									break;
					case day_6_7:	
									if((RTCH >> 5 == 6) || (RTCH >> 5 == 7))
									{
										alarm_timeout_opera(ALARM_2);								
									}
									break;
                    default:break;                 
				}	
			}
		}	
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   alarmed_time_test
    @函数功能：   测试闹铃响起经过的时间，每秒测试一次
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void alarmed_time_test(void)
{
	//------------------闹钟响铃时间长度处理------------
	if (flag_alarm_timeout)
	{
		if (++alarmed_time >= /*rtc_alarm[flag_current_alarmed].a_ringtime*/ALARM_TIME * 60)
		{
			flag_alarm_timeout = 0;
            if(flag_enter_snooze)
        	{
                flag_enter_snooze = !flag_enter_snooze;
            }	
		}
	}
//	else
//	{
//		alarmed_time = 0;
//	}
}




/*********************************************************************************************************************
功	能：   10ms周期调用
参	数：
返回值：
*********************************************************************************************************************/

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   rtc_alarm_opera
    @函数功能：   10ms周期调用
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void rtc_alarm_opera(void) 
{	
	static uint8_t changed = 0;
    if (lcd_mode != mode_test)
    {
        if (flag_dc_5v)
	    {
    		if (flag_alarm_timeout)		
    		{
    			flag_enter_snooze = 0;								//隐藏贪睡符号
    			if (!changed)													//-------第一次进入闹铃
    			{
    				changed = 1;												//设置标志
    			}	
    			if (flag_alarm_style_shake)						//****闹钟时间到时 震动的操作
    			{
    				alarm_shake();											//周期调用		
    			}
                
    			if (flag_alarm_style_ring)							//****闹钟时间到时 铃声的操作
    			{
    				alarm_tone();												//周期调用
    			}
    		}
    		else /*************** 闹钟完成 ********************/
    		{	
    			if (changed)
    			{
    				changed = 0;
    				motor_stop();    				
    			}           
                ring_stop();
    		}	
        }
    }
}

#if 0
void rtc_alarm_opera(void) 
{	
	static uint8_t changed = 0;
    if (lcd_mode != mode_test)
    {
        spk_cmd_ing = 0;
        alarm_tone();	
        //send_ring_cmd(g_current_ring_vol_level);
        //alarm_tone();	

        /*
        if (flag_dc_5v)
	    {
            alarm_tone();	    			
        }
        */
    }
}
#endif

#if 0
void rtc_alarm_opera(void) 
{	
	static uint8_t changed = 0;
	
	if (lcd_mode != mode_test)
    {
        if (flag_dc_5v)
	    {
	        if (flag_alarm_timeout)		
	        {
                flag_enter_snooze = 0;								//隐藏贪睡符号
    			if (!changed)													//-------第一次进入闹铃
    			{
    				changed = 1;												//设置标志
    			}
                if (flag_alarm_style_shake)						//****闹钟时间到时 震动的操作
    			{
    				 alarm_tone();		
    			}
            }
            if (!flag_alarm_timeout)
            {
                if (changed)
    			{
    				changed = 0;
    				motor_stop();    				
    			}
				
                ring_stop();
            }
		}
	}
}
#endif




/*********************************************************************************************************************
功	能：
参	数：
返回值：
*********************************************************************************************************************/
//void factory_set(void)
//{
//	u8 i;
//	flag_24hours_en = 1;						//24小时制
//	set_rtc(factory.st_year,				//设置出厂时间
//					factory.st_month,
//					factory.st_day,
//					factory.st_hour,
//					factory.st_minute,
//					factory.st_second);			
//	get_calendar();	
//	
//	for(i = 0; i < 2; i++)					//设置闹钟参数
//	{
//		rtc_alarm[i].a_ringtime = 10;			//闹铃时长：10分钟
//		rtc_alarm[i].a_day = day_1_7;			//闹铃日期：每天
//		rtc_alarm[i].a_hour = 7;					//闹钟时： 7
//		rtc_alarm[i].a_minute = 0;
//		rtc_alarm[i].a_second = 0;	
//	}
//	
//	snooze_time = 9;								//贪睡时长： 9分钟	
//}




