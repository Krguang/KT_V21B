#ifndef __BSP_UART
#define __BSP_UART

void bsp_InitUart(void);
void UsartReceive_IDLE(UART_HandleTypeDef *huart);

#endif // !__BSP_UART
