/* Includes ------------------------------------------------------------------*/
#include "bl.h"
#include "uart.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/









/* Private functions ---------------------------------------------------------*/
static void bl_printf(void);
static void get_bl_PWM_duty_value(uint16_t * bl_PWM_duty);
static void bl_param_init(void);
static void set_bl_param(adjust_param_type * bl_param,uint16_t _max_bl_AD_value);
static void bl_adjust_AD_init(void);



/* Private variables ---------------------------------------------------------*/
static uint16_t current_bl_AD_value = 0;   //当前背光旋钮的值
static uint16_t before_bl_AD_value = 0;    //上次背光旋钮的值


static uint16_t max_bl_AD_value = 3269;    //当前背光调节的最大AD值
static adjust_param_type bl_param = {0,0};



/* global variables ----------------------------------------------------------*/
uint16_t AD_value_buffer[AD_BUFF_LEN] = {0};
uint16_t bl_PWM_duty = 0;         //背光占空比




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   bl_handler
    @函数功能：   背光调节处理
    @入口参数：   无
    @出口参数：   无
    @备注说明：   防止最小亮度时闪烁：当AD值小于5时，直接认为拨到0了，当做0处理，如果大于5，按推导公式求出来的如果等于0，则按最小亮度处理
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void bl_handler(void)
{
    uint8_t set_bl_duty_flg = 0; //是否需要设置背光占空比   
    uint16_t bl_AD_differ_value = 0;       //前一次与上一次AD值的差值
    #if (AD_FILTER == 0)
        current_bl_AD_value = GetChannelAdcValue(BL_ADC_DETECT_ADCCH);  //直接获取AD值
    #elif (AD_FILTER == 1)
        current_bl_AD_value = get_adjust_AD_value(BL_ADC_DETECT_ADCCH); //获取滤波后的AD值
    #endif
    
    bl_AD_differ_value = get_dvalue_u16(current_bl_AD_value,before_bl_AD_value); //对比当前值和上一次的值求出差值

    if (current_bl_AD_value < 5)  //如果AD值小于5直接关闭背光
    {
        current_bl_AD_value = 0;
        if (!set_bl_duty_flg)
        {
            bl_brightness_set(0);
            set_bl_duty_flg = 1;
        }
        
    }

    if (bl_AD_differ_value >= 5) //如果前一次与后一次的差值大于5，则认为是手动调节
    {
        set_bl_duty_flg = 0;
        if (current_bl_AD_value <= max_bl_AD_value) //如果当前值小于等于最大AD值，则以之前的对应关系计算亮度
        {
			//do noting
        }
        else
        {
            if ((current_bl_AD_value - max_bl_AD_value) > 20) //如果当前值大于最大值20个D
            {
				 if (current_bl_AD_value != 0xFFF)
				 {
					 max_bl_AD_value = current_bl_AD_value; //重新设定最大值
					 set_bl_param(&bl_param,max_bl_AD_value); //以当前的最大值设定kb参数
				 }
            }
        }
        
        get_bl_PWM_duty_value(&bl_PWM_duty); //根据kb设定亮度
        bl_brightness_set(bl_PWM_duty);    //设定亮度到寄存器
        before_bl_AD_value = current_bl_AD_value;
        
    }
	//bl_printf();
}
    
   



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   bl_printf
    @函数功能：   用来打印背光信息
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void bl_printf(void)
{
	#ifdef PRINT_ENABLE
    uart_printf("current_bl_AD_value = %d\n",current_bl_AD_value);
    uart_printf("bl_PWM_duty = %d\n",bl_PWM_duty);
	#endif
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   get_bl_PWM_duty_value
    @函数功能：   获取背光占空比的值
    @入口参数：   无
    @出口参数：   bl_PWM_duty
    @备注说明：   根据AD值和 bl_param 的参数计算bl_PWM_duty,
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void get_bl_PWM_duty_value(uint16_t * bl_PWM_duty)
{

    #if (PWM_DUTY_ADJUST_MODE == LINEAR_ADJUST)  
        *bl_PWM_duty = (uint16_t)(bl_param.k * current_bl_AD_value + bl_param.b);
        if (*bl_PWM_duty > BL_BRIGHTNESS_MAX)
        {
            *bl_PWM_duty = BL_BRIGHTNESS_MAX;
        }
    #elif (PWM_DUTY_ADJUST_MODE == CURVILINEAR_ADJUST)
        double temp_max_bl_AD_value = (double)current_bl_AD_value/100;
        double temp_max_bl_duty = 0;
        temp_max_bl_duty = (temp_max_bl_AD_value * temp_max_bl_AD_value * temp_max_bl_AD_value) * bl_param.k;
        *bl_PWM_duty = (uint16_t)(temp_max_bl_duty * 100);
    #endif
    if (*bl_PWM_duty == 0)
    {
        *bl_PWM_duty = 1;
    }
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   bl_param_init
    @函数功能：   以最大ad值3269来计算，K,B
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void bl_param_init(void)
{
    set_bl_param(&bl_param,max_bl_AD_value);
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   set_bl_param
    @函数功能：   根据当前背光调节的最大值，重新算出背光参数
    @入口参数：   无
    @出口参数：   无
    @备注说明：   
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void set_bl_param(adjust_param_type * bl_param,uint16_t _max_bl_AD_value)
{   
    #if (PWM_DUTY_ADJUST_MODE == LINEAR_ADJUST)    
    
        bl_param->k = (float)BL_BRIGHTNESS_MAX/(float)_max_bl_AD_value;
        bl_param->b = 0;
    
    #elif (PWM_DUTY_ADJUST_MODE == CURVILINEAR_ADJUST)

        double temp_max_bl_AD_value = 0;
        double temp_max_bl_duty = 0;
        
        temp_max_bl_AD_value = (double)_max_bl_AD_value/100;
        temp_max_bl_duty = (double)(BL_BRIGHTNESS_MAX);
        temp_max_bl_duty = (temp_max_bl_duty/100);
        bl_param->k = temp_max_bl_duty/(temp_max_bl_AD_value * temp_max_bl_AD_value * temp_max_bl_AD_value);
    #endif
}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   bl_init
    @函数功能：   bl_init(PWM4,100HZ)
    @入口参数：   无
    @出口参数：   无
    @备注说明：   要点亮lcd必须使能背光，PWM时钟源使用的是IRCL，配置前必须使能IRCL
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void bl_init(void)
{
	#if (BL_CTRL_MOD == BL_IO_CTRL)
        GPIO_Init(BL_CTL_PIN,OUTPUT);
        BL_CTL_GPIO = 1;

    #elif (BL_CTRL_MOD == BL_PWM_CTRL)
        GPIO_Init(BL_CTL_PIN,BL_MOD);

        INDEX = BL_PWMCH;	
        PWMCON = 0x02;					  //PWM4 时钟:	IRCH = 131khz,PWM4 100HZ
        PWMDIVH	= (uint8_t)(BL_PWM_DivDat >> 8);
        PWMDIVL	= (uint8_t)BL_PWM_DivDat;
        PWMDUTL = 0;
        PWMDUTH = 0;

        PWMUPD |= (1 << BL_PWMCH);   //更新 PWM4 参数	
        while(PWMUPD);              //等待更新完成
        PWMEN |= (1 << BL_PWMCH);  //使能 PWM4

        bl_adjust_AD_init();   //初始化亮度AD采集

        current_bl_AD_value = get_adjust_AD_value(BL_ADC_DETECT_ADCCH); //获取AD值
        before_bl_AD_value = current_bl_AD_value;
        if (current_bl_AD_value > max_bl_AD_value)
        {
            if (current_bl_AD_value != 0xFFF)
            {
                 max_bl_AD_value = current_bl_AD_value;
            }
           
        }
        bl_param_init(); //以最大值设定参数
        get_bl_PWM_duty_value(&bl_PWM_duty); //根据参数设定初始化亮度
        //bl_brightness_set(bl_PWM_duty);    //LCD由 DC 供电,设定亮度到寄存器
    #endif

}

uint8_t bl_pwm_mode;


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   bl_brightness_set
    @函数功能：   设置背光亮度
    @入口参数：   bl_duty:亮度占空比
    @出口参数：   无
    @备注说明：   范围0~1300
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void bl_brightness_set(uint16_t bl_duty)
{
    if (!bl_pwm_mode)
    {
        GPIO_Init(BL_CTL_PIN,BL_MOD);
        bl_pwm_mode = 1;
    }
    INDEX = BL_PWMCH;
    PWMDUTH = (uint8_t)(bl_duty >> 8);
    PWMDUTL = (uint8_t)bl_duty;
    
    PWMUPD |= (1 << BL_PWMCH);   //更新 PWM4 参数	
    while(PWMUPD);              //等待更新完成
    PWMEN |= (1 << BL_PWMCH);  //使能 PWM4
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   bl_adjust_AD_init
    @函数功能：   初始化背光调节旋钮,
    @入口参数：   无
    @出口参数：   无
    @备注说明：   P44 ADC4 初始化前必须配置ADC
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void bl_adjust_AD_init(void)
{    
    GPIO_Init(BL_ADC_DETECT_PIN, BL_ADC_DETECT_MOD);      //ch5,背光调节检测
    ADCON = 0x3d;
    //ADCFGL |= 0X60;				//ADC时钟6分频 
    ADCFGL |= 0xc0;	
}



void bl_close(void)
{
    GPIO_Init(BL_CTL_PIN,OUTPUT);
    BL_CTL = 0;

    GPIO_Init(BL_ADC_DETECT_PIN,HIGH_Z);
}




/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   get_adjust_AD_value
    @函数功能：   获取背光调节旋钮值
    @入口参数：   ADC_Channel_TypeDef 对应的AD通道
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
uint16_t get_adjust_AD_value(ADC_Channel_TypeDef adc_chx)
{
    uint8_t i = 0;
    uint32_t sum = 0;
    for (i = 0; i < AD_BUFF_LEN; i++)
    {
        AD_value_buffer[i] = GetChannelAdcValue(adc_chx);
        sum += AD_value_buffer[i];		
    }
    sum /= AD_BUFF_LEN;
    return (uint16_t)sum;
}





/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   get_dvalue_u16
	@函数功能：   获取两个数的差值
	@入口参数：   无  
	@出口参数：   differ_value差值，如果value1 > value2 返回max_flg = 1
	@备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
uint16_t get_dvalue_u16(uint16_t value1,uint16_t value2)
{
    if (value1 > value2)
    {
        return (value1 - value2);
    }
    return (value2 - value1);
}






/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   get_dvalue_u8
	@函数功能：   获取两个数的差值
	@入口参数：   无  
	@出口参数：   differ_value差值，如果value1 > value2 返回max_flg = 1
	@备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
uint8_t get_dvalue_u8(uint8_t value1,uint8_t value2)
{
    if (value1 > value2)
    {
        return (value1 - value2);
    }
    return (value2 - value1);
}















