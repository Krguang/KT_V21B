#ifndef __BSP_UART
#define __BSP_UART

void bsp_InitUart(void);
void UsartReceive_IDLE(UART_HandleTypeDef *huart);

void usart1_dma_send(unsigned char *buffer, unsigned int length);
void usart2_dma_send(unsigned char *buffer, unsigned int length);

#endif // !__BSP_UART
