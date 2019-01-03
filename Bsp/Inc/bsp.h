#ifndef _BSP_H_
#define _BSP_H

#include "main.h"
#include "bsp_key.h"
#include "bsp_led_display.h"
#include "bsp_timer.h"
#include "bsp_uart.h"
#include "bsp_adc.h"
#include "bsp_dac.h"
#include "bsp_cpu_flash.h"
#include "bsp_crc16.h"
#include "bsp_led.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define bsp_printf		printf
//#define BSP_Printf(...)

void bsp_Init(void);
void bsp_Idle(void);
void bsp_RunPer10ms(void);
void bsp_RunPer500ms(void);

extern uint8_t usart1_rx_buffer_temp[];
extern uint8_t usart1_rx_buffer[];
extern uint16_t usart1_rx_len;
extern uint8_t usart1_rx_flag;

extern uint8_t usart2_rx_buffer_temp[];
extern uint8_t usart2_rx_buffer[];
extern uint16_t usart2_rx_len;
extern uint8_t usart2_rx_flag;

extern volatile uint16_t ADC_ConvertedValue[];
extern uint32_t ADC_Average[];

extern volatile uint8_t g_blinkFlag_500ms;

uint16_t BEBufToUint16(uint8_t *_pBuf);

#endif