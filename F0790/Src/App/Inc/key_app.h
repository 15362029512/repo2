#ifndef _KEY_APP_H_
#define _KEY_APP_H_

#include "bsp.h"
#include "user_type.h"




/* Exported macro ------------------------------------------------------------*/
/*--------------------time0 10ms中断中 处理 ，单位：50ms -------------------*/
#define KEY_NO_ACTION_8s					160			
#define KEY_NO_ACTION_5s					100			    /* Exported variables --------------------------------------------------------*/  
#define KEY_NO_ACTION_4s					80
#define KEY_NO_ACTION_3s					60
#define KEY_NO_ACTION_2s					40				/* Exported functions --------------------------------------------------------*/  
#define KEY_NO_ACTION_1s					20


/* global variables ----------------------------------------------------------*/
extern uint8_t key_no_action_start;
extern uint8_t key_no_action_cnt;

extern uint8_t  test_step;								
extern uint8_t 	test_key_num;							




/* Private functions ---------------------------------------------------------*/
static void test_time_set_step(void);
static void key_12_24h_short_up_handler(void);
static void key_12_24h_long_press_handler(void);
static void key_up_handler(void);
static void key_down_handler(void);
static void test_alarm_set_step(uint8_t _set_alarm);
static void enter_alarm_seting(uint8_t _set_alarm);
static void alarm_stop_key_function(uint8_t _key);
static void key_al1_short_up_handler(void);
static void key_al1_long_press_handler(void);
static void key_al2_short_up_handler(void);
static void key_al2_long_press_handler(void);
static void key_snooze_short_up_handler(void);
static void key_snooze_long_handler(void);
static void key_alarm_set_short_up_handler(void);
static void key_alarm_set_long_press_handler(void);
static void key_alarm_set_short_press_handler(void);
static void key_up_down_double_handler(void);
static void set_clock_add(void);
static void set_clock_sub(void);
static void set_alarm_add(void);
static void set_alarm_sub(void);


/* Exported functions --------------------------------------------------------*/  
extern void key_process(void);
extern void key_no_action_test(void);




#endif
