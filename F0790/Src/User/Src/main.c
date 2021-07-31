#ifndef _MAIN_C_
#define _MAIN_C_
/* Includes ------------------------------------------------------------------*/
#include "app.h"
#include "bsp.h"
#include "uart.h"
#include "delay.h"
/*-------------------------------------------------------------------------
*MCU:CA51F253L3
*FOSC:22118400hZ
*仿真时需要供电
*-------------------------------------------------------------------------
*/


void main(void)
{

    #if (SYSCLK_SRC == IRCH)
		Sys_Clk_Set_IRCH();
	#elif (SYSCLK_SRC == PLL)
        Sys_Clk_Set_PLL(PLL_Multiple);
    #endif
	CKCON |= ILCKE;	//使能内部低速时钟，用于背光pwm										//IRCL时钟使能

    GPIO_Init(P72F,P72_XOSCL_IN_SETTING);
	GPIO_Init(P71F,P71_XOSCL_OUT_SETTING);

	CKCON |= XLCKE;									//外部低速时钟使能
	while(!(CKCON & XLSTA));				//等待时钟稳定	

    
	#ifdef PRINT_ENABLE
	Uart0_Initial(115200);
	#endif
	
	timer_init();//初始化语音定时器
	rtc_init();

    //adc_user_init(); 
    
    lcd_init();
    bl_init();
    ring_init();
    
    switch_ADC_init();

    test_5v_init();
    bat_ADC_init();
    
    switch_led_mod_init();

    motor_init();
	
	no_use_io_init();
    
	EA = 1;
    		
	while (1)
	{
        if(!lowspeed)
        {
            if (flag_10ms)
            {
                flag_10ms = 0; 
                rtc_alarm_opera();
                motor_pro();
                test_5v_in(); 
                key_process();
                lcd_display();
                key_no_action_test();
                low_vol_opera();  //低电量时电池图标闪烁
            }


            if (lcd_mode != mode_9v_power_on_wait)
            {
                if (flag_20ms)
                {
        			flag_20ms = 0;
                    bl_handler();
                    ring_vol_adjust_handler();
                    g_timer0_20ms_cnt = TIMER0_20MS_CNT;
                }
    			
    			
                if(flag_50ms)
        		{
        			flag_50ms = 0;								
                    bat_vol_test();													//测量电池电压
                }

                if (flag_200ms)
                {
                    flag_200ms = 0;
                    get_volume_and_mode();									//声音、震动等级 和 响铃模式测试
                    led_mod_detect();
                    //uart_printf("lcd_mode = %d\n",lcd_mode); 
                    //uart_printf("flag_alarm_timeout = %d\n",flag_alarm_timeout); 
                }
    			
    			
            	if(flag_second)										//----- 1 秒
        		{
        			flag_second = 0;					
        			snooze_alarm_test();										//贪睡闹钟测试
        			alarmed_time_test();										//闹钟响铃时间测试
        			nothing_action_test();	
        		}

               
            	if(flag_minute)										//----- 1 分
        		{
        			flag_minute = 0;			
        			get_calendar(); 												//更新日历时间		
        			alarm_test();														//闹钟测试
            	}

            }

        }

        else
        {
            test_5v_in_lowspeed();
        }
        
	}
}
#endif
