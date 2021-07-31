/* Includes ------------------------------------------------------------------*/
#include "bat.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/* global variables ----------------------------------------------------------*/


uint8_t lowspeed = 0;                       //低功耗运行标志
uint8_t time_to_enter_lowspeed = 0;			//进入低速模式的时间，为 0 时间进入低速模式

uint8_t flag_dc_5v = 1;									//1：5v电源接入




uint16_t low_power_test_time = 5000; //5s


static uint8_t dc_5v_timer = 0;

static uint8_t flag_low_vol = 0;								//电池低电标志

static uint8_t low_vol_time_cnt = 0;
static uint8_t test_ok_first_time = 0;      //防止重复发出开机 滴 声


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   test_5v_init
    @函数功能：   5v接入测试引脚初始化
    @入口参数：   设置为输入模式
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void test_5v_init(void)
{   
    GPIO_Init(DC_5V_PIN,   INPUT);

    #if 0
    GPIO_Init(DC_5V_PIN,					P42_ADC2_SETTING);
    ADCON = 0x3d;
    ADCFGL |= 0xc0;	 
    #endif
}


void test_5v_config_io_in(void)
{

}


void test_5v_config_ADC_in(void)
{
    
}

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   bat_ADC_init
    @函数功能：   电量检测引脚初始化
    @入口参数：   设置为AD模式
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void bat_ADC_init(void)
{
    GPIO_Init(BAT_ADC_PIN,				P41_ADC1_SETTING);		//ch1

    ADCON = 0x3d;
    ADCFGL |= 0xc0;	 
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   close_bat_adc
    @函数功能：   低功耗模式下关闭电量检测AD，设置为高阻
    @入口参数：   设置为AD模式
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void close_bat_adc(void)
{
    GPIO_Init(BAT_ADC_PIN,				HIGH_Z);		//ch1
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   test_5v_in
    @函数功能：   测试5V是否接入
    @入口参数：   接入为高电平
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void test_5v_in(void)
{
	static uint8_t test_ng_first_time = 0;		//防止重复设置进入低功耗时间
	static u16 usb_vol = 0;    

    if (lcd_mode != mode_9v_power_on_wait)
    {
        
        //usb_vol = GetChannelAdcValue(V_ADC_CH);

        if (DC_5V_GPIO) 
        //if(usb_vol >= 2500)	
    	{
    		if (++dc_5v_timer >= TEST_DC_5V_TIME) 
    		{
    			dc_5v_timer = TEST_DC_5V_TIME;
    			flag_dc_5v = 1;										//5vDC电源接入			
    			test_ng_first_time = 0;
    			if(!test_ok_first_time)						//第一次检查到 ,说明是5V上电
    			{
    				test_ok_first_time = 1;

                    lcd_mode = mode_9v_power_on_wait;       //进入上电等待模式
                    g_9v_power_on_wait_time = 500;          //等待500ms
    			}
    		}		
    	}	
    	/**************** ********************/
    	else
    	{	
    		dc_5v_timer = 0;
    		flag_dc_5v = 0;
    		test_ok_first_time = 0;	
    		bl_close();//关背光
    		
    		if(!test_ng_first_time)
    		{
    			test_ng_first_time = 1;
                enter_low_speed_mode();
                return;
    		}		
    	}



    }


}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   bat_vol_test
    @函数功能：   电池电量检测
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void bat_vol_test(void)
{
	if(GetChannelAdcValue(BAT_ADC_CH) <=  BAT_LOW_VOL)
	{
		if(++ low_vol_time_cnt >= 20)					//延时判断是否真正是低电
		{
			low_vol_time_cnt = 20;
			flag_low_vol = 1;
		}
	}
	else
	{
		low_vol_time_cnt = 0;
		flag_low_vol = 0;
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   low_vol_opera
    @函数功能：   低电时的操作
    @入口参数：   无
    @出口参数：   无
    @备注说明：   低电时闪烁电池图标
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void low_vol_opera(void)
{
	if(flag_low_vol)					//低电压时
	{
        lcd_symbol_deal(symbol_bat,flag_clockdot_show);		//低电压符号跟随时钟秒点闪烁	
	}
	else											//非 低电压时
	{
		lcd_symbol_deal(symbol_bat,HIDE);										//隐藏低电压符号
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   test_5v_in_lowspeed
    @函数功能：   低功耗时测试5v是否接入，如果接入，则唤醒
    @入口参数：   无
    @出口参数：   无
    @备注说明：   低电时闪烁电池图标
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void test_5v_in_lowspeed(void)
{
    #ifdef TIME_TEST_LOWPOWER_WAKEUP
    if (!low_power_test_time)
    {
        quit_low_speed_mode();//退出低功耗
        low_power_test_time = 5000;
        bl_pwm_mode = 0;
    }
    #endif

    #ifndef TIME_TEST_LOWPOWER_WAKEUP
	if(dc_5v_in)
	{
		quit_low_speed_mode();//退出低功耗
		time_to_enter_lowspeed = ENTER_LOWSPEED_TIME;			//进入低功耗倒计时 10s
		bl_pwm_mode = 0;
	}
	else
	{
		flag_dc_5v = 0;
	}
   #endif

}
/*********************************************************************************************************************


*********************************************************************************************************************/
void nothing_action_test(void)
{
    #ifdef TIME_TEST_LOWPOWER_WAKEUP
    if (!low_power_test_time)
    {
        enter_low_speed_mode();
    }
    #endif

    #ifndef TIME_TEST_LOWPOWER_WAKEUP
	if(!flag_dc_5v)					//只在 电池供电 情况下会 进入低功耗
	{
		if(time_to_enter_lowspeed == 0)
		{
			enter_low_speed_mode();
		}
	}
   #endif


}


void ring_amp_close(void)
{
    SCL_LOW;
	SDA_LOW;
    DAC_R_HIGH;
    V_AMP_CTL_PIN_LOW;
}




/*********************************************************************************************************************


*********************************************************************************************************************/
#if 0
void enter_low_speed_mode(void)
{

	GPIO_Init(MOTOR_AIN_PIN,HIGH_Z);        //电机引脚关闭
	GPIO_Init(MOTOR_BIN_PIN,HIGH_Z);
    	
	lcd_clear();

    bl_close();//关背光

    ring_close(); //关语音
    //ring_amp_close();
    close_switch_ADC_pin(); //拨码开关设置为IO，拉高

    close_bat_adc();        //关电池电压检测引脚

    close_key_ad();

    //GPIO_Init(DC_5V_PIN,					INPUT);

    TR0 = 0;
    ET0 = 0;

    TR1 = 0;
    ET1 = 0;
    
		
	
#if ((MOTOR_FREQ == PLL)	|| (SYSCLK_SRC == PLL))
	PLLCON &= 0x7f;												//PLL关闭，不再使用IRCH时钟
#endif		
//--------低速模式--------------

	I2CCON = 0; 
    CKCON |= ILCKE;
    Delay_ms(1);
	CKSEL = (CKSEL&0xF8) | CKSEL_IRCL;		//系统时钟选择 内部低速时钟 131KHz		
	CKCON = 0;								//关闭没被选择使用的其他所有时钟 ( )
	PWCON &= 0xf7;							//设置LDO进入低功率模式
    

//------------------------------
	lowspeed = 1;
test_ok_first_time = 0;

low_power_test_time = 50;

}
#endif





/*********************************************************************************************************************


*********************************************************************************************************************/
#if 0
void quit_low_speed_mode(void)
{	
#if ((MOTOR_FREQ == PLL)	|| (SYSCLK_SRC == PLL))	
	PLLCON |= 0xa0;													//PLL打开   6倍频 3686400 * 6 = 22118400hZ
	while(!(PLLCON & 0x01));								//等待PLL稳定
#endif	
	
	CKCON |= IHCKE;
	CKCON |= ILCKE;	//使能内部低速时钟，用于背光pwm										//IRCL时钟使能
	CKSEL = (CKSEL&0xF8) | CKSEL_IRCH;			//内部 IRCH 4M 时钟
	
	#if (SYSCLK_SRC == PLL)
	CKSEL = (CKSEL & 0XF8) | 4;							//系统时钟选择 PLL
	ADCFGL |= 0x60;													//ADC时钟 6 分频
	#endif	

	PWCON |= 0x08;													//LDO设置回高功率模式
	//------------------	

    TR0 = 1;
    ET0 = 1;
   
    //背光调节功能恢复    
    GPIO_Init(BL_ADC_DETECT_PIN, BL_ADC_DETECT_MOD);
    //5V检测
    //GPIO_Init(DC_5V_PIN,P42_ADC2_SETTING);
    //语音功能恢复

    //SCL_LOW;
	//SDA_LOW;
    
    SCL_HIGH;
	SDA_HIGH;
    
    GPIO_Init(DAC_R_PIN,OUTPUT);
    DAC_R_LOW;
    V_AMP_CTL_PIN_HIGH;
    GPIO_Init(RING_VOL_CTL_PIN, RING_VOL_CTL_MOD);     
    GPIO_Init(BUSY_PIN,INPUT);

    //拨码开关功能恢复
    GPIO_Init(MOTOR_SWITCH_ADC_PIN,			MOTOR_SWITCH_PIN_ADC_MODE);
    GPIO_Init(MODE_SWITCH_ADC_PIN,			MODE_SWITCH_PIN_ADC_MODE);

    //电池电量检测功能恢复
    GPIO_Init(BAT_ADC_PIN,				P41_ADC1_SETTING);	
		
	lowspeed = 0;//
}
#endif



void enter_low_speed_mode(void)
{

	GPIO_Init(MOTOR_AIN_PIN,HIGH_Z);        //电机引脚关闭
	GPIO_Init(MOTOR_BIN_PIN,HIGH_Z);
    	
	lcd_clear();

    bl_close();//关背光

    ring_close(); //关语音
    close_switch_ADC_pin(); //拨码开关设置为IO，拉高

    close_bat_adc();        //关电池电压检测引脚

    close_key_ad();

    TR0 = 0;
    ET0 = 0;

    TR1 = 0;
    ET1 = 0;
    
		
	
#if ((MOTOR_FREQ == PLL)	|| (SYSCLK_SRC == PLL))
	PLLCON &= 0x7f;												//PLL关闭，不再使用IRCH时钟
#endif		
//--------低速模式--------------
	I2CCON = 0; 
    CKCON |= ILCKE;
    Delay_ms(1);
	CKSEL = (CKSEL&0xF8) | CKSEL_IRCL;		//系统时钟选择 内部低速时钟 131KHz		
	CKCON = 0;								//关闭没被选择使用的其他所有时钟 ( )
	PWCON &= 0xf7;							//设置LDO进入低功率模式
//------------------------------
	lowspeed = 1;
test_ok_first_time = 0;

low_power_test_time = 50;

}





void quit_low_speed_mode(void)
{	
#if ((MOTOR_FREQ == PLL)	|| (SYSCLK_SRC == PLL))	
	PLLCON |= 0xa0;													//PLL打开   6倍频 3686400 * 6 = 22118400hZ
	while(!(PLLCON & 0x01));								//等待PLL稳定
#endif	
	
	CKCON |= IHCKE;
	CKCON |= ILCKE;	//使能内部低速时钟，用于背光pwm										//IRCL时钟使能
	CKSEL = (CKSEL&0xF8) | CKSEL_IRCH;			//内部 IRCH 4M 时钟
	
	#if (SYSCLK_SRC == PLL)
	CKSEL = (CKSEL & 0XF8) | 4;							//系统时钟选择 PLL
	ADCFGL |= 0x60;													//ADC时钟 6 分频
	#endif	

	PWCON |= 0x08;													//LDO设置回高功率模式
	//------------------	

    TR0 = 1;
    ET0 = 1;
   
    //背光调节功能恢复    
    GPIO_Init(BL_ADC_DETECT_PIN, BL_ADC_DETECT_MOD);
    
    //语音功能恢复

    SCL_HIGH;
	SDA_HIGH;
    
    GPIO_Init(DAC_R_PIN,OUTPUT);
    DAC_R_LOW;
    //V_AMP_CTL_PIN_HIGH;
    V_AMP_CTL_PIN_LOW;
    GPIO_Init(RING_VOL_CTL_PIN, RING_VOL_CTL_MOD);     
    GPIO_Init(BUSY_PIN,INPUT);




    

    //拨码开关功能恢复
    GPIO_Init(MOTOR_SWITCH_ADC_PIN,			MOTOR_SWITCH_PIN_ADC_MODE);
    GPIO_Init(MODE_SWITCH_ADC_PIN,			MODE_SWITCH_PIN_ADC_MODE);

    //电池电量检测功能恢复
    GPIO_Init(BAT_ADC_PIN,				P41_ADC1_SETTING);	
		
	lowspeed = 0;//
}









