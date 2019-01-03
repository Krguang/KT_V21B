#include "bsp.h"

static uint16_t led_usart1_close_count;
static uint16_t led_usart2_close_count;
static uint16_t led4_blink_count;
static uint8_t led4_blink_flag;
static uint16_t led5_blink_count;
static uint8_t led5_blink_flag;
static uint16_t beep_count;
static uint8_t beep_flag;

/*
* �˺�����10ms��ѯ���ã�λled��˸�ṩʱ��׼
*/
void ledTimeReference10ms()
{
	
	if (led_usart1_close_count < 1000)
	{
		led_usart1_close_count++;
	}
	else
	{
		HAL_GPIO_WritePin(led_uart1_state_GPIO_Port, led_uart1_state_Pin, GPIO_PIN_SET);
	}

	if (led_usart2_close_count < 1000)
	{
		led_usart2_close_count++;
	}
	else
	{
		HAL_GPIO_WritePin(led_uart2_state_GPIO_Port, led_uart2_state_Pin, GPIO_PIN_SET);
	}

	led4_blink_count++;
	if (led4_blink_count > 50)
	{
		led4_blink_count = 0;
		led4_blink_flag ^= 1;
	}

	led5_blink_count++;
	if (led5_blink_count > 50)
	{
		led5_blink_count = 0;
		led5_blink_flag ^= 1;
	}

	beep_count++;
	if (beep_count > 50)
	{
		beep_count = 0;
		beep_flag ^= 1;
	}
}


/*
* ����1��ָʾ�Ʒ�ת
*/
 void usart1_led_toggle()
{
	 led_usart1_close_count = 0;
	HAL_GPIO_TogglePin(led_uart1_state_GPIO_Port, led_uart1_state_Pin);
}


/*
* ����2��ָʾ�Ʒ�ת
*/
void usart2_led_toggle()
{
	led_usart2_close_count = 0;
	HAL_GPIO_TogglePin(led_uart2_state_GPIO_Port, led_uart2_state_Pin);
}


/*
* key1��Ӧled����
*/
void key1_led_open()
{
	HAL_GPIO_WritePin(led_key1_GPIO_Port, led_key1_Pin, GPIO_PIN_RESET);
}

/*
* key1��Ӧled�ر�
*/
void key1_led_close()
{
	HAL_GPIO_WritePin(led_key1_GPIO_Port, led_key1_Pin, GPIO_PIN_SET);
}

/*
* key2��Ӧled����
*/
void key2_led_open()
{
	HAL_GPIO_WritePin(led_key2_GPIO_Port, led_key2_Pin, GPIO_PIN_RESET);
}

/*
* key2��Ӧled�ر�
*/
void key2_led_close()
{
	HAL_GPIO_WritePin(led_key2_GPIO_Port, led_key2_Pin, GPIO_PIN_SET);
}


/*
* key3��Ӧled����
*/
void key3_led_open()
{
	HAL_GPIO_WritePin(led_key3_GPIO_Port, led_key3_Pin, GPIO_PIN_RESET);
}

/*
* key3��Ӧled�ر�
*/
void key3_led_close()
{
	HAL_GPIO_WritePin(led_key3_GPIO_Port, led_key3_Pin, GPIO_PIN_SET);
}

/*
* key4��Ӧled����
*/
void key4_led_open()
{
	HAL_GPIO_WritePin(led_key4_GPIO_Port, led_key4_Pin, GPIO_PIN_RESET);
}

/*
* key4��Ӧled�ر�
*/
void key4_led_close()
{
	HAL_GPIO_WritePin(led_key4_GPIO_Port, led_key4_Pin, GPIO_PIN_SET);
}

/*
* led1 ����
*/
void led1_open()
{
	HAL_GPIO_WritePin(led_state_1_GPIO_Port, led_state_1_Pin, GPIO_PIN_SET);
}

/*
* led1 �ر�
*/
void led1_close()
{
	HAL_GPIO_WritePin(led_state_1_GPIO_Port, led_state_1_Pin, GPIO_PIN_RESET);
}

/*
* led2 ����
*/
void led2_open()
{
	HAL_GPIO_WritePin(led_state_2_GPIO_Port, led_state_2_Pin, GPIO_PIN_SET);
}

/*
* led2 �ر�
*/
void led2_close()
{
	HAL_GPIO_WritePin(led_state_2_GPIO_Port, led_state_2_Pin, GPIO_PIN_RESET);
}

/*
* led3 ����
*/
void led3_open()
{
	HAL_GPIO_WritePin(led_state_3_GPIO_Port, led_state_3_Pin, GPIO_PIN_SET);
}

/*
* led3 �ر�
*/
void led3_close()
{
	HAL_GPIO_WritePin(led_state_3_GPIO_Port, led_state_3_Pin, GPIO_PIN_RESET);
}


/*
* led4 ��˸
*/
void led4_blink()
{
	HAL_GPIO_WritePin(led_state_4_GPIO_Port, led_state_4_Pin, led4_blink_flag);
}

/*
* led4 �ر�
*/
void led4_close()
{
	HAL_GPIO_WritePin(led_state_4_GPIO_Port, led_state_4_Pin, GPIO_PIN_RESET);
}


/*
* led5 ��˸
*/
void led5_blink()
{
	HAL_GPIO_WritePin(led_state_5_GPIO_Port, led_state_5_Pin, led5_blink_flag);
}

/*
* led5 �ر�
*/
void led5_close()
{
	HAL_GPIO_WritePin(led_state_5_GPIO_Port, led_state_5_Pin, GPIO_PIN_RESET);
}

/*
* beep ����
*/
void beep_open()
{
	HAL_GPIO_WritePin(beep_GPIO_Port, beep_Pin, beep_flag);
}

/*
* beep �ر�
*/
void beep_close()
{
	HAL_GPIO_WritePin(beep_GPIO_Port, beep_Pin, GPIO_PIN_SET);
}

