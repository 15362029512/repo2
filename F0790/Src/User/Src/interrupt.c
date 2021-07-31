/* Includes ------------------------------------------------------------------*/
#include "interrupt.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/* global variables ----------------------------------------------------------*/





/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   timer0_isr
	@函数功能：   定时器0,1MS中断一次，用于基本定时
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void timer0_isr (void) interrupt 1 	
{
    static uint8_t cnt_10ms = 0;
    static uint8_t cnt_1ms = 0;
    static uint8_t cnt_20ms = 0;
    static uint8_t cnt_200ms = 0;
    static uint8_t cnt_2s = 0;
	/********* 正常模式 *************/
	if(!lowspeed)
	{
		#if 	(SYSCLK_SRC == IRCH)
		TL0 = 0xcc;								
		TH0 = 0xfe;								// 1ms
		#elif	(SYSCLK_SRC == PLL)
		TL0 = 0xcc;								
		TH0 = 0xf8;								// 1ms
		#endif

        if (low_power_test_time)
		{
            low_power_test_time--;
        }

        if (g_9v_power_on_wait_time)
        {
            g_9v_power_on_wait_time--;
        }

        if(++cnt_1ms >= 10)
		{
			cnt_1ms = 0;
			//========================= 10ms =====
			cnt_20ms++;
            //========================= 20ms =====
            if (cnt_20ms > 2)
            {
                cnt_20ms = 0;
                flag_20ms = 1;	
            }
			flag_10ms = 1;	
			if(set_no_flash_gap > 0)
			{
				if(--set_no_flash_gap == 0)
					flag_set_flash_en = 1;					//设置位重新开始闪烁
			}					
			
			//========================= 50ms =====
			if(++cnt_10ms >= 5)
			{
				cnt_10ms = 0;
				flag_50ms = 1;
				
				/***** 按键	*****/				
				if(key_no_action_cnt)		key_no_action_cnt--;	
			}
            //========================= 200ms =====
            if (++cnt_200ms > 20)
            {
                cnt_200ms = 0;
                flag_200ms = 1;
            }

            
            if (++cnt_2s > 200)
            {
                cnt_2s = 0;
                flag_2s = 1;
            }

            if (g_timer0_2s_cnt)
            {
                g_timer0_2s_cnt--;
            }

            if (ring_gap_time)
            {
                ring_gap_time--;
            }
            
		}	

        /*
		if (g_timer0_5ms_cnt)
		{
			g_timer0_5ms_cnt--;
		}
        if (g_timer0_10ms_cnt)
        {
            g_timer0_10ms_cnt--;
        }		
        if (g_timer0_20ms_cnt)
        {
            g_timer0_20ms_cnt--;
        }
        
        if (g_timer0_200ms_cnt)
        {
            g_timer0_200ms_cnt--;
        }
        if (g_timer0_2s_cnt)
        {
            g_timer0_2s_cnt--;
        }
        */
	}
	/********* 低速模式 *************/
	else						//低速模式下 141KHz 
	{
		TL0 = 0xbc;								
		TH0 = 0xfb;		//100ms	

		if (low_power_test_time)
		{
            low_power_test_time--;
        }
	}
}






/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   timer1_isr
	@函数功能：   定时器1中断处理，语音IC双线通信	
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void timer1_isr(void) interrupt 3			
{
	//if (!busy)
	//{
		ring_cmd_isr_handler();
	//}
		
	
   
}








/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   rtc_isr
	@函数功能：   rtc中断处理
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/

void rtc_isr (void) interrupt 13 	 
{
	static uint8_t test_flag = 0;
	//==================================================================
	if(RTCIF & RTC_HF)			//半秒中断
	{
		RTCIF = RTC_HF;	
//		flag_half_second = 1;	
		//时钟秒点闪烁
		flag_clockdot_show = !flag_clockdot_show;	
		
		//秒更新 
		if(flag_clockdot_show)
		{			
			flag_second = 1;
            
			if(time_to_enter_lowspeed) time_to_enter_lowspeed--;
		}
		if(RTCS == 0)			
		{
			test_flag = !test_flag;
			//分更新
			if(test_flag)
			{
				flag_minute = 1;
			}
		}		
	}	
}












