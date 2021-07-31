#ifndef _BL_H_
#define _BL_H_

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "user_type.h"
#include "adc.h"
/* Exported types ------------------------------------------------------------*/




/* Exported macro ------------------------------------------------------------*/
#define AD_BUFF_LEN                     10
sbit BL_CTL = P5^4;

#define BL_CTL_PIN                      P54F        //数码管背光控制
#define BL_MOD			                P54_PWM4_SETTING
#define BL_PWMCH		                4           //PWM CH4
sbit BL_CTL_GPIO = P5^4;

#define BL_ADC_DETECT_PIN                       P44F  
#define BL_ADC_DETECT_MOD                       P44_ADC4_SETTING
#define BL_ADC_DETECT_ADCCH                     ADC_CH4  


#define BL_PWM_DivDat				    2000		//PWM频率131000/2000 = 65hz
#define BL_BRIGHTNESS_MAX				2000     //PWM频率131000/2000 = 65hz
#define BL_BRIGHTNESS_MIN				0



/* Exported variables --------------------------------------------------------*/  
extern uint16_t AD_value_buffer[AD_BUFF_LEN];
extern uint16_t bl_PWM_duty;         //背光占空比
extern uint8_t bl_pwm_mode;



/* Exported functions --------------------------------------------------------*/  
extern void bl_init(void);
extern void bl_handler(void);
extern uint16_t get_adjust_AD_value(ADC_Channel_TypeDef adc_chx);
extern uint16_t get_dvalue_u16(uint16_t value1,uint16_t value2);
extern uint8_t get_dvalue_u8(uint8_t value1,uint8_t value2);
extern void bl_close(void);
extern void bl_brightness_set(uint16_t bl_duty);

#endif
