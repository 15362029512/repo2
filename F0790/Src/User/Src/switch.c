#include "switch.h"


uint8_t g_current_motor_level = 0;
uint8_t g_current_alarm_mode_level = 0;


uint8_t g_before_motor_level = 0;
uint8_t g_before_alarm_mode_level = 0;


uint8_t g_volume_and_mode_init_flg = 0;  //震动大小和闹铃模式初始化标志



u8 m_switch[2] = {0};	



uint16_t switch_level_buf[][SWITCH_LEVEL_CNT] = 
{
    {SWITCH_H,SWITCH_L,SWITCH_M},
    {SWITCH_H,SWITCH_L,SWITCH_M}
};

uint8_t switch_ADC_ch_table[] = {MOTOR_ADC_CH,MODE_ADC_CH};


uint8_t g_led_alarm_enable = 0;


//code int switch_level_buf[SWITCH_LEVEL_CNT] = {SWITCH_H,SWITCH_L,SWITCH_M};

//code u8 switch_adc_ch[] = {MOTOR_ADC_CH,MODE_ADC_CH};



void switch_ADC_init(void)
{
    GPIO_Init(MOTOR_SWITCH_ADC_PIN,			MOTOR_SWITCH_PIN_ADC_MODE);
    GPIO_Init(MODE_SWITCH_ADC_PIN,				MODE_SWITCH_PIN_ADC_MODE);
    ADCON = 0x3d;
    ADCFGL |= 0xc0;	
}

void switch_led_mod_init(void)
{
    GPIO_Init(LED_SW_MOD_PIN, INPUT);
}


void led_mod_detect(void)
{
    if (LED_SW_MOD_GPIO)
    {
        g_led_alarm_enable = 1;
    }
    else
    {
        g_led_alarm_enable = 0;
    }
}


void close_switch_ADC_pin(void)
{
    GPIO_Init(MOTOR_SWITCH_ADC_PIN,			OUTPUT);
    GPIO_Init(MODE_SWITCH_ADC_PIN,			OUTPUT);

    MOTOR_SWITCH_ADC_GPIO = 0;
    MODE_SWITCH_ADC_GPIO = 0;
    
}



uint8_t get_current_switch_level(uint8_t _adc_chx_index)
{
    uint8_t i = 0;
    uint16_t adc_value = 0;
    adc_value = GetChannelAdcValue(switch_ADC_ch_table[_adc_chx_index]);
    for (i = 0; i < SWITCH_LEVEL_CNT; i++)
    {
       if (adc_value < switch_level_buf[_adc_chx_index][i])
       {
            if (i == 0)
            {
                i = 2;
            }
            else if (i == 1)
            {
                i = 0;
            }
            else if (i == 2)
            {
                i = 1;
            }
            return i;
       }
    }
	return 0xff;
}






void get_volume_and_mode(void)
{
    if (!flag_alarm_timeout)   //正在播放闹钟时不处理，调节后下次生效
    {
        g_current_motor_level = get_current_switch_level(MOTOR_ADC_INDEX);
        g_current_alarm_mode_level = get_current_switch_level(MODE_ADC_INDEX);

        if ((g_current_motor_level != g_before_motor_level) ||  \
            (g_current_alarm_mode_level != g_before_alarm_mode_level) || \
            (!g_volume_and_mode_init_flg))
        {
            switch (g_current_motor_level)
            {
                case SWITCH_H_INDEX:
                    alarm_shake_seted_strength = STRENGTH_H;
                    break;
                    
                case SWITCH_L_INDEX:
                    alarm_shake_seted_strength = STRENGTH_L;	
                    break;
                    
                case SWITCH_M_INDEX:
                    alarm_shake_seted_strength = STRENGTH_M;
                    break;
                    
                default:break;
            }

            switch (g_current_alarm_mode_level)
            {
                case SWITCH_MODE_1:
                    flag_alarm_style_ring = 1;
                    flag_alarm_style_shake = 0;
                    break;
                    
                case SWITCH_MODE_2:
                    flag_alarm_style_ring = 1;
                    flag_alarm_style_shake = 1;
                    break;
                    
                case SWITCH_MODE_3:
                    flag_alarm_style_ring = 0;
                    flag_alarm_style_shake = 1;
                    break;
                default:break;
            }


            if (!g_volume_and_mode_init_flg)
            {
                g_volume_and_mode_init_flg = 1;
            }
            if (g_current_motor_level != g_before_motor_level)
            {
                g_before_motor_level = g_current_motor_level;
            }
            if (g_current_alarm_mode_level != g_before_alarm_mode_level)
            {
                g_before_alarm_mode_level = g_current_alarm_mode_level;
            }
        }
    }
}
