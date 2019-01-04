#include "bsp.h"
#include "main.h"
#include "controlCenter.h"

FlashVar flash;					//�洢��flash�еĽṹ��
FlashVar flashTemp;				//�����ж�flash�ṹ���Ƿ�仯�Ļ���

uint16_t localArray[LOCAL_ARRAY_LENGTH];	//�洢�������ݵ����飬����modbusͨѶ

uint16_t tempValue;				//�¶�ʵʱֵ
uint16_t humiValue;				//ʪ��ʵʱֵ

uint16_t tempKeyChangeTemp;
uint16_t humiKeyChangeTemp;

uint8_t fanSwitch;				//�����ͣ����
uint8_t standbySwitch;			//���ÿ���
uint8_t onDutySwitch;			//ֵ�࿪��
uint8_t alarmSwitch;			//��������


uint8_t fanStatus;				//���״̬
uint8_t standbyStatus;			//����״̬
uint8_t onDutyStatus;			//ֵ��״̬
uint8_t hepaAlarm;				//��Ч����
uint8_t unitFault;				//�������

static uint8_t displayMode;			//��ʾģʽ 0����ʾ��ʪ�ȣ�1����ʾ���ý��棻
static uint8_t ucKeyCode;				//��������
static uint32_t tempSetCount = 10;		//�¶��趨��ʱ
static uint32_t humiSetCount = 10;		//ʪ���趨��ʱ
static uint8_t checkFlashCount;			//��Ҫд��flash�����ݵĶ�ʱ��飬ÿ10����һ���Ƿ���Ҫд�롣
static uint8_t checkFanSwitchCount;
static uint8_t checkstandbySwitchCount;
static uint8_t checkOnDutySwitchCount;

static void flashParamCheck();

/*
* ÿ500ms������һ��
*/
void tempHumiSetCountTimeReference500ms()
{
	tempSetCount++;
	humiSetCount++;
	if (checkFlashCount < 255) checkFlashCount++;
	if (checkFanSwitchCount < 255) checkFanSwitchCount++;
	if (checkstandbySwitchCount < 255) checkstandbySwitchCount++;
	if (checkOnDutySwitchCount < 255) checkOnDutySwitchCount++;
	flashParamCheck();
}


/*
* �Ƚ������ṹ���Ƿ���ȣ���ȣ�����1������ȣ�����0��
*/
static uint8_t compareStruct(FlashVar flashvar1, FlashVar flashvar2)
{
	if (flashvar1.h1Set != flashvar2.h1Set) return 0;
	if (flashvar1.h2Set != flashvar2.h2Set) return 0;
	if (flashvar1.h3Set != flashvar2.h3Set) return 0;
	if (flashvar1.h4Set != flashvar2.h4Set) return 0;
	if (flashvar1.h5Set != flashvar2.h5Set) return 0;
	if (flashvar1.tempSet != flashvar2.tempSet) return 0;
	if (flashvar1.humiSet != flashvar2.humiSet) return 0;
	return 1;
}

/*
* ��һ���ϵ��ʼ����Ҫ�洢��flash�Ĳ���
*/
void paramInFlashInit()
{
	uint8_t statusRecord = 0;

	STMFLASH_Read(FLASH_SAVE_ADDR, (uint16_t *)&flash.tempSet, 7);

	if (flash.tempSet == 0xffff)
	{
		flash.tempSet = 250;
		flashTemp.tempSet = 250;
		statusRecord++;
	}

	if (flash.humiSet == 0xffff)
	{
		flash.humiSet = 500;
		flashTemp.humiSet = 500;
		statusRecord++;
	}

	if (flash.h1Set == 0xffff)
	{
		flash.h1Set = 2;
		flashTemp.h1Set = 2;
		statusRecord++;
	}

	if (flash.h2Set == 0xffff)
	{
		flash.h2Set = 1;
		flashTemp.h2Set = 1;
		statusRecord++;
	}

	if (flash.h3Set == 0xffff)
	{
		flash.h3Set = 0;
		flashTemp.h3Set = 0;
		statusRecord++;
	}

	if (flash.h4Set == 0xffff)		//�¶��趨����
	{
		flash.h4Set = 300;
		flashTemp.h4Set = 300;
		statusRecord++;
	}

	if (flash.h5Set == 0xffff)		//�¶��趨����
	{
		flash.h5Set = 160;
		flashTemp.h5Set = 160;
		statusRecord++;
	}

	if (statusRecord > 0)
	{
		STMFLASH_Write(FLASH_SAVE_ADDR, (uint16_t *)&flash.tempSet, 7);
	}

	flashTemp = flash;
	tempKeyChangeTemp = flash.tempSet;
	humiKeyChangeTemp = flash.humiSet;
}

/*
* ���ⲿ��������
*/
static void operateProcessing()
{

	if (flash.h1Set == 2)	//���尴��
	{
		if (ucKeyCode != KEY_NONE)
		{

			if (ucKeyCode == KEY_1_DOWN)
			{
				fanSwitch = 1;
				checkFanSwitchCount = 0;
			}

			if (ucKeyCode == KEY_2_DOWN)
			{
				standbySwitch = 1;
				checkstandbySwitchCount = 0;
			}

			if (ucKeyCode == KEY_3_DOWN)
			{
				onDutySwitch = 1;
				checkOnDutySwitchCount = 0;
			}
		}

		if (checkFanSwitchCount > 10)
		{
			fanSwitch = 0;
		}

		if (checkstandbySwitchCount > 10)
		{
			standbySwitch = 0;
		}

		if (checkOnDutySwitchCount > 10)
		{
			onDutySwitch = 0;
		}

	}
	else					//��ƽ��ת����
	{
		if (ucKeyCode != KEY_NONE)
		{
			if (ucKeyCode == KEY_1_DOWN)
			{
				fanSwitch ^= 1;
			}

			if (ucKeyCode == KEY_2_DOWN)
			{
				standbySwitch ^= 1;
			}

			if (ucKeyCode == KEY_3_DOWN)
			{
				onDutySwitch ^= 1;
			}
		}
	}

	if (ucKeyCode == KEY_4_DOWN)
	{
		alarmSwitch ^= 1;
	}

	if (fanSwitch == 1)
	{
		key1_led_open();
	}
	else 
	{
		key1_led_close();
	}

	if (standbySwitch == 1)
	{
		key2_led_open();
	}
	else
	{
		key2_led_close();
	}

	if (onDutySwitch == 1)
	{
		key3_led_open();
	}
	else
	{
		key3_led_close();
	}

	if (alarmSwitch == 1)
	{
		key4_led_open();
	}
	else
	{
		key4_led_close();
	}

	if (fanStatus == 1)
	{
		led1_open();
	}
	else 
	{
		led1_close();
	}

	if (standbyStatus == 1)
	{
		led2_open();
	}
	else
	{
		led2_close();
	}

	if (onDutyStatus == 1)
	{
		led3_open();
	}
	else
	{
		led3_close();
	}

	if (hepaAlarm == 1)
	{
		led4_blink();
	}
	else
	{
		led4_close();
	}

	if (unitFault == 1)
	{
		led5_blink();
	}
	else
	{
		led5_close();
	}

	if (alarmSwitch == 0)
	{
		if (hepaAlarm || unitFault)
		{
			beep_open();
		}
	}
	else
	{
		beep_close();
	}
}


/*
*
* ����ģʽ
*/
void operatingMode()
{
	if (ucKeyCode != KEY_NONE)
	{
		if (ucKeyCode == KEY_5_DOWN)
		{
			tempSetCount = 0;
			
			if (tempKeyChangeTemp > flash.h5Set)
			{
				tempKeyChangeTemp--;
			}
		}

		if (ucKeyCode == KEY_6_DOWN)
		{
			tempSetCount = 0;

			if (tempKeyChangeTemp < flash.h4Set)
			{
				tempKeyChangeTemp++;
			}
		}
	
		if (ucKeyCode == KEY_7_DOWN)
		{
			humiSetCount = 0;
			if (humiKeyChangeTemp > 0)
			{
				humiKeyChangeTemp -= 10;
			}
		}

		if (ucKeyCode == KEY_8_DOWN)
		{
			humiSetCount = 0;
			if (humiKeyChangeTemp < 1000)
			{
				humiKeyChangeTemp += 10;
			}
		}
	}

	if (tempSetCount >= 10)
	{
		displayFloat(0, tempValue);
	}
	else
	{
		displayFloatBlink(0, tempKeyChangeTemp);
	}

	if (humiSetCount >= 10)
	{
		displayFloat(1, humiValue);
	}
	else 
	{
		displayFloatBlink(1, humiKeyChangeTemp);
	}

	operateProcessing();
}


static void flashParamCheck()
{
	if ((tempSetCount >= 10) && (humiSetCount >= 10))			//������ʪ������״̬�Żᱣ���������ֹƵ��дflash
	{
		flash.tempSet = tempKeyChangeTemp;
		flash.humiSet = humiKeyChangeTemp;

		if (checkFlashCount > 20)
		{
			checkFlashCount = 0;
			if (compareStruct(flashTemp, flash) == 0)
			{
				flashTemp = flash;
				STMFLASH_Write(FLASH_SAVE_ADDR, (uint16_t *)&flash.tempSet, 7);
			}
		}
	}
}

/*
*
* ģʽѡ��displayMode == 0������ģʽ��displayMode != 0������ģʽ��
*/
void modeSelect()
{
	ucKeyCode = bsp_GetKey();	/* ��ȡ��ֵ, �޼�����ʱ���� KEY_NONE = 0 */

	if (ucKeyCode != KEY_NONE)
	{
		if ((ucKeyCode == KEY_9_LONG)||(ucKeyCode == KEY_10_LONG))		//���󱳼���3��4��ϼ������Խ�������ģʽ
		{
			displayMode = 1;
		}

		if (displayMode != 0)
		{
			if ((ucKeyCode == KEY_9_DOWN)||(ucKeyCode == KEY_4_DOWN))	//���󱳼���4��ģʽ++
			{
				displayMode++;
				if (displayMode > 5)
				{
					displayMode = 0;
				}
			}

			if (ucKeyCode == KEY_3_DOWN)								//��3��ģʽ--��
			{
				displayMode--;
			}
		}
	}

	switch (displayMode)
	{
	case 0:
		operatingMode();
		break;
	case 1:

		if (ucKeyCode != KEY_NONE)
		{
			if (ucKeyCode == KEY_8_DOWN)
			{
				if (flash.h1Set < 3)
				{
					flash.h1Set++;
				}	
			}

			if (ucKeyCode == KEY_7_DOWN)
			{
				if (flash.h1Set > 0)
				{
					flash.h1Set--;
				}
			}
		}

		displayString(0, "H-1");
		displayInt(1, flash.h1Set);
		break;
	case 2:

		if (ucKeyCode != KEY_NONE)
		{
			if (ucKeyCode == KEY_8_DOWN)
			{
				if (flash.h2Set < 128)
				{
					flash.h2Set++;
				}
			}

			if (ucKeyCode == KEY_7_DOWN)
			{
				if (flash.h2Set > 1)
				{
					flash.h2Set--;
				}
			}
		}

		displayString(0, "H-2");
		displayInt(1, flash.h2Set);
		break;
	case 3:

		if (ucKeyCode != KEY_NONE)
		{
			if (ucKeyCode == KEY_8_DOWN)
			{
				if (flash.h3Set < 6)
				{
					flash.h3Set++;
				}
			}

			if (ucKeyCode == KEY_7_DOWN)
			{
				if (flash.h3Set > 0)
				{
					flash.h3Set--;
				}
			}
		}

		displayString(0, "H-3");
		displayInt(1, flash.h3Set);
		break;
	case 4:

		if (ucKeyCode != KEY_NONE)
		{
			if (ucKeyCode == KEY_8_DOWN)
			{
				if (flash.h4Set < 500)
				{
					flash.h4Set += 10;
				}
			}

			if (ucKeyCode == KEY_7_DOWN)
			{
				if (flash.h4Set > 10)
				{
					flash.h4Set -= 10;
				}
			}
		}

		displayString(0, "H-4");
		displayFloat(1, flash.h4Set);
		break;
	case 5:

		if (ucKeyCode != KEY_NONE)
		{
			if (ucKeyCode == KEY_8_DOWN)
			{
				if (flash.h5Set < flash.h4Set)
				{
					flash.h5Set += 10;
				}
			}

			if (ucKeyCode == KEY_7_DOWN)
			{
				if (flash.h5Set > 0)
				{
					flash.h5Set -= 10;
				}
			}
		}

		displayString(0, "H-5");
		displayFloat(1, flash.h5Set);
		break;
	default:
		break;
	}
}
