/* Includes ------------------------------------------------------------------*/
#include "ring.h"
#include "uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 


#define VOL_MAX_LEVEL 15 //(静音——(F0~FF)最大声音一共有17个)


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint16_t current_ring_vol_AD_value = 0;     //当前声音旋钮的值
static uint16_t before_ring_vol_AD_value = 0;     //上次声音旋钮的值
static uint16_t max_ring_vol_AD_value = 3269;     //最大声音的AD值  


uint8_t spk_cmd_ing = 0;          //正在发送命令
static uint8_t spk_cmd_step = 0;         //发送命令的步骤
static uint8_t spk_time_cnt = 0;         
static uint8_t cmd_bit_cnt = 0;


//不同的音量等级，将450划分为15段，每一段站不同的比重:90,90,40,40,35,35,35,25,20,5,5,5,5,5,15
static uint16_t vol_level_percent_array[VOL_MAX_LEVEL] = {90,180,220,260,295,330,365,390,410,415,420,425,430,435,450};



/* global variables ----------------------------------------------------------*/
uint8_t g_cmd = cmd_ring_stop;				//通信时要发送的命令		
uint8_t need_replay = 0;
uint8_t g_alarm_ring_index	= 0x02;			//闹钟选择的铃声	

uint8_t g_current_ring_vol_level = 0XF0;     //当前声音的等级  

uint8_t alarm_ring_cycle = 2;								//一个循环里 滴 的次数


uint8_t ring_gap_time = 0;



/* Private functions ---------------------------------------------------------*/
static uint8_t get_current_vol_level(void);
static void ring_vol_printf(void);
static void ring_vol_ad_init(void);



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   ring_vol_ad_init
    @函数功能：   初始化音量检测引脚为ADCH3
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void ring_vol_ad_init(void)
{
    GPIO_Init(RING_VOL_CTL_PIN, RING_VOL_CTL_MOD);      //ch3,语音调节检测
	ADCON = 0x3d;
    //ADCFGL |= 0X60;				//ADC时钟6分频 
    ADCFGL |= 0xc0;	
}

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   ring_init
    @函数功能：   初始化语音相关引脚，语音IC和音量检测，并给初始化音量
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/

void ring_init(void)
{
	GPIO_Init(BUSY_PIN,INPUT);
	GPIO_Init(SCL_PIN,OUTPUT);
	GPIO_Init(SDA_PIN,OUTPUT);
    GPIO_Init(DAC_R_PIN,OUTPUT);
    GPIO_Init(V_AMP_CTL_PIN,OUTPUT);

    /*
	SCL_LOW;
    SDA_LOW;
    */

	SCL_HIGH;
    SDA_HIGH;

    DAC_R_LOW;
    V_AMP_CTL_PIN_LOW;
   

    ring_vol_ad_init();
    current_ring_vol_AD_value = get_adjust_AD_value(RING_VOL_CTL_ADCCH);
	if (current_ring_vol_AD_value > max_ring_vol_AD_value)
	{
		max_ring_vol_AD_value = current_ring_vol_AD_value;
	}
    before_ring_vol_AD_value = current_ring_vol_AD_value;
	
    g_current_ring_vol_level = get_current_vol_level();    //获取初始声音等级
	//g_current_ring_vol_level = 0xF0;
}


void ring_close(void)
{
    
    GPIO_Init(BUSY_PIN,HIGH_Z);
    SCL_LOW;
	SDA_LOW;
    GPIO_Init(DAC_R_PIN,HIGH_Z);
    GPIO_Init(RING_VOL_CTL_PIN, HIGH_Z);      //语音调节检测关闭
    DAC_R_HIGH;
    V_AMP_CTL_PIN_LOW;
    
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   get_current_vol_level
    @函数功能：   获取当前音量等级
    @入口参数：   无
    @出口参数：   无
    @备注说明：   一共15个等级 音量0xf0~0xff,0xef为关闭音量
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static uint8_t get_current_vol_level(void)
{
    float temp_value = 0;
	float temp_level = 0;
	uint16_t _level = 0;
    uint8_t i = 0;
    uint16_t dvalue = 0;
    uint16_t dvalue_min = 450;
    uint8_t dvalue_min_index = 0;

	if (current_ring_vol_AD_value != 0)
	{
		temp_value = (float)max_ring_vol_AD_value/(float)(VOL_MAX_LEVEL * 30); //将当前最大量程分为450份
		temp_value = (uint16_t)(temp_value + 0.5);                   //求出当前的AD大约占多少份
		temp_level = (float)current_ring_vol_AD_value/temp_value; 
		_level = (uint16_t)(temp_level + 0.5);                   //求出当前的AD大约占多少份

        //遍历数据，求出与当前份数最接近的
        for (i = 0; i < 15; i++)
        {
            dvalue = get_dvalue_u16(_level,vol_level_percent_array[i]);
            if (dvalue < dvalue_min)
            {
                dvalue_min_index = i;
                dvalue_min = dvalue;
            }
        }
        dvalue_min_index++;
		dvalue_min_index += 0xF0;
	}		
    return dvalue_min_index;
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   ring_vol_adjust_handler
    @函数功能：   音量等级调节处理
    @入口参数：   无
    @出口参数：   无
    @备注说明：   主函数20ms调用一次
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void ring_vol_adjust_handler(void)
{
    
    uint16_t ring_vol_AD_differ_value = 0;       //前一次与上一次AD值的差值

    #if (AD_FILTER == 0)
        current_ring_vol_AD_value = GetChannelAdcValue(RING_VOL_CTL_ADCCH);  //直接获取AD值
    #elif (AD_FILTER == 1)
        current_ring_vol_AD_value = get_adjust_AD_value(RING_VOL_CTL_ADCCH); //获取滤波后的AD值
    #endif

    if (current_ring_vol_AD_value == 0)
	{
		g_current_ring_vol_level = 0xF0;   //最小声音
	}

    else 
    {        
        ring_vol_AD_differ_value = get_dvalue_u16(current_ring_vol_AD_value,before_ring_vol_AD_value); //对比当前值和上一次的值求出差值

        if (ring_vol_AD_differ_value >= 5) //两次相差大于10，则认为是手动调节
        {
            if (current_ring_vol_AD_value <= max_ring_vol_AD_value)
        	{
        		//do nothing
        	}
        	else
        	{
        		if ((current_ring_vol_AD_value - max_ring_vol_AD_value ) > 20)
        		{
        			 if (current_ring_vol_AD_value != 0xFFF)
        			 {
        				max_ring_vol_AD_value = current_ring_vol_AD_value;
        			 }      			 
        		}
        	}
        	g_current_ring_vol_level = get_current_vol_level();
        	before_ring_vol_AD_value = current_ring_vol_AD_value;
        	//ring_vol_printf();
        }
    }
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   ring_vol_printf
    @函数功能：   打印当前的音量信息
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void ring_vol_printf(void)
{
	#ifdef PRINT_ENABLE
    uart_printf("current_ring_vol_AD_value = %d\n",current_ring_vol_AD_value);
    uart_printf("g_current_ring_vol_level = %d\n",g_current_ring_vol_level);
	#endif
}

uint8_t addr = 0;


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   send_ring_cmd
	@函数功能：   发送语音命令	
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void send_ring_cmd(uint8_t CMD)
{	
	if (!spk_cmd_ing)
	{
        V_AMP_CTL_PIN_HIGH;    
        SCL_HIGH;
		g_cmd = CMD;
		need_replay = 1;
		cmd_bit_cnt = 0;
		//SCL_HIGH;                 //拉高 时钟线 400us
		spk_time_cnt = 4;
	  
		spk_cmd_ing = 1;
		spk_cmd_step = 0;
		

	#if (SYSCLK_SRC == IRCH)  //  3686400/12 = 307200
		TH1 = 0xe1;
		TL1 = 0xe1;     //(0xff - 0xe1) = 30 / 307200 = 1/10000s = 100us
	#elif (SYSCLK_SRC == PLL) //  22118400 / 12 = 1843200
		TH1 = 0x47;
		TL1 = 0x47;     //(0xff - 0x47) = 184 / 1843200 = 1/10000s = 100us
	#endif
		TR1 = 1;
		ET1 = 1;
		if (g_cmd != cmd_ring_stop)
        {
            send_ring_cmd_step++;
        }		
	}
}



uint8_t sig_cnt = 0;



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   ring_cmd_isr_handler
	@函数功能：   语音命令中断处理程序	
    @入口参数：   无
    @出口参数：   无
    @备注说明：   timer1 100us调用
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void ring_cmd_isr_handler(void)
{
    if (spk_cmd_ing)
	{
		if (spk_time_cnt)
		{
			spk_time_cnt--;
		}
		
		switch(spk_cmd_step)
		{
			case 0:
			{
				if (!spk_time_cnt) //如果时钟线拉高了400us
				{
					spk_cmd_step = 1;         //判断数据线是拉高还是拉低
				}
			}
			break;

			case 1:
			{
                if (!spk_time_cnt)
                {
                    if(g_cmd & (0x80 >> cmd_bit_cnt))   //判断高位
    				{
    					SDA_HIGH;
    				}
    				else
    				{
    					SDA_LOW;
    				}
                    SCL_LOW;
                    spk_cmd_step = 2;
                    spk_time_cnt = 4;                    
                }

			}
			break;

			case 2:
			{
				if (!spk_time_cnt)
				{
					cmd_bit_cnt++;
					spk_cmd_step = 3; 
					spk_time_cnt = 4;
                    SCL_HIGH;

				}

			}
			break;
			
			case 3:
			{
				if (!spk_time_cnt)
				{
					if (cmd_bit_cnt > 7)
					{
						cmd_bit_cnt = 0;
						spk_cmd_ing = 0;
						spk_cmd_step = 0;

                        
                        if (g_cmd == cmd_ring_stop)
                        {
                            V_AMP_CTL_PIN_LOW;
                            //SCL_LOW;
                            
                        }/*
                        if (g_cmd != cmd_ring_stop)
                        {
                            send_ring_cmd_step++;
                        }
                        if ((send_ring_cmd_step >= 2)||(g_cmd == cmd_ring_stop))
                        {
                            send_ring_cmd_step = 0;
                        }
                        */
					}
					else
					{
						spk_cmd_step = 1;
					}
				}
			}     
			break;            
            default:break;
		}	
	}
}






void turn_on_tone(void)
{

}



void send_byte(uint8_t addr)
{
    uint8_t i;
    SCL_HIGH;
    Delay_50us(8);
    for(i=0;i<8;i++)
    {
        if(addr&0x80)
        {
            SDA_HIGH;
        }
        
        else
        {
             SDA_LOW;
        }     
        SCL_LOW;
        Delay_50us(8);
        SCL_HIGH;
        Delay_50us(8);
        addr<<=1;
    } 
}

















uint8_t send_once_flg = 0;
uint8_t send_ring_cmd_step = 0;  // step = 0,发送音量  step = 1 发送语音命令  step = 2，延时




void alarm_tone(void)
{
    if (!busy)
    {
        if (!ring_gap_time)
        {
			if (send_ring_cmd_step == 0)
			{
				send_ring_cmd(g_current_ring_vol_level);
			}
            if (send_ring_cmd_step == 1)
			{
				 send_ring_cmd(g_alarm_ring_index);
			}
			if (send_ring_cmd_step >= 2)
			{
				ring_gap_time = 200;
			   send_ring_cmd_step = 0;
			}
        }

    }
}


uint8_t ring_stop_flg = 0;

void ring_stop(void)
{
   
    if (g_cmd != cmd_ring_stop)
    {
		#if 0
		SCL_HIGH;                 //拉高 时钟线 400us
		SDA_HIGH;
		#endif
		
		
		
        if (!busy)
    	{       
    		send_ring_cmd(cmd_ring_stop);
            ring_gap_time = 0;
    		ring_stop_flg = 0;
    	}
		
    }
}

