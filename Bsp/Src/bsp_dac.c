#include "bsp.h"
#include "dac.h"

void bsp_InitDAC(void)
{
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
}