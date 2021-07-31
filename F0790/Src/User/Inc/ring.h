#ifndef _RING_H_
#define _RING_H_


/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "user_type.h"

/* Exported types ------------------------------------------------------------*/
typedef enum
{
	cmd_ring_key1	= 0,					//				0						
	cmd_ring_key2,							//				1				
	cmd_ring_key3,							//				2				
	
	cmd_ring_power_conn1,				//				3				
	cmd_ring_power_conn2,				//				4				
	cmd_ring_power_conn3,				//				5				
	cmd_ring_power_conn4,				//				6				
	
	cmd_ring_off1,							//				7				
	cmd_ring_off2,							//				8				
	cmd_ring_on1,								//				9				
	cmd_ring_on2,								//				10				
	
	cmd_ring_alarm1,						//闹铃1   	11			
	cmd_ring_alarm2,						//闹铃2			12			
	cmd_ring_alarm3,						//				13				
	cmd_ring_alarm4,						//				14												
	cmd_ring_alarm5,						//				15						
		
	cmd_ring_stop = 0xef,
	
	cmd_valume_1 = 0xf0,
	cmd_valume_2,
	cmd_valume_3,
	cmd_valume_4,
	cmd_valume_5,
	cmd_valume_6,
	cmd_valume_7,
	cmd_valume_8,
	cmd_valume_9,
	cmd_valume_10,
	cmd_valume_11,
	cmd_valume_12,
	cmd_valume_13,
	cmd_valume_14,
	cmd_valume_15,
	cmd_valume_16,
}CMD_INDEX;

/* Exported macro ------------------------------------------------------------*/
#define SCL										7
#define SDA										6
#define DAC_R								    2
#define V_AMP_CTL                               7



#define SCL_PIN								P67F
#define SDA_PIN								P66F
#define BUSY_PIN							P40F
#define DAC_R_PIN							P52F
#define V_AMP_CTL_PIN					    P57F



#define SCL_LOW								P6 &= ~(1 << SCL)
#define SCL_HIGH							P6 |= (1 << SCL)

#define SDA_LOW								P6 &= ~(1 << SDA)
#define SDA_HIGH							P6 |= (1 << SDA)

#define DAC_R_LOW							P5 &= ~(1 << DAC_R)
#define DAC_R_HIGH							P5 |= (1 << DAC_R)



#define V_AMP_CTL_PIN_LOW					P5 &= ~(1 << V_AMP_CTL)    
#define V_AMP_CTL_PIN_HIGH					P5 |= (1 << V_AMP_CTL)    


#define RING_VOL_CTL_PIN    P43F
#define RING_VOL_CTL_MOD    P43_ADC3_SETTING
#define RING_VOL_CTL_ADCCH  ADC_CH3


#define DAC_R_PIN       P52F




sbit is_busy = P4^0;    



sbit busy = P4^0;//语音IC的PA2


#define ALARM_RING_CNT			    5				//闹铃铃声的数量
#define ALARM_RING_OFFSET		    11
#define ALARM_RING_1				0				// 鸟叫
#define ALARM_RING_2				1				// 蟋蟀 青蛙
#define ALARM_RING_3				2				// 海浪 海鸥
#define ALARM_RING_4				3				// 上发条
#define ALARM_RING_5				4				// 滴滴滴滴

#define VALUME_CNT					3				//音量共有3个等级
#define VALUME_L					0
#define VALUME_M					1
#define VALUME_H					2

#define STATIC_VALUME_CMD			cmd_valume_8



/* Exported variables --------------------------------------------------------*/  

extern uint8_t g_cmd;               //通信时要发送的命令
extern uint8_t g_current_ring_vol_level;     //当前声音的等级  
extern uint8_t alarm_ring_cycle;								//一个循环里 滴 的次数

extern uint8_t send_once_flg;

extern uint8_t ring_gap_time;

extern uint8_t ring_stop_flg;

extern uint8_t send_ring_cmd_step;  // step = 0,发送音量  step = 1 发送语音命令  step = 2，延时

extern uint8_t g_alarm_ring_index;

extern uint8_t spk_cmd_ing;          //正在发送命令

/* Exported functions --------------------------------------------------------*/  
extern void ring_init(void);
extern void send_ring_cmd(uint8_t CMD);
extern void ring_cmd_isr_handler(void);
extern void ring_vol_adjust_handler(void);
extern void turn_on_tone(void);
extern void ring_replay(void);

extern void alarm_tone(void);
extern void ring_stop(void);

extern void ring_close(void);






#endif
