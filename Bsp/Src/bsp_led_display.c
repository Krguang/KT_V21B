#include "bsp.h"


static void led_a_on() { HAL_GPIO_WritePin(digtal_led_a_GPIO_Port, digtal_led_a_Pin, GPIO_PIN_RESET); }
static void led_b_on() { HAL_GPIO_WritePin(digtal_led_b_GPIO_Port, digtal_led_b_Pin, GPIO_PIN_RESET); }
static void led_c_on() { HAL_GPIO_WritePin(digtal_led_c_GPIO_Port, digtal_led_c_Pin, GPIO_PIN_RESET); }
static void led_d_on() { HAL_GPIO_WritePin(digtal_led_d_GPIO_Port, digtal_led_d_Pin, GPIO_PIN_RESET); }
static void led_e_on() { HAL_GPIO_WritePin(digtal_led_e_GPIO_Port, digtal_led_e_Pin, GPIO_PIN_RESET); }
static void led_f_on() { HAL_GPIO_WritePin(digtal_led_f_GPIO_Port, digtal_led_f_Pin, GPIO_PIN_RESET); }
static void led_g_on() { HAL_GPIO_WritePin(digtal_led_g_GPIO_Port, digtal_led_g_Pin, GPIO_PIN_RESET); }
static void led_dp_on() { HAL_GPIO_WritePin(digtal_led_dp_GPIO_Port, digtal_led_dp_Pin, GPIO_PIN_RESET); }

static void led_1_on() { HAL_GPIO_WritePin(digtal_led_1_GPIO_Port, digtal_led_1_Pin, GPIO_PIN_RESET); }
static void led_2_on() { HAL_GPIO_WritePin(digtal_led_2_GPIO_Port, digtal_led_2_Pin, GPIO_PIN_RESET); }
static void led_3_on() { HAL_GPIO_WritePin(digtal_led_3_GPIO_Port, digtal_led_3_Pin, GPIO_PIN_RESET); }
static void led_4_on() { HAL_GPIO_WritePin(digtal_led_4_GPIO_Port, digtal_led_4_Pin, GPIO_PIN_RESET); }
static void led_5_on() { HAL_GPIO_WritePin(digtal_led_5_GPIO_Port, digtal_led_5_Pin, GPIO_PIN_RESET); }
static void led_6_on() { HAL_GPIO_WritePin(digtal_led_6_GPIO_Port, digtal_led_6_Pin, GPIO_PIN_RESET); }


static void led_a_off() { HAL_GPIO_WritePin(digtal_led_a_GPIO_Port, digtal_led_a_Pin, GPIO_PIN_SET); }
static void led_b_off() { HAL_GPIO_WritePin(digtal_led_b_GPIO_Port, digtal_led_b_Pin, GPIO_PIN_SET); }
static void led_c_off() { HAL_GPIO_WritePin(digtal_led_c_GPIO_Port, digtal_led_c_Pin, GPIO_PIN_SET); }
static void led_d_off() { HAL_GPIO_WritePin(digtal_led_d_GPIO_Port, digtal_led_d_Pin, GPIO_PIN_SET); }
static void led_e_off() { HAL_GPIO_WritePin(digtal_led_e_GPIO_Port, digtal_led_e_Pin, GPIO_PIN_SET); }
static void led_f_off() { HAL_GPIO_WritePin(digtal_led_f_GPIO_Port, digtal_led_f_Pin, GPIO_PIN_SET); }
static void led_g_off() { HAL_GPIO_WritePin(digtal_led_g_GPIO_Port, digtal_led_g_Pin, GPIO_PIN_SET); }
static void led_dp_off() { HAL_GPIO_WritePin(digtal_led_dp_GPIO_Port, digtal_led_dp_Pin, GPIO_PIN_SET); }

static void led_1_off() { HAL_GPIO_WritePin(digtal_led_1_GPIO_Port, digtal_led_1_Pin, GPIO_PIN_SET); }
static void led_2_off() { HAL_GPIO_WritePin(digtal_led_2_GPIO_Port, digtal_led_2_Pin, GPIO_PIN_SET); }
static void led_3_off() { HAL_GPIO_WritePin(digtal_led_3_GPIO_Port, digtal_led_3_Pin, GPIO_PIN_SET); }
static void led_4_off() { HAL_GPIO_WritePin(digtal_led_4_GPIO_Port, digtal_led_4_Pin, GPIO_PIN_SET); }
static void led_5_off() { HAL_GPIO_WritePin(digtal_led_5_GPIO_Port, digtal_led_5_Pin, GPIO_PIN_SET); }
static void led_6_off() { HAL_GPIO_WritePin(digtal_led_6_GPIO_Port, digtal_led_6_Pin, GPIO_PIN_SET); }

static uint8_t displayFlag;

void displayTimeReference500ms()
{
	displayFlag ^= 1;
}


static uint8_t* floatToChar(uint16_t data) {
	static uint8_t floatTemp[5];
	if (data<1000)
	{
		floatTemp[0] = (data / 100) % 10 + 48;
		floatTemp[1] = (data / 10) % 10 + 48;
		floatTemp[2] = '.';
		floatTemp[3] = data % 10 + 48;
		floatTemp[4] = '\0';
	}
	else
	{
		floatTemp[0] = (data / 1000) % 10 + 48;
		floatTemp[1] = (data / 100) % 10 + 48;
		floatTemp[2] = (data / 10) % 10 + 48;
		floatTemp[3] = '\0';
	}

	return floatTemp;
}

static uint8_t* intToChar(uint16_t data)
{
	static uint8_t charTemp[4];
	if (data<1000)
	{
		charTemp[0] = (data / 100) % 10 + 48;
		charTemp[1] = (data / 10) % 10 + 48;
		charTemp[2] = data % 10 + 48;
		charTemp[3] = '\0';
	}
	return charTemp;
}

/*
num 1-4 对应 1-4数码管
data 1:显示小数点  0：不显示小数点
*/
static void pointDisplay(uint8_t num, uint8_t data) {

	led_a_off();
	led_b_off();
	led_c_off();
	led_d_off();
	led_e_off();
	led_f_off();
	led_g_off();
	led_dp_off();

	switch (num)
	{
	case 2:

		led_1_off();
		led_2_on();
		led_3_off();
		led_4_off();
		led_5_off();
		led_6_off();

		break;
	case 5:
		led_1_off();
		led_2_off();
		led_3_off();
		led_4_off();
		led_5_on();
		led_6_off();
		break;
	default:
		break;
	}

	led_a_off();
	led_b_off();
	led_c_off();
	led_d_off();
	led_e_off();
	led_f_off();
	led_g_off();
	led_dp_on();
	HAL_Delay(1);
}


/*
num : 1-6 对应 1-6数码管
data : 要显示的数值
*/
static void displayNumber(uint8_t num, uint8_t data) {

	led_a_off();
	led_b_off();
	led_c_off();
	led_d_off();
	led_e_off();
	led_f_off();
	led_g_off();
	led_dp_off();

	led_1_off();
	led_2_off();
	led_3_off();
	led_4_off();
	led_5_off();
	led_6_off();

	switch (num)
	{
	case 0:

		led_1_off();
		led_2_off();
		led_3_off();
		led_4_off();
		led_5_off();
		led_6_off();
		break;

	case 1:

		led_1_on();
		led_2_off();
		led_3_off();
		led_4_off();
		led_5_off();
		led_6_off();
		break;
	case 2:
		led_1_off();
		led_2_on();
		led_3_off();
		led_4_off();
		led_5_off();
		led_6_off();
		break;
	case 3:
		led_1_off();
		led_2_off();
		led_3_on();
		led_4_off();
		led_5_off();
		led_6_off();
		break;
	case 4:
		led_1_off();
		led_2_off();
		led_3_off();
		led_4_on();
		led_5_off();
		led_6_off();
		break;
	case 5:
		led_1_off();
		led_2_off();
		led_3_off();
		led_4_off();
		led_5_on();
		led_6_off();
		break;
	case 6:
		led_1_off();
		led_2_off();
		led_3_off();
		led_4_off();
		led_5_off();
		led_6_on();
		break;
	default:
		break;
	}

	switch (data)
	{
	case '0':
		led_a_on();
		led_b_on();
		led_c_on();
		led_d_on();
		led_e_on();
		led_f_on();
		led_g_off();
		led_dp_off();
		break;
	case '1':
		led_a_off();
		led_b_on();
		led_c_on();
		led_d_off();
		led_e_off();
		led_f_off();
		led_g_off();
		led_dp_off();
		break;
	case '2':
		led_a_on();
		led_b_on();
		led_c_off();
		led_d_on();
		led_e_on();
		led_f_off();
		led_g_on();
		led_dp_off();
		break;
	case '3':
		led_a_on();
		led_b_on();
		led_c_on();
		led_d_on();
		led_e_off();
		led_f_off();
		led_g_on();
		led_dp_off();
		break;
	case '4':
		led_a_off();
		led_b_on();
		led_c_on();
		led_d_off();
		led_e_off();
		led_f_on();
		led_g_on();
		led_dp_off();
		break;
	case '5':
		led_a_on();
		led_b_off();
		led_c_on();
		led_d_on();
		led_e_off();
		led_f_on();
		led_g_on();
		led_dp_off();
		break;
	case '6':
		led_a_on();
		led_b_off();
		led_c_on();
		led_d_on();
		led_e_on();
		led_f_on();
		led_g_on();
		led_dp_off();
		break;
	case '7':
		led_a_on();
		led_b_on();
		led_c_on();
		led_d_off();
		led_e_off();
		led_f_off();
		led_g_off();
		led_dp_off();
		break;
	case '8':
		led_a_on();
		led_b_on();
		led_c_on();
		led_d_on();
		led_e_on();
		led_f_on();
		led_g_on();
		led_dp_off();
		break;
	case '9':
		led_a_on();
		led_b_on();
		led_c_on();
		led_d_on();
		led_e_off();
		led_f_on();
		led_g_on();
		led_dp_off();
		break;
	case 'h':
		led_a_off();
		led_b_on();
		led_c_on();
		led_d_off();
		led_e_on();
		led_f_on();
		led_g_on();
		led_dp_off();
		break;
	case 'H':
		led_a_off();
		led_b_on();
		led_c_on();
		led_d_off();
		led_e_on();
		led_f_on();
		led_g_on();
		led_dp_off();
		break;
	case '-':
		led_a_off();
		led_b_off();
		led_c_off();
		led_d_off();
		led_e_off();
		led_f_off();
		led_g_on();
		led_dp_off();
		break;
	default:
		break;
	}
	HAL_Delay(1);
}

/*
* eftrOrRight : 选择显示在左侧还是右侧 0：左侧，1：右侧
* string : 要显示的字符串
*/
void displayString(uint8_t leftrOrRight,char * string)
{
	uint8_t *p = string;
	uint8_t i;
	uint8_t limit = 3;//限制字符显示个数

	if (leftrOrRight)
	{
		i = 4;
	}
	else {
		i = 1;
	}

	while (*p&&(limit--))
	{
		if (*p != '.') {

			displayNumber(i++, *p);
		}
		else
		{
			pointDisplay(2, 1);
		}
		p++;
	}
}
			
/*
* eftrOrRight : 选择显示在左侧还是右侧 0：左侧，1：右侧
* data : 要显示的整形
*/
void displayFloat(uint8_t leftrOrRight, uint16_t data)
{
	uint8_t *p = floatToChar(data);
	uint8_t i;

	if (leftrOrRight)
	{
		i = 4;
	}
	else {
		i = 1;
	}

	while (*p)
	{
		if (*p != '.') {

			displayNumber(i++, *p);
		}
		else
		{
			if (leftrOrRight)
			{
				pointDisplay(4, 1);
			}
			else {
				pointDisplay(2, 1);
			}
		}
		p++;
	}
}

/*
* eftrOrRight : 选择显示在左侧还是右侧 0：左侧，1：右侧
* data : 要显示的整形
*/
void displayFloatBlink(uint8_t leftrOrRight, uint16_t data)
{

	uint8_t *p = floatToChar(data);
	uint8_t i;

	if (leftrOrRight)
	{
		i = 4;
	}
	else {
		i = 1;
	}

	while (*p)
	{
		if (*p != '.') {
			if (displayFlag)
			{
				displayNumber(i++, *p);
			}
			else
			{
				displayNumber(0, 0);
			}
		}
		else
		{
			if (displayFlag)
			{
				if (leftrOrRight)
				{
					pointDisplay(4, 1);
				}
				else {
					pointDisplay(2, 1);
				}
			}
			else
			{
				pointDisplay(0, 0);
			}
		}
		p++;
	}
}

void displayInt(uint8_t leftrOrRight, uint16_t data)
{
	uint8_t *p = intToChar(data);
	uint8_t i;

	if (leftrOrRight)
	{
		i = 4;
	}
	else {
		i = 1;
	}

	while (*p)
	{
		displayNumber(i++, *p);
		p++;
	}
}