#ifndef __BSP_IO
#define __BSP_IO

void unitRelayOn(void);
void unitRelayOff(void);

void PN_PressRelayOn(void);
void PN_PressRelayOff(void);

void exhaustFanRelayOn(void);
void exhaustFanRelayOff(void);

void sterilampRelayOn(void);
void sterilampRelayOff(void);

void standbyRelayOn(void);
void standbyRelayOff(void);

uint8_t fanStateIn(void);
uint8_t PN_PressStateIn(void);
uint8_t onDutyStateIn(void);
uint8_t hepaStateIn(void);
uint8_t unitFaultStateIn(void);

#endif // !__BSP_IO
