#ifndef _BSP_H_
#define _BSP_H_


/* Includes ------------------------------------------------------------------*/
#include "ca51f_config.h"		
#include "ca51f2sfr.h"
#include "ca51f2xsfr.h"
#include "gpiodef_f2.h"
#include "system_clock.h"
#include "ring.h"
#include "timer.h"
#include "rtc.h"
#include "interrupt.h"
#include "bat.h"
#include "lcd.h"
#include "key.h"
#include "bl.h"
#include "motor.h"
#include "switch.h"
/* Exported types ------------------------------------------------------------*/
typedef struct
{
    double k;
    double b;
}adjust_param_type; //占空比和AD值的关系


/* Exported macro ------------------------------------------------------------*/
#define LINEAR_ADJUST  		0
#define CURVILINEAR_ADJUST 	1

#define PWM_DUTY_ADJUST_MODE		CURVILINEAR_ADJUST

#define PRINT_ENABLE		1

#define AD_FILTER         1


//#define TIME_TEST_LOWPOWER_WAKEUP 1
#define BL_PWM_CTRL 0
#define BL_IO_CTRL  1 

#define BL_CTRL_MOD   BL_PWM_CTRL  


/* Exported variables --------------------------------------------------------*/  


/* Exported functions --------------------------------------------------------*/  
extern void bsp_init(void);
extern void no_use_io_init(void);


#endif
