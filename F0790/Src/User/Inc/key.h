#ifndef _KEY_H_
#define _KEY_H_

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "user_type.h"

/* Private define ------------------------------------------------------------*/
#define KEY_AD_CH  			    ADC_CH5
#define KEY_AD_PIN              P45F
#define KEY_AD_MOD              P45_ADC5_SETTING
sbit KEY_AD_GPIO = P4^5;


#define HARD_KEY_NUM	        7	   				        // 实体按键个数 
#define KEY_COUNT               (HARD_KEY_NUM + 1)          //实际用到的按键 = 实体按键个数 + 双按的按键
#define KEY_FIFO_SIZE	        15

/*
	按键滤波时间50ms, 单位10ms。
	只有连续检测到50ms状态不变才认为有效，包括弹起和按下两种事件
	即使按键电路不做硬件滤波，该滤波机制也可以保证可靠地检测到按键事件
*/
#define KEY_FILTER_TIME         5
#define KEY_LONG_TIME           200			/* 单位5ms， 持续2秒，认为长按事件 */
#define KEY_REPEAT_TIME         20			/* 长按每200ms发送一次按键值 */

#define KEY_NONE_AD_VAL         4095

#define KEY_12_24H_SET_AD_VAL           698// (KEY_12_24H_SET_AD_VAL + KEY_SNOOZE_AD_VAL)/2                     
#define KEY_SNOOZE_AD_VAL               1541// (KEY_SNOOZE_AD_VAL + KEY_UP_DOWM_DOUBLE_AD_VAL)/2       
#define KEY_UP_DOWM_DOUBLE_AD_VAL       1825// (KEY_UP_DOWM_DOUBLE_AD_VAL + KEY_UP_AD_VAL)/2           
#define KEY_UP_AD_VAL                   2204// (KEY_UP_AD_VAL + KEY_ALARM_SET_AD_VAL)/2         
#define KEY_ALARM_SET_AD_VAL            2636// (KEY_ALARM_SET_AD_VAL + KEY_ALARM1_AD_VAL)/2     
#define KEY_ALARM1_AD_VAL               3000//2978// (KEY_ALARM1_AD_VAL + KEY_DOWN_AD_VAL)/2          
#define KEY_DOWN_AD_VAL                 3377// (KEY_DOWN_AD_VAL + KEY_ALARM2_AD_VAL)/2          
#define KEY_ALARM2_AD_VAL               3854// (KEY_ALARM2_AD_VAL + KEY_NONE_AD_VAL)/2  
/* Exported macro ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/*
    每个按键对应1个全局的结构体变量。
	
*/
typedef struct
{
	uint8_t      Count;         /* 滤波器计数器 */
    uint16_t     LongCount;     /* 长按计数器 */
    uint16_t     LongTime;      /* 按键按下持续时间, 0表示不检测长按 */
    uint8_t      State;         /* 按键当前状态（按下还是弹起） */
    uint8_t      RepeatSpeed;   /* 连续按键周期 */
    uint8_t      RepeatCount;   /* 连续按键计数器 */
}KEY_T;

typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* 键值缓冲区 */
	uint8_t Read;					/* 缓冲区读指针1 */
	uint8_t Write;					/* 缓冲区写指针 */
}KEY_FIFO_T;

/* Exported types ------------------------------------------------------------*/
/*按键ID,按键数组对应的下标*/
typedef enum
{
    KID_12_24H_SET = 0,
    KID_SNOOZE,
    KID_UP_DOWM_DOUBLE,
	KID_UP,
	KID_ALARM_SET,
	KID_ALARM1,
	KID_DOWN,
	KID_ALARM2,
}KEY_ID_E;

/*按键返回值(不同的按键状态)*/
typedef enum
{
    KEY_NONE = 0,                   /* 0 表示按键事件 */

    KEY_12_24H_SET_SHORT_PRESS,                /*按下*/
    KEY_12_24H_SET_SHORT_UP,            /*短按抬起*/
    KEY_12_24H_SET_LONG_FIRST,          /*第一次长按*/
    KEY_12_24H_SET_LONG_REPEAT,         /*持续长按*/
    KEY_12_24H_SET_LONG_UP,             /*长按抬起*/

    KEY_SNOOZE_SHORT_PRESS,                /*按下*/
    KEY_SNOOZE_SHORT_UP,            /*短按抬起*/
    KEY_SNOOZE_LONG_FIRST,          /*第一次长按*/
    KEY_SNOOZE_LONG_REPEAT,         /*持续长按*/
    KEY_SNOOZE_LONG_UP,             /*长按抬起*/

    KEY_UP_DOWM_DOUBLE_SHORT_PRESS,             /*按下*/
    KEY_UP_DOWM_DOUBLE_SHORT_UP,         /*短按抬起*/
    KEY_UP_DOWM_DOUBLE_LONG_FIRST,       /*第一次长按*/
    KEY_UP_DOWM_DOUBLE_LONG_REPEAT,      /*持续长按*/
    KEY_UP_DOWM_DOUBLE_LONG_UP,          /*长按抬起*/
    

    KEY_UP_SHORT_PRESS,             /*按下*/
    KEY_UP_SHORT_UP,         /*短按抬起*/
    KEY_UP_LONG_FIRST,       /*第一次长按*/
    KEY_UP_LONG_REPEAT,      /*持续长按*/
    KEY_UP_LONG_UP,          /*长按抬起*/

    KEY_ALARM_SET_SHORT_PRESS,                /*按下*/
    KEY_ALARM_SET_SHORT_UP,            /*短按抬起*/
    KEY_ALARM_SET_LONG_FIRST,          /*第一次长按*/
    KEY_ALARM_SET_LONG_REPEAT,         /*持续长按*/
    KEY_ALARM_SET_LONG_UP,             /*长按抬起*/

    KEY_ALARM1_SHORT_PRESS,                     /*按下*/
    KEY_ALARM1_SHORT_UP,                /*短按抬起*/
    KEY_ALARM1_LONG_FIRST,              /*第一次长按*/
    KEY_ALARM1_LONG_REPEAT,             /*持续长按*/
    KEY_ALARM1_LONG_UP,                 /*长按抬起*/ 

    KEY_DOWN_SHORT_PRESS,                  /*按下*/
    KEY_DOWN_SHORT_UP,              /*短按抬起*/
    KEY_DOWN_LONG_FIRST,            /*第一次长按*/
    KEY_DOWN_LONG_REPEAT,           /*持续长按*/
    KEY_DOWN_LONG_UP,               /*长按抬起*/  

    KEY_ALARM2_SHORT_PRESS,                  /*按下*/
    KEY_ALARM2_SHORT_UP,              /*短按抬起*/
    KEY_ALARM2_LONG_FIRST,            /*第一次长按*/
    KEY_ALARM2_LONG_REPEAT,           /*持续长按*/
    KEY_ALARM2_LONG_UP,               /*长按抬起*/  
}KEY_ENUM;


/* Exported variables --------------------------------------------------------*/  
extern uint8_t ucKeyCode;      /* 按键返回值 */


/* Private functions ---------------------------------------------------------*/
static void bsp_InitKeyVar(void);
static uint8_t KeyPinActive(uint8_t index);
static uint8_t IsKeyDownFunc(uint8_t _id);
static void bsp_PutKey(uint8_t _KeyCode);
static void bsp_DetectKey(uint8_t i);

/* Exported functions --------------------------------------------------------*/  
extern void key_ad_init(void);
extern uint8_t bsp_GetKey(void);
extern void bsp_KeyScan10ms(void);
extern void key_print(void);
extern void close_key_ad(void);



#endif
