#include "motor.h"

u8 motor_strength;				//震动强度   【pwm的占空比】
bit motor_en;
bit motor_state;	
u8 motor_time_cnt;
u8 motor_cycle;								//一个循环 滴 的次数【为0表示一直间隔响，无限次】  【滴 滴，  滴 滴 滴，   滴 滴 滴 滴】
u8 motor_cycle_cnt;		

bit motor_finish;	



u8 shake_strength[] = {motor_strength_L,motor_strength_M,motor_strength_H};

u8 alarm_shake_seted_strength = 0;					//设置的震动的强度  由3挡开关控制
u8 alarm_shake_cycle = 2;										//一个循环里 滴 的次数
/*********************************************************************************************************************
功	能：电机开启 转动
参	数：
返回值：
*********************************************************************************************************************/
void motor_enable(u8 _strength)		
{
	GPIO_Init(MOTOR_AIN_PIN,MOTOR_A_FUNC_EN);		//motor A 引脚设置为PWM
	GPIO_Init(MOTOR_BIN_PIN,OUTPUT);						//motor B 引脚设置为 输出
	motor_b = 0;																//motor B 输出 0
	
	INDEX = MOTOR_PWM_CH;
    
	
//	PWMDIVH	= 0;
//	PWMDIVL	= 255;															//4M 14.4KHz	
	
//	PWMDIVH	= 0;
//	PWMDIVL	= 123;															//4M 30KHz
	
//	PWMDIVH	= 0;
//	PWMDIVL	= 73;																//4M 50KHz
	
//	PWMDIVH	= (u8)(442>>8);
//	PWMDIVL	= (u8)442;													//22.1184M 50KHz

	PWMDIVH	= (u8)(221>>8);
	PWMDIVL	= (u8)221;													//22.1184M 100KHz
	
	PWMDUTH = (u8)(_strength>>8);
	PWMDUTL = (u8)_strength;								//设置PWM2 （motor A的PWM） 占空比：即电机的震动强度
	PWMUPD |= 1 << MOTOR_PWM_CH;								//更新PWM6参数		
	while(PWMUPD);															//等待更新完成
	PWMEN |= 1 << MOTOR_PWM_CH;									//开启PWM6 （motor A的PWM）		
}
/*********************************************************************************************************************
功	能：电机 关闭
参	数：
返回值：
*********************************************************************************************************************/
void motor_disable(void)
{
	PWMEN &= ~(1 << MOTOR_PWM_CH);					//关闭PWM6 （motor A）	
	GPIO_Init(MOTOR_AIN_PIN,	OUTPUT);		//motor A 引脚设置为 输出	
	GPIO_Init(MOTOR_BIN_PIN,	OUTPUT);		//motor B 引脚设置为 输出	
	motor_a = 1;
	motor_b = 1;													//两个引脚都输出高，，刹车
	Delay_50us(20);//延时1ms
    motor_a = 0;
	motor_b = 0;													//两个引脚都输出高，，刹车
	
}


void motor_init(void)
{
    GPIO_Init(MOTOR_AIN_PIN,	OUTPUT);		//motor A 引脚设置为 输出	
    GPIO_Init(MOTOR_BIN_PIN,	OUTPUT);		//motor B 引脚设置为 输出	

    motor_a = 0;
    motor_b = 0;													//两个引脚都输出高，，刹车
}

/*********************************************************************************************************************
功	能：
参	数：
返回值：
*********************************************************************************************************************/
void motor_start(u8 _strength,u8 _cycle)
{
	motor_state = 0;
	motor_finish = 0;	
	motor_strength = _strength;
//	motor.shaketime = 20;					//
//	motor.stoptime = 8;
	motor_time_cnt = 0;
	motor_cycle = _cycle;
	motor_cycle_cnt = 0;
	
	motor_en = 1;	
	motor_enable(_strength);
}
/*********************************************************************************************************************
功	能：
参	数：
返回值：
*********************************************************************************************************************/
void motor_stop(void)
{
	motor_en = 0;
	motor_disable();
}
/*********************************************************************************************************************
功	能：
参	数：
返回值：
*********************************************************************************************************************/
void motor_pro(void)
{
	if(motor_en == 0)// || motor.stoptime == 0)
		return;
	
	if(motor_state == 0)
	{
//		if(motor.stoptime > 0)			//震动有停止，即不是一直持续，而是有间隔的震动
		{
			if(++motor_time_cnt >= SHAKE_TIME/*motor.shaketime*/)
			{
				motor_state = 1;
				motor_time_cnt = 0;
				motor_disable();				
			}
		}		
	}
	else// if(motor_state == 1)
	{
		if(++motor_time_cnt >= STOP_TIME/*motor.stoptime*/ )
		{
			if(motor_cycle > 0)				//如果是震动多次（包括一次），即不是一直震动，而是有间隔的震动   嗡 嗡 嗡
			{
				if(++motor_cycle_cnt >= motor_cycle)		//达到震动的次数，
				{			
					motor_en = 0;
					motor_finish = 1;
					return ;															//直接退出
				}
			}			
			motor_state = 0;					//无限次震动  或  没达到震动的次数 ，
			motor_time_cnt = 0;
			motor_enable(motor_strength);							//再次震动
		}
	}
}
/*********************************************************************************************************************
功	能：
参	数：
返回值：
*********************************************************************************************************************/
void alarm_shake(void)
{
	static u8 cycle_delay_time = 0;				//两个循环之间的间隔时间 
	static u8 alarm_cycle_times = 0;			//单个循环重复的次数	
	
	if(alarm_shake_step == 0)
	{
		cycle_delay_time = 0;
		alarm_cycle_times = 0;
		motor_start(shake_strength[alarm_shake_seted_strength],alarm_shake_cycle);
		alarm_shake_step = 1;	
	}
	else
	{
		if(motor_finish)										//完成了一次单个的循环
		{
			if(++ cycle_delay_time >= 50)			//延时500ms
			{					
				cycle_delay_time = 0;
				motor_finish = 0;

				if(++ alarm_cycle_times >= SHAKE_CYCLE_TIMES)		//每个循环 达到重复次数 后
				{
					alarm_cycle_times = 0;
					alarm_shake_cycle += 1;		
				}
				
				if(alarm_shake_cycle > 4)												//超过 ‘ 嗡 嗡 嗡 嗡 ’ 后，			
				{					
					motor_start(shake_strength[alarm_shake_seted_strength],0);		
				}
				else
				{
					motor_start(shake_strength[alarm_shake_seted_strength],alarm_shake_cycle);				
				}
			}				
		}	
	}
}

