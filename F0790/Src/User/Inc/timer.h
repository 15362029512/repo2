#ifndef _TIMER_H_
#define _TIMER_H_

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "user_type.h"

/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define TIMER0_5MS_CNT     5
#define TIMER0_10MS_CNT     10
#define TIMER0_20MS_CNT     20
#define TIMER0_200MS_CNT    200
#define TIMER0_2S_CNT       200



/* Exported variables --------------------------------------------------------*/  
extern uint16_t g_timer0_5ms_cnt;
extern uint16_t g_timer0_10ms_cnt;
extern uint16_t g_timer0_20ms_cnt;
extern uint16_t g_timer0_200ms_cnt;
extern uint16_t g_timer0_2s_cnt;

extern uint8_t flag_50ms; 
extern uint8_t flag_10ms; 
extern uint8_t flag_20ms;
extern uint8_t flag_100ms;
extern uint8_t flag_200ms;
extern uint8_t flag_2s;



/* Exported functions --------------------------------------------------------*/  
extern void timer_init(void);









#endif
