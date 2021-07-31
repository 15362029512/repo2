#ifndef _BAT_H_
#define _BAT_H_

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "user_type.h"


/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
sbit dc_5v_in	= P4^2;	

/* Exported variables --------------------------------------------------------*/  


/* Exported functions --------------------------------------------------------*/  
#define V_ADC_CH							ADC_CH2				//
#define BAT_ADC_CH						    ADC_CH1 				//  0010 AD_CH[1]	


#define DC_5V_PIN									P42F    //5v检测
#define BAT_ADC_PIN						            P41F	//电池电压
sbit    DC_5V_GPIO = P4^2;
sbit    BAT_ADC_GPIO = P4^1;


#define BAT_3V										1228
#define BAT_LOW_VOL								901			//2.2V	 	电池低电 		
#define BAT_VOL_OFFSET						0				//测量电池电压时允许的误差				

#define BAT_V_SAMP_COUNT					30


#define TEST_DC_5V_TIME						20		//测试DC5V接入的延时确定时间，	单位：10ms     

#define ENTER_LOWSPEED_TIME				10		//无动作进入低速模式的时间，		单位：1s 


#define SHOW_LOW_VOL_ALWAYS							//定义 只要检测到低电就进行低电指示




/* Exported variables --------------------------------------------------------*/  
extern uint8_t  lowspeed;
extern uint8_t  time_to_enter_lowspeed;
extern uint8_t  flag_dc_5v;
extern uint16_t low_power_test_time; //5s


void test_5v_in(void);
void bat_vol_test(void);   
void get_low_vol_flag(void);
void low_vol_opera(void);
void test_5v_in_lowspeed(void);
void nothing_action_test(void);

void enter_low_speed_mode(void);
void quit_low_speed_mode(void);

extern void test_5v_ADC_init(void);
extern void bat_ADC_init(void);

extern void close_bat_adc(void);

extern void test_5v_init(void);

#endif
