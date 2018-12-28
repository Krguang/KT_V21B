#include "bsp.h"
#include "tim.h"

void bsp_InitTimer(void)
{
	HAL_TIM_Base_Start_IT(&htim6);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim6)
	{
		bsp_RunPer10ms();
	}
}