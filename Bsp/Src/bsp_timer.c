#include "bsp.h"
#include "tim.h"



void bsp_InitTimer(void)
{
	HAL_TIM_Base_Start_IT(&htim6);
}

