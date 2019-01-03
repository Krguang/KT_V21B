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
* 此函数被10ms轮询调用，位led闪烁提供时基准
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
* 串口1的指示灯翻转
*/
 void usart1_led_toggle()
{
	 led_usart1_close_count = 0;
	HAL_GPIO_TogglePin(led_uart1_state_GPIO_Port, led_uart1_state_Pin);
}


/*
* 串口2的指示灯翻转
*/
void usart2_led_toggle()
{
	led_usart2_close_count = 0;
	HAL_GPIO_TogglePin(led_uart2_state_GPIO_Port, led_uart2_state_Pin);
}


/*
* key1对应led开启
*/
void key1_led_open()
{
	HAL_GPIO_WritePin(led_key1_GPIO_Port, led_key1_Pin, GPIO_PIN_RESET);
}

/*
* key1对应led关闭
*/
void key1_led_close()
{
	HAL_GPIO_WritePin(led_key1_GPIO_Port, led_key1_Pin, GPIO_PIN_SET);
}

/*
* key2对应led开启
*/
void key2_led_open()
{
	HAL_GPIO_WritePin(led_key2_GPIO_Port, led_key2_Pin, GPIO_PIN_RESET);
}

/*
* key2对应led关闭
*/
void key2_led_close()
{
	HAL_GPIO_WritePin(led_key2_GPIO_Port, led_key2_Pin, GPIO_PIN_SET);
}


/*
* key3对应led开启
*/
void key3_led_open()
{
	HAL_GPIO_WritePin(led_key3_GPIO_Port, led_key3_Pin, GPIO_PIN_RESET);
}

/*
* key3对应led关闭
*/
void key3_led_close()
{
	HAL_GPIO_WritePin(led_key3_GPIO_Port, led_key3_Pin, GPIO_PIN_SET);
}

/*
* key4对应led开启
*/
void key4_led_open()
{
	HAL_GPIO_WritePin(led_key4_GPIO_Port, led_key4_Pin, GPIO_PIN_RESET);
}

/*
* key4对应led关闭
*/
void key4_led_close()
{
	HAL_GPIO_WritePin(led_key4_GPIO_Port, led_key4_Pin, GPIO_PIN_SET);
}

/*
* led1 开启
*/
void led1_open()
{
	HAL_GPIO_WritePin(led_state_1_GPIO_Port, led_state_1_Pin, GPIO_PIN_SET);
}

/*
* led1 关闭
*/
void led1_close()
{
	HAL_GPIO_WritePin(led_state_1_GPIO_Port, led_state_1_Pin, GPIO_PIN_RESET);
}

/*
* led2 开启
*/
void led2_open()
{
	HAL_GPIO_WritePin(led_state_2_GPIO_Port, led_state_2_Pin, GPIO_PIN_SET);
}

/*
* led2 关闭
*/
void led2_close()
{
	HAL_GPIO_WritePin(led_state_2_GPIO_Port, led_state_2_Pin, GPIO_PIN_RESET);
}

/*
* led3 开启
*/
void led3_open()
{
	HAL_GPIO_WritePin(led_state_3_GPIO_Port, led_state_3_Pin, GPIO_PIN_SET);
}

/*
* led3 关闭
*/
void led3_close()
{
	HAL_GPIO_WritePin(led_state_3_GPIO_Port, led_state_3_Pin, GPIO_PIN_RESET);
}


/*
* led4 闪烁
*/
void led4_blink()
{
	HAL_GPIO_WritePin(led_state_4_GPIO_Port, led_state_4_Pin, led4_blink_flag);
}

/*
* led4 关闭
*/
void led4_close()
{
	HAL_GPIO_WritePin(led_state_4_GPIO_Port, led_state_4_Pin, GPIO_PIN_RESET);
}


/*
* led5 闪烁
*/
void led5_blink()
{
	HAL_GPIO_WritePin(led_state_5_GPIO_Port, led_state_5_Pin, led5_blink_flag);
}

/*
* led5 关闭
*/
void led5_close()
{
	HAL_GPIO_WritePin(led_state_5_GPIO_Port, led_state_5_Pin, GPIO_PIN_RESET);
}

/*
* beep 鸣叫
*/
void beep_open()
{
	HAL_GPIO_WritePin(beep_GPIO_Port, beep_Pin, beep_flag);
}

/*
* beep 关闭
*/
void beep_close()
{
	HAL_GPIO_WritePin(beep_GPIO_Port, beep_Pin, GPIO_PIN_SET);
}

