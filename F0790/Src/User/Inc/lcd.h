#ifndef _LCD_H_
#define _LCD_H_

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "user_type.h"


/* Exported types ------------------------------------------------------------*/
/*LCD 数字区能显示的所以数字，对应 num_table[] 数字的下标*/
typedef enum
{
	num_0,			
	num_1,
	num_2,
	num_3,
	num_4,
	num_5,
	num_6,
	num_7,
	num_8,
	num_9,
	num_,
	num_H,
	num_r,
}NUMBER;


/*指向4个数字显示区*/
typedef enum
{
	location_num_1,		
	location_num_2,			
	location_num_3,
	location_num_4,
}NUMBER_LOCATION;

/*指向lcd所有非数字的符号*/
typedef enum
{
    symbol_am,                  //am
    symbol_bat,                 //电池电压                  
    symbol_pm,                  //pm    
    symbol_al1,                 //闹钟 1          
    symbol_al2,                 //闹钟 2                              
    symbol_snooze,              //贪睡           
    symbol_sec1,                 //时钟秒点 
    symbol_sec2,                 //时钟秒点
    symbol_alarm_out,           //闹铃灯
}SYMBOL;

/*LCD 显示模式*/
/*
typedef enum
{
	mode_clock = 0,								//时钟模式  正常显示时钟
	mode_set_clock,								//设置时钟模式
	mode_set_alarm,								//设置闹钟模式
	mode_alarming,
	mode_set_snooze_time,					//设置贪睡时长
	mode_test,										//测试模式
	mode_volum_adjust,
	mode_9v_power_on_wait,
}LCD_MODE;
*/

#define	mode_clock  		0								//时钟模式  正常显示时钟
#define	mode_set_clock		1							//设置时钟模式
#define	mode_set_alarm		2						//设置闹钟模式
#define	mode_alarming		3
#define	mode_set_snooze_time	4				//设置贪睡时长
#define	mode_test				5				//测试模式
#define	mode_volum_adjust		6
#define	mode_9v_power_on_wait	7




typedef enum
{
	from_calendar = 0,
	from_alarm_1,
	from_alarm_2,
}TIME_FROM;


typedef enum
{
	set_clock_hour,
	set_clock_minute,
	set_clock_12_24,
	set_clock_year,
	set_clock_month,
	set_clock_day,
	
	set_alarm_hour,
	set_alarm_minute,
	set_alarm_day,
	
}SET_LOCATION;




/* Exported macro ------------------------------------------------------------*/
#define SMG1_A_PIN				P05F
#define SMG1_B_PIN				P25F
#define SMG1_C_PIN				P26F
#define SMG1_D_PIN				P02F
#define SMG1_E_PIN				P03F
#define SMG1_F_PIN				P04F
#define SMG1_G_PIN				P07F

#define SMG1_A_PORT				P0
#define SMG1_B_PORT				P2
#define SMG1_C_PORT				P2
#define SMG1_D_PORT				P0
#define SMG1_E_PORT				P0
#define SMG1_F_PORT				P0
#define SMG1_G_PORT				P0

#define SMG1_A_BIT				5
#define SMG1_B_BIT				5
#define SMG1_C_BIT				6
#define SMG1_D_BIT				2
#define SMG1_E_BIT				3
#define SMG1_F_BIT				4
#define SMG1_G_BIT				7
//-----------------------------------------
#define SMG2_A_PIN				P21F
#define SMG2_B_PIN				P23F
#define SMG2_C_PIN				P64F
#define SMG2_D_PIN				P35F
#define SMG2_E_PIN				P63F
#define SMG2_F_PIN				P24F
#define SMG2_G_PIN				P13F

#define SMG2_A_PORT				P2
#define SMG2_B_PORT				P2
#define SMG2_C_PORT				P6
#define SMG2_D_PORT				P3
#define SMG2_E_PORT				P6
#define SMG2_F_PORT				P2
#define SMG2_G_PORT				P1

#define SMG2_A_BIT				1
#define SMG2_B_BIT				3
#define SMG2_C_BIT				4
#define SMG2_D_BIT				5
#define SMG2_E_BIT				3
#define SMG2_F_BIT				4
#define SMG2_G_BIT				3
//-----------------------------------------
#define SMG3_A_PIN				P16F
#define SMG3_B_PIN				P15F
#define SMG3_C_PIN				P65F
#define SMG3_D_PIN				P50F
#define SMG3_E_PIN				P61F
#define SMG3_F_PIN				P27F
#define SMG3_G_PIN				P14F

#define SMG3_A_PORT				P1
#define SMG3_B_PORT				P1
#define SMG3_C_PORT				P6
#define SMG3_D_PORT				P5
#define SMG3_E_PORT				P6
#define SMG3_F_PORT				P2
#define SMG3_G_PORT				P1

#define SMG3_A_BIT				6
#define SMG3_B_BIT				5
#define SMG3_C_BIT				5
#define SMG3_D_BIT				0
#define SMG3_E_BIT				1
#define SMG3_F_BIT				7
#define SMG3_G_BIT				4
//-----------------------------------------
#define SMG4_A_PIN				P60F
#define SMG4_B_PIN				P55F
#define SMG4_C_PIN				P53F
#define SMG4_D_PIN				P51F
#define SMG4_E_PIN				P10F
#define SMG4_F_PIN				P12F
#define SMG4_G_PIN				P11F

#define SMG4_A_PORT				P6
#define SMG4_B_PORT				P5
#define SMG4_C_PORT				P5
#define SMG4_D_PORT				P5
#define SMG4_E_PORT				P1
#define SMG4_F_PORT				P1
#define SMG4_G_PORT				P1

#define SMG4_A_BIT				0
#define SMG4_B_BIT				5
#define SMG4_C_BIT				3
#define SMG4_D_BIT				1
#define SMG4_E_BIT				0
#define SMG4_F_BIT				2
#define SMG4_G_BIT				1
//-----------------------------------------
#define SYMB_AM_PIN		        P06F
#define SYMB_BAT_PIN			P01F
#define SYMB_PM_PIN				P20F
#define SYMB_AL1_PIN			P22F
#define SYMB_AL2_PIN			P34F
#define SYMB_SNOOZE_PIN			P56F	
#define SYMB_SEC1_PIN			P17F
#define SYMB_SEC2_PIN			P62F

#define SYMB_AM_PORT		    P0
#define SYMB_BAT_PORT			P0
#define SYMB_PM_PORT			P2
#define SYMB_AL1_PORT			P2
#define SYMB_AL2_PORT			P3
#define SYMB_SNOOZE_PORT		P5	
#define SYMB_SEC1_PORT			P1
#define SYMB_SEC2_PORT			P6


#define SYMB_AM_BIT		        6
#define SYMB_BAT_BIT			1
#define SYMB_PM_BIT				0
#define SYMB_AL1_BIT			2
#define SYMB_AL2_BIT			4
#define SYMB_SNOOZE_BIT			6	
#define SYMB_SEC1_BIT			7
#define SYMB_SEC2_BIT			2


#define LED_SW_CTL_PIN          P00F        //闹铃灯背光控制
#define LED_SW_CTL_PORT		    P0
#define LED_SW_CTL_BIT	        0


/*********************************************************************************************************************/
#define SHOW							1						//显示
#define HIDE							0						//不显示
/**************************************************/

#define NUM_TABLE_SIZE					13									//lcd 7段数字区 可以显示的所以数字的个数
#define SET_KEY_FLASH_GAP				60									//设置按键 设置时不允许闪烁的间隔时长	单位：10ms
//#define SHOW_ALARM_TIME_EN												//定义是否显示闹钟时间
	
#define SHOW_ALARM_TIMER			KEY_NO_ACTION_5s		//定义显示闹钟的时间长度
#define SETING_FLASH_TIMER			KEY_NO_ACTION_8s		//定义设为闪烁的时间长度
#define BAT_SHOW_CLOCK_TIMER		KEY_NO_ACTION_5s		//定义电池供电时显示时钟的时间长度
/*********************************************************************************************************************
显示(X = SHOW) 或 隐藏(X = HIDE) 某一个笔段
**************************************************/
#define SMG1_A(X)		if(X) SMG1_A_PORT &= ~(1 << SMG1_A_BIT);else SMG1_A_PORT |=(1 << SMG1_A_BIT)			//A笔段显示 或 不显示
#define SMG1_B(X)		if(X) SMG1_B_PORT &= ~(1 << SMG1_B_BIT);else SMG1_B_PORT |=(1 << SMG1_B_BIT)			//B笔段显示 或 不显示
#define SMG1_C(X)		if(X) SMG1_C_PORT &= ~(1 << SMG1_C_BIT);else SMG1_C_PORT |=(1 << SMG1_C_BIT)	
#define SMG1_D(X)		if(X) SMG1_D_PORT &= ~(1 << SMG1_D_BIT);else SMG1_D_PORT |=(1 << SMG1_D_BIT)	
#define SMG1_E(X)		if(X) SMG1_E_PORT &= ~(1 << SMG1_E_BIT);else SMG1_E_PORT |=(1 << SMG1_E_BIT)	
#define SMG1_F(X)		if(X) SMG1_F_PORT &= ~(1 << SMG1_F_BIT);else SMG1_F_PORT |=(1 << SMG1_F_BIT)	
#define SMG1_G(X)		if(X) SMG1_G_PORT &= ~(1 << SMG1_G_BIT);else SMG1_G_PORT |=(1 << SMG1_G_BIT)	

#define SMG2_A(X)		if(X) SMG2_A_PORT &= ~(1 << SMG2_A_BIT);else SMG2_A_PORT |=(1 << SMG2_A_BIT)				//A笔段显示 或 不显示
#define SMG2_B(X)		if(X) SMG2_B_PORT &= ~(1 << SMG2_B_BIT);else SMG2_B_PORT |=(1 << SMG2_B_BIT)			//B笔段显示 或 不显示
#define SMG2_C(X)		if(X) SMG2_C_PORT &= ~(1 << SMG2_C_BIT);else SMG2_C_PORT |=(1 << SMG2_C_BIT)
#define SMG2_D(X)		if(X) SMG2_D_PORT &= ~(1 << SMG2_D_BIT);else SMG2_D_PORT |=(1 << SMG2_D_BIT)
#define SMG2_E(X)		if(X) SMG2_E_PORT &= ~(1 << SMG2_E_BIT);else SMG2_E_PORT |=(1 << SMG2_E_BIT)
#define SMG2_F(X)		if(X) SMG2_F_PORT &= ~(1 << SMG2_F_BIT);else SMG2_F_PORT |=(1 << SMG2_F_BIT)
#define SMG2_G(X)		if(X) SMG2_G_PORT &= ~(1 << SMG2_G_BIT);else SMG2_G_PORT |=(1 << SMG2_G_BIT)

#define SMG3_A(X)		if(X) SMG3_A_PORT &= ~(1 << SMG3_A_BIT);else SMG3_A_PORT |=(1 << SMG3_A_BIT)			//A笔段显示 或 不显示
#define SMG3_B(X)		if(X) SMG3_B_PORT &= ~(1 << SMG3_B_BIT);else SMG3_B_PORT |=(1 << SMG3_B_BIT)			//B笔段显示 或 不显示
#define SMG3_C(X)		if(X) SMG3_C_PORT &= ~(1 << SMG3_C_BIT);else SMG3_C_PORT |=(1 << SMG3_C_BIT)
#define SMG3_D(X)		if(X) SMG3_D_PORT &= ~(1 << SMG3_D_BIT);else SMG3_D_PORT |=(1 << SMG3_D_BIT)
#define SMG3_E(X)		if(X) SMG3_E_PORT &= ~(1 << SMG3_E_BIT);else SMG3_E_PORT |=(1 << SMG3_E_BIT)
#define SMG3_F(X)		if(X) SMG3_F_PORT &= ~(1 << SMG3_F_BIT);else SMG3_F_PORT |=(1 << SMG3_F_BIT)
#define SMG3_G(X)		if(X) SMG3_G_PORT &= ~(1 << SMG3_G_BIT);else SMG3_G_PORT |=(1 << SMG3_G_BIT)

#define SMG4_A(X)		if(X) SMG4_A_PORT &= ~(1 << SMG4_A_BIT);else SMG4_A_PORT |=(1 << SMG4_A_BIT)			//A笔段显示 或 不显示
#define SMG4_B(X)		if(X) SMG4_B_PORT &= ~(1 << SMG4_B_BIT);else SMG4_B_PORT |=(1 << SMG4_B_BIT)			//B笔段显示 或 不显示
#define SMG4_C(X)		if(X) SMG4_C_PORT &= ~(1 << SMG4_C_BIT);else SMG4_C_PORT |=(1 << SMG4_C_BIT)
#define SMG4_D(X)		if(X) SMG4_D_PORT &= ~(1 << SMG4_D_BIT);else SMG4_D_PORT |=(1 << SMG4_D_BIT)
#define SMG4_E(X)		if(X) SMG4_E_PORT &= ~(1 << SMG4_E_BIT);else SMG4_E_PORT |=(1 << SMG4_E_BIT)
#define SMG4_F(X)		if(X) SMG4_F_PORT &= ~(1 << SMG4_F_BIT);else SMG4_F_PORT |=(1 << SMG4_F_BIT)
#define SMG4_G(X)		if(X) SMG4_G_PORT &= ~(1 << SMG4_G_BIT);else SMG4_G_PORT |=(1 << SMG4_G_BIT)

#define SYMB_AM_S(X)	if(X) SYMB_AM_PORT &= ~(1 << SYMB_AM_BIT);else SYMB_AM_PORT |=(1 << SYMB_AM_BIT)
#define SYMB_BAT(X)	    if(X) SYMB_BAT_PORT &= ~(1 << SYMB_BAT_BIT);else SYMB_BAT_PORT |=(1 << SYMB_BAT_BIT)
#define SYMB_PM(X)	    if(X) SYMB_PM_PORT  &= ~(1 << SYMB_PM_BIT); else SYMB_PM_PORT  |=(1 << SYMB_PM_BIT)
#define SYMB_AL1(X)	    if(X) SYMB_AL1_PORT &= ~(1 << SYMB_AL1_BIT);else SYMB_AL1_PORT |=(1 << SYMB_AL1_BIT)
#define SYMB_AL2(X)	    if(X) SYMB_AL2_PORT &= ~(1 << SYMB_AL2_BIT);else SYMB_AL2_PORT |=(1 << SYMB_AL2_BIT)
#define SYMB_SNOOZE(X)	if(X) SYMB_SNOOZE_PORT &= ~(1 << SYMB_SNOOZE_BIT);else SYMB_SNOOZE_PORT |=(1 << SYMB_SNOOZE_BIT)
#define SYMB_SEC1(X)	if(X) SYMB_SEC1_PORT &= ~(1 << SYMB_SEC1_BIT);else SYMB_SEC1_PORT |=(1 << SYMB_SEC1_BIT)
#define SYMB_SEC2(X)	if(X) SYMB_SEC2_PORT &= ~(1 << SYMB_SEC2_BIT);else SYMB_SEC2_PORT |=(1 << SYMB_SEC2_BIT)

#define LED_SW_CTL(X)	if(X) LED_SW_CTL_PORT |=(1 << LED_SW_CTL_BIT); else LED_SW_CTL_PORT &= ~(1 << LED_SW_CTL_BIT)  //闹铃灯背光控制


/* Exported variables --------------------------------------------------------*/  
extern uint8_t  		    lcd_mode;
extern SET_LOCATION		    set_location;
extern uint8_t				flag_set_flash_en;
extern uint8_t				flag_set_flash_show;
extern uint8_t				flag_clockdot_show;
extern uint8_t				set_no_flash_gap;

extern uint8_t				flag_turn_on_bl;
extern uint8_t				flag_turn_off_mode;
extern uint8_t				show_in_bat_5sec;

extern uint16_t g_9v_power_on_wait_time; //500ms


/* Exported functions --------------------------------------------------------*/  
extern void lcd_init(void);
extern void lcd_clear(void);
extern void lcd_show_all(void);
extern void lcd_num_deal(NUMBER_LOCATION which,uint8_t deal,uint8_t num);
extern void lcd_symbol_deal(SYMBOL which,uint8_t deal);
extern void lcd_display(void);





#endif
