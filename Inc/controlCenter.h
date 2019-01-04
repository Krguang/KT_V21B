#ifndef __CONTROLCENTER
#define __CONTROLCENTER

#include "main.h"

#define FLASH_SAVE_ADDR  0X08030000		//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
#define LOCAL_ARRAY_LENGTH 128

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
extern uint16_t localArray[];

extern uint8_t fanSwitch;				//风机启停开关
extern uint8_t standbySwitch;			//备用开关
extern uint8_t onDutySwitch;			//值班开关
extern uint8_t alarmSwitch;			//消音开关


extern uint8_t fanStatus;				//风机状态
extern uint8_t standbyStatus;			//备用状态
extern uint8_t onDutyStatus;			//值班状态
extern uint8_t hepaAlarm;				//高效报警
extern uint8_t unitFault;				//机组故障

extern uint16_t tempValue;				//温度实时值
extern uint16_t humiValue;				//湿度实时值

void paramInFlashInit(void);
void modeSelect(void);
void tempHumiSetCountTimeReference500ms(void);

#endif // !__CONTROLCENTER
