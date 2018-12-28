#include "bsp.h"
#include "adc.h"

void bsp_InitADC(void)
{
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_ConvertedValue, 3);
}