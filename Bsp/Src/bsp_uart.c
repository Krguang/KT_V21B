#include "bsp.h"
#include "usart.h"

int _write(int fd, char *pBuffer, int size)
{
	HAL_UART_Transmit(&huart2, pBuffer, size, 0xff);
	return size;
}

void bsp_InitUart(void)
{
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);    //ʹ�ܿ����ж�
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);    //ʹ�ܿ����ж�

	if (HAL_UART_Receive_DMA(&huart1, (uint8_t *)&usart1_rx_buffer, 128) != HAL_OK)    Error_Handler();
	if (HAL_UART_Receive_DMA(&huart2, (uint8_t *)&usart2_rx_buffer, 128) != HAL_OK)    Error_Handler();
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

			/* �˴��������ݣ���Ҫ�ǿ�������λ��־λ */
			if (usart1_rx_flag == 0)
			{
				usart1_tx_len = 128 - i;
				memcpy(usart1_tx_buffer, usart1_rx_buffer, usart1_tx_len);
				usart1_rx_flag = 1;
			}

			/* ��ջ��棬���½��� */
			memset(usart1_rx_buffer, 0x00, 128);
			HAL_UART_Receive_DMA(huart, (uint8_t *)&usart1_rx_buffer, 128);
		}

		if (huart->Instance == USART2)
		{
			__HAL_UART_CLEAR_IDLEFLAG(huart);
			i = huart->Instance->SR;
			i = huart->Instance->DR;
			i = hdma_usart2_rx.Instance->CNDTR;
			HAL_UART_DMAStop(huart);

			/* �˴��������ݣ���Ҫ�ǿ�������λ��־λ */
			if (usart2_rx_flag == 0)
			{
				usart2_tx_len = 128 - i;
				memcpy(usart2_tx_buffer, usart2_rx_buffer, usart2_tx_len);
				usart2_rx_flag = 1;
			}

			/* ��ջ��棬���½��� */
			memset(usart2_rx_buffer, 0x00, 128);
			HAL_UART_Receive_DMA(huart, (uint8_t *)&usart2_rx_buffer, 128);
		}
	}
}