#ifndef __CONTROLCENTER
#define __CONTROLCENTER

#include "main.h"

#define FLASH_SAVE_ADDR  0X08030000		//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
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

extern uint8_t fanSwitch;				//�����ͣ����
extern uint8_t standbySwitch;			//���ÿ���
extern uint8_t onDutySwitch;			//ֵ�࿪��
extern uint8_t alarmSwitch;			//��������


extern uint8_t fanStatus;				//���״̬
extern uint8_t standbyStatus;			//����״̬
extern uint8_t onDutyStatus;			//ֵ��״̬
extern uint8_t hepaAlarm;				//��Ч����
extern uint8_t unitFault;				//�������

extern uint16_t tempValue;				//�¶�ʵʱֵ
extern uint16_t humiValue;				//ʪ��ʵʱֵ

void paramInFlashInit(void);
void modeSelect(void);
void tempHumiSetCountTimeReference500ms(void);

#endif // !__CONTROLCENTER
