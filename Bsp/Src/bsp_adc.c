#include "bsp.h"
#include "adc.h"
#include "controlCenter.h"

static uint16_t adcValueTemp[3];
static volatile uint16_t ADC_ConvertedValue[3];
static uint32_t ADC_Average[3];

void bsp_InitADC(void)
{
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_ConvertedValue, 3);
}

void bsp_GetAdcValue()
{

	for (uint16_t i = 0; i < 100; i++)
	{
		for (uint8_t j = 0; j < 3; j++) {
			ADC_Average[j] += ADC_ConvertedValue[j];
		}
	}

	for (uint8_t i = 0; i < 3; i++)
	{
		//adcValueTemp[i] = (uint16_t)(ADC_Average[i] / 100 * 1000 / 4096);
		adcValueTemp[i] = (uint16_t)(ADC_Average[i]  * 10 / 4096);			//°Ñ0-10V×ª»»Îª0-1000
		ADC_Average[i] = 0;
	}
}

void getTempIn()
{
	tempValue = adcValueTemp[0] / 2;
}

void getHumiIn()
{
	humiValue = adcValueTemp[1];
}

void getPressIn()
{
	pressValue = adcValueTemp[2];
}