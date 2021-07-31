#ifndef _USER_TYPE_H_
#define _USER_TYPE_H_

#define enableInterrupts()    EA = 1	 /** 开启总中断	 **/
#define disableInterrupts()   EA = 0	 /** 关闭总中断	 **/
 
#define     __I     volatile const   /*!< defines 'read only' permissions     */
#define     __O     volatile         /*!< defines 'write only' permissions    */
#define     __IO    volatile         /*!< defines 'read / write' permissions  */
	
typedef unsigned char Byte;
typedef unsigned int Word;
	
#define HIBYTE(v1) ((Byte)(v1>>8))
#define LOBYTE(v1) ((Byte)((v1)&0xff))
#define MAKEWORD(v1,v2) ((((Word)(v1))<<8)+(Word)(v2))

/*!< Signed integer types  */
typedef   signed char     int8_t;
typedef   signed short    int16_t;
typedef   signed long     int32_t;

/*!< Unsigned integer types  */
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned long     uint32_t;

typedef int32_t  s32;
typedef int16_t  s16;
typedef int8_t   s8;

typedef uint32_t  u32;
typedef uint16_t  u16;
typedef uint8_t   u8;


typedef enum {FALSE = 0, TRUE = !FALSE} bool;

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus, BitStatus;

//typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

typedef enum {LOW = 0, HIGH = !LOW} PriorityStatus;

#endif