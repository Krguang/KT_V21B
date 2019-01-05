#ifndef __BSP_DAC
#define __BSP_DAC

void bsp_InitDAC(void);

void putTempSetOut(uint16_t tempSetValue);
void putHumiSetOut(uint16_t humiSetValue);

#endif // !__BSP_DAC
