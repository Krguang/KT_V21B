#include "bsp.h"
#include "dac.h"

void bsp_InitDAC(void)
{
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
}

void putTempSetOut(uint16_t tempSetValue)
{

	uint16_t tempSetTemp = tempSetValue * 2 * 4095 / 1000;
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, tempSetTemp);
}

void putHumiSetOut(uint16_t humiSetValue)
{
	uint16_t humiSetTemp = humiSetValue * 4095 / 1000;
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, humiSetTemp);
}