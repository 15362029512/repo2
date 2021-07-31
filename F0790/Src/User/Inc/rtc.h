#ifndef _RTC_H_
#define _RTC_H_

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "delay.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	uint16_t st_year;
	uint8_t st_month;
	uint8_t st_day;
	uint8_t st_hour;					
	uint8_t st_minute;
	uint8_t st_second;
	uint8_t st_week;		
}_CALENDAR;


typedef struct
{
	uint8_t a_day;						//闹钟 日期   														//最高位： 闹钟开关标志 【1：闹钟开   0：闹钟关】
	uint8_t a_hour;					//闹钟 时				
	uint8_t a_minute;				//闹钟 分
	uint8_t a_second;				//闹钟 秒
}_ALARM;



typedef enum
{
	day_1_1,						//单日闹钟
	day_1_5,						//工作日闹钟
	day_1_7,						//每日闹钟
	day_6_7,						//周末闹钟
}_ALARM_DAY;


/* Exported macro ------------------------------------------------------------*/
#define ALARM_CNT					4					//闹钟的数量
#define ALARM_1						0					//第一个闹钟		
#define ALARM_2						1					//第二个闹钟
#define ALARM_SNOOZE			    2					//贪睡闹钟
#define ALARM_TEMP				    3					//设置闹钟时的 中间变量

#define RTC_HF		(1<<1)


#define RTC_W_EN		RTCON |= 0x02														//RTC时钟寄存器写使能
#define RTC_W_DIS		RTCON &= 0xfd														//RTC时钟寄存器写禁止


#define F_12HOURS				0x80
#define MODE_12HOUR				1
#define MODE_24HOUR				0


#define ALARM_TIME        10    //单位：分钟




#define ALARM_TIME_MIN		1					//闹铃时长最小值，单位：分钟
#define ALARM_TIME_MAX		30				//闹铃时长最大值，单位：分钟

#define SNOOZE_TIME_MIN		1					//贪睡时长最小值，单位：分钟
#define SNOOZE_TIME_MAX		15//60				//贪睡时长最大值，单位：分钟

#define SNOOZE_HIDE_TIME		1				//贪睡符号闪烁间隔时间，单位：半秒 500ms	
#define SNOOZE_SHOW_TIME		2

#define BAT_SHOW_TIME				1
#define BAT_HIDE_TIME				3


#define use_clock_dot								//设置位闪烁 和 贪睡闹钟闪烁 以 flag_clockdot_show 来控制

													
#define DAY_OF_COMMONYEAR		365		//平年一年的天数
#define DAY_OF_LEAPYEAR			366		//闰年一年的天数




/* Exported variables --------------------------------------------------------*/  
extern _CALENDAR    calendar;
extern _ALARM		rtc_alarm[ALARM_CNT];
extern code uint8_t day_of_month[12];


extern uint8_t		flag_current_alarmed;						//当前闹铃的是哪一个闹钟
extern uint8_t		flag_current_seting_alarm;			//当前设置的是哪一个闹钟
extern uint8_t 		flag_24hours_en;
extern uint8_t 		flag_alarm1_en;
extern uint8_t 		flag_alarm2_en;


extern uint8_t 		flag_alarm_style_ring;
extern uint8_t 		flag_alarm_style_shake;


extern uint8_t		flag_show_which_alarm;

extern uint8_t 		flag_alarm_timeout;
extern uint16_t 		flag_enter_snooze;
extern uint8_t		flag_snooze_show;
extern uint8_t		flag_current_snooze;
extern uint8_t 		snooze_time;


extern uint8_t		flag_half_second;
extern uint8_t		flag_minute;
extern uint8_t		flag_second;

extern uint8_t		flag_current_seting_alarm;

extern uint16_t     alarmed_time;
extern uint8_t      alarm_ring_step;
extern uint8_t      alarm_shake_step;



/* Exported functions --------------------------------------------------------*/  
extern void rtc_init(void);
extern void get_calendar(void);
extern void set_rtc(uint16_t s_year,uint8_t s_month,uint8_t s_day,uint8_t s_hour,uint8_t s_minute,uint8_t s_second);
extern uint8_t is_over_max_days(uint16_t _year,uint8_t _month,uint8_t _day);
extern void set_max_days_of_month(uint16_t _year,uint8_t _month);
extern uint8_t is_leap_year(uint16_t _year);
extern void updata_calendar(void);
extern void enter_snooze(void);
extern void alarm_test(void);
extern void snooze_alarm_test(void);
extern void alarmed_time_test(void);
extern void nothing_action_test(void);
extern void rtc_alarm_opera(void);

#endif
