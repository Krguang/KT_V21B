#include "bsp.h"
#include "usart.h"

int _write(int fd, char *pBuffer, int size)
{
	HAL_UART_Transmit(&huart2, pBuffer, size, 0xff);
	return size;
}

void bsp_InitUart(void)
{
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);    //使能空闲中断
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);    //使能空闲中断

	if (HAL_UART_Receive_DMA(&huart1, (uint8_t *)&usart1_rx_buffer_temp, 128) != HAL_OK)    Error_Handler();
	if (HAL_UART_Receive_DMA(&huart2, (uint8_t *)&usart2_rx_buffer_temp, 128) != HAL_OK)    Error_Handler();
}

void UsartReceive_IDLE(UART_HandleTypeDef *huart)
{
	uint16_t i = 0;

	if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET))
	{
		if (huart->Instance == USART1)
		{
			__HAL_UART_CLEAR_IDLEFLAG(huart);
			i = huart->Instance->SR;
			i = huart->Instance->DR;
			i = hdma_usart1_rx.Instance->CNDTR;
			HAL_UART_DMAStop(huart);

			/* 此处处理数据，主要是拷贝和置位标志位 */
			if (usart1_rx_flag == 0)
			{
				usart1_rx_len = 128 - i;
				memcpy(usart1_rx_buffer, usart1_rx_buffer_temp, usart1_rx_len);
				usart1_rx_flag = 1;
			}

			/* 清空缓存，重新接收 */
			memset(usart1_rx_buffer_temp, 0x00, 128);
			HAL_UART_Receive_DMA(huart, (uint8_t *)&usart1_rx_buffer_temp, 128);
		}

		if (huart->Instance == USART2)
		{
			__HAL_UART_CLEAR_IDLEFLAG(huart);
			i = huart->Instance->SR;
			i = huart->Instance->DR;
			i = hdma_usart2_rx.Instance->CNDTR;
			HAL_UART_DMAStop(huart);

			/* 此处处理数据，主要是拷贝和置位标志位 */
			if (usart2_rx_flag == 0)
			{
				usart2_rx_len = 128 - i;
				memcpy(usart2_rx_buffer, usart2_rx_buffer_temp, usart2_rx_len);
				usart2_rx_flag = 1;
			}

			/* 清空缓存，重新接收 */
			memset(usart2_rx_buffer_temp, 0x00, 128);
			HAL_UART_Receive_DMA(huart, (uint8_t *)&usart2_rx_buffer_temp, 128);
		}
	}
}

void usart1_dma_send(unsigned char *buffer, unsigned int length)
{

	//等待上一次的数据发送完毕
	while (HAL_DMA_GetState(&hdma_usart1_tx) == HAL_DMA_STATE_BUSY);

	//关闭DMA
	__HAL_DMA_DISABLE(&hdma_usart1_tx);

	//开始发送数据
	HAL_UART_Transmit_DMA(&huart1, buffer, length);
}

void usart2_dma_send(unsigned char *buffer, unsigned int length)
{

	//等待上一次的数据发送完毕
	while (HAL_DMA_GetState(&hdma_usart2_tx) == HAL_DMA_STATE_BUSY);

	//关闭DMA
	__HAL_DMA_DISABLE(&hdma_usart2_tx);

	//开始发送数据
	HAL_UART_Transmit_DMA(&huart2, buffer, length);
}