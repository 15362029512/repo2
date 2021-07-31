/* Includes ------------------------------------------------------------------*/
#include "key.h"
#include "uart.h"

        
/* Private variables ---------------------------------------------------------*/
//按键AD值从小到大
static uint16_t key_AD_value_list[KEY_COUNT] =  {
    KEY_12_24H_SET_AD_VAL,KEY_SNOOZE_AD_VAL,
    KEY_UP_DOWM_DOUBLE_AD_VAL,KEY_UP_AD_VAL,
    KEY_ALARM_SET_AD_VAL,KEY_ALARM1_AD_VAL,
    KEY_DOWN_AD_VAL,KEY_ALARM2_AD_VAL
}; //按键的AD值


static KEY_T s_tBtn[KEY_COUNT] = {0}; //不同按键的参数

static KEY_FIFO_T s_tKey;       // 按键FIFO变量,结构体 

static uint16_t current_key_AD_value = 0; //当前按键的AD值



/* global variables ----------------------------------------------------------*/
uint8_t ucKeyCode = 0;      // 按键返回值 






/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   key_ad_init
    @函数功能：   初始化AD按键 
    @入口参数：   无
    @出口参数：   无
    @备注说明：   adch5
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void key_ad_init(void)
{
    GPIO_Init(KEY_AD_PIN, KEY_AD_MOD);      
	ADCON = 0x3d;
    //ADCFGL |= 0X60;		
    ADCFGL |= 0xc0;	
    bsp_InitKeyVar();
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   close_key_ad
    @函数功能：   按键AD设置为高阻态 
    @入口参数：   无
    @出口参数：   无
    @备注说明：   adch5
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void close_key_ad(void)
{
    GPIO_Init(KEY_AD_PIN, HIGH_Z);   
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   bsp_InitKeyVar
    @函数功能：   初始化按键参数，长按，短按，长按重复触发时间
    @入口参数：   无
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void bsp_InitKeyVar(void)
{
	uint8_t i;

	/* 对按键FIFO读写指针清零 */
	s_tKey.Read = 0;
	s_tKey.Write = 0;

	/* 给每个按键结构体成员变量赋一组缺省值 */
	for (i = 0; i < KEY_COUNT; i++)
	{
		s_tBtn[i].LongTime = KEY_LONG_TIME;			/* 长按时间 0 表示不检测长按键事件(如果需要区分长按抬起和短按抬起则必须设置长按(程序上设计的s_tBtn[i].LongTime > 0时 s_tBtn[i].Count才会累加)) */
        s_tBtn[i].LongCount = 0;	
        s_tBtn[i].Count = KEY_FILTER_TIME / 2;		/* 计数器设置为滤波时间的一半 */
		s_tBtn[i].State = 0;							/* 按键缺省状态，0为未按下 */
		s_tBtn[i].RepeatSpeed = KEY_REPEAT_TIME;		/* 按键连发的速度，0表示不支持连发 */
		s_tBtn[i].RepeatCount = 0;						/* 连发计数器 */
	}
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   KeyPinActive
    @函数功能：   按键是否处于按下状态
    @入口参数：   无
    @出口参数：   1有按键 0无按键
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static uint8_t KeyPinActive(uint8_t index)
{
    if (current_key_AD_value < key_AD_value_list[index])
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   IsKeyDownFunc
    @函数功能：   如果有按键按下，获取按下的按键的按键值
    @入口参数：   id：按键的编号顺序
    @出口参数：   按键值
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static uint8_t IsKeyDownFunc(uint8_t _id)
{
	uint8_t i;
	uint8_t count = 0;
	uint8_t save = 255;
	if (_id < KEY_COUNT)
	{
        for (i = 0; i < KEY_COUNT; i++)
		{
            if (KeyPinActive(i))
			{
				count++;
				save = i;
			}
			if (count == 1 && save == _id)
			{
				return 1;	// 只有1个键按下时才有效 
			}
		}
        return 0;
	}
	return 0;		
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   bsp_GetKey
    @函数功能：   从按键FIFO缓冲区读取一个键值。
    @入口参数：   无
    @出口参数：   按键代码
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
uint8_t bsp_GetKey(void)
{
	uint8_t ret;

	if (s_tKey.Read == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read];

		if (++s_tKey.Read >= KEY_FIFO_SIZE)
		{
			s_tKey.Read = 0;
		}
		return ret;
	}
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   bsp_PutKey
    @函数功能：   将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
    @入口参数：   _KeyCode : 按键代码
    @出口参数：   无
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void bsp_PutKey(uint8_t _KeyCode)
{
	s_tKey.Buf[s_tKey.Write] = _KeyCode;

	if (++s_tKey.Write  >= KEY_FIFO_SIZE)
	{
		s_tKey.Write = 0;
	}
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   bsp_DetectKey
    @函数功能：   检查按键是否真的按下，消抖，如果按键真的按下，将按键的动作压入缓冲区
    @入口参数：   i:按键的编号顺序
    @出口参数：   按键值
    @备注说明：   无
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
static void bsp_DetectKey(uint8_t i)
{
    //static uint8_t _count = 0;
	KEY_T *pBtn;
	uint8_t save = 0;
	pBtn = &s_tBtn[i];

	if (IsKeyDownFunc(i))       //如果按键按下了
	{
        
		if (pBtn->Count < KEY_FILTER_TIME) // s_tBtn[i].Count = KEY_FILTER_TIME / 2;		// 计数器初值设置为滤波时间的一半 
		{
			pBtn->Count = KEY_FILTER_TIME;      
		}                                       
		                                                  
		else if(pBtn->Count < 2 * KEY_FILTER_TIME)
		{
			pBtn->Count++;
		}                                
		else
		{
			if (pBtn->State == 0)
			{
				pBtn->State = 1;

				// 发送按钮按下的消息(短按) 
				bsp_PutKey((uint8_t)(5 * i + 1));
			}

			if (pBtn->LongTime > 0)                         //如果设置了长按
			{
				if (pBtn->LongCount < pBtn->LongTime)       //如果长按计数值小于设定的长按时间
				{
					// 发送按钮持续按下的消息 
					if (++pBtn->LongCount >= pBtn->LongTime) //则累加长按计数值，直到计数值等于长按时间
					{
						// 键值放入按键FIFO(第一次长按) 
						bsp_PutKey((uint8_t)(5 * i + 3));
					}
				}                                           //如果长按计数值 >= 长按时间，则每隔一段时间发送短按值
				else
				{
					if (pBtn->RepeatSpeed > 0)
					{
						if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
						{
							pBtn->RepeatCount = 0;
							// 长按键后，每隔200ms发送1个按键(持续长按) 
							bsp_PutKey((uint8_t)(5 * i + 4));
						}                        
					}
				}
			}
		}
	}
	else                //如果按键松手了
	{
		if(pBtn->Count > KEY_FILTER_TIME)   //如果超过了滤波值，则等于滤波值
		{                                                           
			pBtn->Count = KEY_FILTER_TIME;                               
		}                                   
		else if(pBtn->Count != 0)          //如果没超过则计数值--(松手检测)
		{
			pBtn->Count--;
		}
		else                                //隐藏条件是 else if (pBtn->Count == 0)                                  
		{       
			if (pBtn->State == 1)           //如果之前的状态标志位是1，(之前是按下的)
			{
				pBtn->State = 0;            //状态标志位清0

				// 发送按钮弹起的消息 
				if (pBtn->LongCount >= pBtn->LongTime)
				{
					pBtn->LongCount = 0;
					bsp_PutKey((uint8_t)(5 * i + 5));       //长按抬起
				}
				else
				{
					pBtn->LongCount = 0;
					bsp_PutKey((uint8_t)(5 * i + 2));       //短按抬起
				}
			}
		}
		pBtn->RepeatCount = 0;
	}
}



/*------------------------------------------------------------------------------------------------------------------------------------------*/
/***@函数名称：   bsp_KeyScan10ms
    @函数功能：   按键扫描
    @入口参数：   无
    @出口参数：   无
    @备注说明：   定时器10ms调用一次
*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
void bsp_KeyScan10ms(void)
{
	uint8_t a;
	current_key_AD_value = GetChannelAdcValue(KEY_AD_CH);	//获取当前按键AD值
	uart_printf("current_key_AD_value = %d\n",current_key_AD_value); 
	for (a = 0; a < KEY_COUNT; a++)
	{
		bsp_DetectKey(a);
	}
}






