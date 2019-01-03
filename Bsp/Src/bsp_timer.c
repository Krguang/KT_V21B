#include "bsp.h"
#include "tim.h"



void bsp_InitTimer(void)
{
	HAL_TIM_Base_Start_IT(&htim6);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint8_t count = 0;

	if (htim->Instance == TIM4) {
		HAL_IncTick();
	}

	if (htim == &htim6)
	{
		bsp_RunPer10ms();
	}

	count++;
	if (count > 50)
	{
		count = 0;
		bsp_RunPer500ms();
	}
}