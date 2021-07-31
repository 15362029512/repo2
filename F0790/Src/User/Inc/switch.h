#ifndef _SWITCH_H_
#define _SWITCH_H_

#include "user_type.h"
#include "bsp.h"


#define SWITCH_CNT						2				//开关数量
#define SWITCH_VOL						0				//音量开关
#define SWITCH_MODE						1				//模式开关

#define SWITCH_LEVEL_CNT			    3				//每个开关分 3 档
/*
#define SWITCH_H						13		//4095 * 0.033 / (0.033 + 10)	= 13
#define SWITCH_L						2047		//4095 * 10 / (10 + 10)	= 2047
#define SWITCH_M						3424		//4095 * 51 / (51 + 10)	= 3424
*/



#define SWITCH_H						1030		//(SWITCH_H + SWITCH_L)/2
#define SWITCH_L						2735		//(SWITCH_L + SWITCH_M)/2
#define SWITCH_M						3759		//(SWITCH_M + 4095)/2

#define SWITCH_L_INDEX						0	
#define SWITCH_M_INDEX						1		
#define SWITCH_H_INDEX						2	


#define SWITCH_MODE_1					0				//闹钟响铃方式1：单铃声 
#define SWITCH_MODE_2 				    1				//闹钟响铃方式2：铃声 + 震动
#define SWITCH_MODE_3					2				//闹钟响铃方式3：单震动



#define MOTOR_SWITCH_ADC_PIN					P46F		//ch3				震动强度
#define MODE_SWITCH_ADC_PIN					P47F		//ch7				闹钟方式

#define MOTOR_ADC_CH					ADC_CH6				//  0100 AD_CH[3]
#define MODE_ADC_CH						ADC_CH7				//  1000 AD_CH[7]

#define MOTOR_SWITCH_PIN_ADC_MODE              P46_ADC6_SETTING
#define MODE_SWITCH_PIN_ADC_MODE               P47_ADC7_SETTING

sbit MOTOR_SWITCH_ADC_GPIO = P4^6;
sbit MODE_SWITCH_ADC_GPIO  = P4^7;  


#define LED_SW_MOD_PIN      P32F
sbit LED_SW_MOD_GPIO =      P3^2;





#define MOTOR_ADC_INDEX                       0
#define MODE_ADC_INDEX                      1











extern bit f_ok_to_test_switch;

extern u8 m_switch[2];


extern uint8_t g_current_motor_level;
extern uint8_t g_current_alarm_mode_level;

extern uint8_t g_before_motor_level;
extern uint8_t g_before_alarm_mode_level;

extern uint8_t g_volume_and_mode_init_flg;  //震动大小和闹铃模式初始化标志

extern uint8_t g_led_alarm_enable;


extern void switch_ADC_init(void);
extern void get_volume_and_mode(void);
extern void switch_test(void);
extern void switch_level_handler(void);
extern void close_switch_ADC_pin(void);
extern void switch_led_mod_init(void);
extern void led_mod_detect(void);


#endif
