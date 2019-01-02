#include "bsp.h"
#include "modbus_slave.h"
#include "controlCenter.h"

uint8_t usart1_rx_buffer_temp[128];
uint8_t usart1_rx_buffer[128];
uint16_t usart1_rx_len = 0;
uint8_t usart1_rx_flag = 0;

uint8_t usart2_rx_buffer_temp[128];
uint8_t usart2_rx_buffer[128];
uint16_t usart2_rx_len = 0;
uint8_t usart2_rx_flag = 0;

volatile uint16_t ADC_ConvertedValue[3];
uint32_t ADC_Average[3];

volatile uint8_t g_blinkFlag_500ms;


/*
*********************************************************************************************************
*	函 数 名: BEBufToUint16
*	功能说明: 将2字节数组(大端Big Endian次序，高字节在前)转换为16位整数
*	形    参: _pBuf : 数组
*	返 回 值: 16位整数值
*
*   大端(Big Endian)与小端(Little Endian)
*********************************************************************************************************
*/
uint16_t BEBufToUint16(uint8_t *_pBuf)
{
	return (((uint16_t)_pBuf[0] << 8) | _pBuf[1]);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_Init
*	功能说明: 初始化硬件设备。只需要调用一次。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。
*			 全局变量。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Init(void)
{
	bsp_InitKey();		/* 按键要在定时器前初始化，不然定时器开始扫描按键时，却没有初始化 */
	bsp_InitTimer();	/* 定时器初始化 */
	bsp_InitUart(); 	/* 初始化串口 */
	bsp_InitADC();		/* 初始化adc */
	bsp_InitDAC();		/* 初始化dac */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_RunPer10ms
*	功能说明: 该函数每隔10ms被Systick中断调用1次。详见 bsp_timer.c的定时中断服务程序。一些处理时间要求不严格的
*			任务可以放在此函数。比如：按键扫描、蜂鸣器鸣叫控制等。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RunPer10ms(void)
{
	bsp_KeyScan();		/* 按键扫描 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_RunPer500ms
*	功能说明: 该函数每隔500ms被Systick中断调用1次。详见 bsp_timer.c的定时中断服务程序。用于数码管和LED的闪烁
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RunPer500ms(void)
{
	g_blinkFlag_500ms = g_blinkFlag_500ms ^ 1;
	tempSetCount++;
	humiSetCount++;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_Idle
*	功能说明: 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
*			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Idle(void)
{
	MODS_Poll();	/* 从站 MODBUS函数 */
	modeSelect();
}