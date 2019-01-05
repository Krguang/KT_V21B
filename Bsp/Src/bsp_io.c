#include "bsp.h"


void unitRelayOn() { HAL_GPIO_WritePin(relay1_GPIO_Port, relay1_Pin, GPIO_PIN_SET); }
void unitRelayOff() { HAL_GPIO_WritePin(relay1_GPIO_Port, relay1_Pin, GPIO_PIN_RESET); }

void PN_PressRelayOn() { HAL_GPIO_WritePin(relay2_GPIO_Port, relay2_Pin, GPIO_PIN_SET); }
void PN_PressRelayOff() { HAL_GPIO_WritePin(relay2_GPIO_Port, relay2_Pin, GPIO_PIN_RESET); }

void exhaustFanRelayOn() { HAL_GPIO_WritePin(relay3_GPIO_Port, relay3_Pin, GPIO_PIN_SET); }
void exhaustFanRelayOff() { HAL_GPIO_WritePin(relay3_GPIO_Port, relay3_Pin, GPIO_PIN_RESET); }

void sterilampRelayOn() { HAL_GPIO_WritePin(relay4_GPIO_Port, relay4_Pin, GPIO_PIN_SET); }
void sterilampRelayOff() { HAL_GPIO_WritePin(relay4_GPIO_Port, relay4_Pin, GPIO_PIN_RESET); }

void standbyRelayOn() { HAL_GPIO_WritePin(relay5_GPIO_Port, relay5_Pin, GPIO_PIN_SET); }
void standbyRelayOff() { HAL_GPIO_WritePin(relay5_GPIO_Port, relay5_Pin, GPIO_PIN_RESET); }

uint8_t fanStateIn() { return HAL_GPIO_ReadPin(switch_in1_GPIO_Port, switch_in1_Pin) ^ 1; }
uint8_t PN_PressStateIn() { return HAL_GPIO_ReadPin(switch_in2_GPIO_Port, switch_in2_Pin) ^ 1; }
uint8_t onDutyStateIn() { return HAL_GPIO_ReadPin(switch_in3_GPIO_Port, switch_in3_Pin) ^ 1; }
uint8_t hepaStateIn() { return HAL_GPIO_ReadPin(switch_in4_GPIO_Port, switch_in4_Pin) ^ 1; }
uint8_t unitFaultStateIn() { return HAL_GPIO_ReadPin(switch_in5_GPIO_Port, switch_in5_Pin) ^ 1; }