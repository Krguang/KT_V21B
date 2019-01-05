#ifndef __BASP_ADC
#define __BASP_ADC

void bsp_InitADC(void);
void bsp_GetAdcValue();

void getTempIn();
void getHumiIn();
void getPressIn();

#endif // !__BASP_ADC
