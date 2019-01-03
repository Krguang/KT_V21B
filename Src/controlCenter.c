#include "bsp.h"
#include "main.h"
#include "controlCenter.h"

#define FLASH_SAVE_ADDR  0X08030000		//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)

FlashVar flash;					//存储在flash中的结构体
FlashVar flashTemp;				//用于判断flash结构体是否变化的缓存

uint8_t displayMode;			//显示模式 0：显示温湿度，1：显示设置界面；

uint16_t tempValue;				//温度实时值
uint16_t humiValue;				//湿度实时值

uint8_t ucKeyCode;				//按键代码

static uint32_t tempSetCount = 10;		//温度设定计时
static uint32_t humiSetCount = 10;		//湿度设定计时

uint8_t fanStatus;				//风机状态
uint8_t standbyStatus;			//备用状态
uint8_t onDutyStatus;			//值班状态
uint8_t hepaAlarm;				//高效报警
uint8_t unitFault;				//机组故障

/*
* 每500ms被调用一次
*/
void tempHumiSetCountTimeReference500ms()
{
	tempSetCount++;
	humiSetCount++;
}


/*
* 比较两个结构体是否相等，相等：返回1；不相等：返回0；
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
* 第一次上电初始化需要存储在flash的参数
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

	if (flash.h4Set == 0xffff)		//温度设定上限
	{
		flash.h4Set = 300;
		flashTemp.h4Set = 300;
		statusRecord++;
	}

	if (flash.h5Set == 0xffff)		//温度设定下限
	{
		flash.h5Set = 160;
		flashTemp.h5Set = 160;
		statusRecord++;
	}

	if (statusRecord > 0)
	{
		STMFLASH_Write(FLASH_SAVE_ADDR, (uint16_t *)&flash.tempSet, 7);
	}
}

/*
*
* 工作模式
*/
void operatingMode()
{

	if (ucKeyCode != KEY_NONE)
	{
		if (ucKeyCode == KEY_5_DOWN)
		{
			tempSetCount = 0;

			if (flash.tempSet > flash.h5Set)
			{
				flash.tempSet --;
			}
		}

		if (ucKeyCode == KEY_6_DOWN)
		{
			tempSetCount = 0;

			if (flash.tempSet < flash.h4Set)
			{
				flash.tempSet ++;
			}
		}
	
		if (ucKeyCode == KEY_7_DOWN)
		{
			humiSetCount = 0;
			if (flash.humiSet > 0)
			{
				flash.humiSet -= 10;
			}
		}

		if (ucKeyCode == KEY_8_DOWN)
		{
			humiSetCount = 0;
			if (flash.humiSet < 1000)
			{
				flash.humiSet += 10;
			}
		}
	}

	if (tempSetCount >= 10)
	{
		displayFloat(0, tempValue);
	}
	else
	{
		displayFloatBlink(0, flash.tempSet);
	}

	if (humiSetCount >= 10)
	{
		displayFloat(1, humiValue);
	}
	else 
	{
		displayFloatBlink(1, flash.humiSet);
	}

	if ((tempSetCount >= 10)&&(humiSetCount >= 10))			//不在温湿度设置状态才会保存参数，防止频繁写flash
	{
		if (compareStruct(flashTemp, flash) == 0)
		{
			flashTemp = flash;
			STMFLASH_Write(FLASH_SAVE_ADDR, (uint16_t *)&flash.tempSet, 7);
		}
	}
}


/*
*
* 模式选择，displayMode == 0：工作模式，displayMode != 0：设置模式。
*/
void modeSelect()
{
	ucKeyCode = bsp_GetKey();	/* 读取键值, 无键按下时返回 KEY_NONE = 0 */

	if (ucKeyCode != KEY_NONE)
	{
		if ((ucKeyCode == KEY_9_LONG)||(ucKeyCode == KEY_10_LONG))		//按后背键和3，4组合键都可以进入设置模式
		{
			displayMode = 1;
		}

		if (displayMode != 0)
		{
			if ((ucKeyCode == KEY_9_DOWN)||(ucKeyCode == KEY_4_DOWN))	//按后背键或4键模式++
			{
				displayMode++;
				if (displayMode > 5)
				{
					displayMode = 0;
				}
			}

			if (ucKeyCode == KEY_3_DOWN)								//按3键模式--；
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
