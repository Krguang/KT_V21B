#ifndef __BSP_LED_DISPLAY
#define __BSP_LED_DISPLAY

void displayInt(uint8_t leftrOrRight, uint16_t data);
void displayString(uint8_t leftrOrRight, char * string);
void displayFloat(uint8_t leftrOrRight, uint16_t data);
void displayFloatBlink(uint8_t leftrOrRight, uint16_t data);
#endif // !__BSP_LED_DISPLAY
