#ifndef __CONTROLCENTER
#define __CONTROLCENTER

#include "main.h"

typedef struct
{
	uint16_t tempSet;
	uint16_t humiSet;
	uint16_t h1Set;
	uint16_t h2Set;
	uint16_t h3Set;
	uint16_t h4Set;
	uint16_t h5Set;
} FlashVar;

extern FlashVar flash;
extern FlashVar flashTemp;
extern uint32_t tempSetCount;		//�¶��趨��ʱ
extern uint32_t humiSetCount;		//ʪ���趨��ʱ

void paramInFlashInit(void);
void modeSelect(void);

#endif // !__CONTROLCENTER
