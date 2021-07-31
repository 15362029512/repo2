#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "user_type.h"
#include "bsp.h"

/*********************************************************************************************************************/
sbit motor_a	= P3^3;
sbit motor_b 	= P3^7;

#define MOTOR_AIN_PIN					P33F
#define MOTOR_BIN_PIN					P37F

#define MOTOR_A_FUNC_EN				P33_PWM6_SETTING			//PWM6
#define MOTOR_A_FUNC_DIS			OUTPUT

#define MOTOR_B_FUNC					OUTPUT


#define MOTOR_PWM_CH					6									


#define SHAKE_TIME						50								//震动时间	单位：10ms
#define STOP_TIME 						10								//停止时间	单位：10ms

#define MOTOR_FREQ						PLL								//电机PWM2 的时钟
#define MOTOR_CTR_PWM_FREQ		100000						//控制电机的pwm的频率  	单位：Hz	


#define motor_strength_L			137				//62%			  
#define motor_strength_M			159				//72%
#define motor_strength_H			199				//90%

#define STRENGTH_L			0					//震动强度 低
#define STRENGTH_M			1					//震动强度 中
#define STRENGTH_H			2					//震动强度 高

#define SHAKE_CYCLE_TIMES			3//8
/*********************************************************************************************************************/
typedef struct
{
//	u16 freq;					//震动频率
	u8 strength;				//震动强度   【pwm的占空比】
	u8 enable;
	u8 state;	
//	u8 shaketime;			//震动时间
//	u8 stoptime;			//停止时间
	u8 time_cnt;
	
	u8 cycle;								//一个循环 滴 的次数【为0表示一直间隔响，无限次】  【滴 滴，  滴 滴 滴，   滴 滴 滴 滴】
	u8 cycle_cnt;		
	
	u8 finish;	
}_MOTOR;


extern _MOTOR motor;
extern u8 alarm_shake_seted_strength;
extern u8 alarm_shake_cycle;
extern u8 shake_strength[3];


extern void motor_init(void);


extern void motor_start(u8 _strength,u8 _cycle);
extern void motor_stop(void);

extern void motor_pro(void);
extern void alarm_shake(void);


extern void motor_enable(u8 _strength);		



#endif
