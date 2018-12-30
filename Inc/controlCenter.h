#ifndef __CONTROLCENTER
#define __CONTROLCENTER

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

void paramInFlashInit(void);
void mainActivity(void);

#endif // !__CONTROLCENTER
