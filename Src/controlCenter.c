#include "bsp.h"
#include "main.h"
#include "controlCenter.h"

#define FLASH_SAVE_ADDR  0X08030000		//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)

FlashVar flash;
FlashVar flashTemp;

uint8_t displayMode;	//显示模式 0：显示温湿度，1：显示设置界面；

uint16_t tempValue;
uint16_t humiValue;

uint8_t ucKeyCode;				/* 按键代码 */

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
		statusRecord++;
	}

	if (flash.humiSet == 0xffff)
	{
		flash.humiSet = 500;
		statusRecord++;
	}

	if (flash.h1Set == 0xffff)
	{
		flash.h1Set = 2;
		statusRecord++;
	}

	if (flash.h2Set == 0xffff)
	{
		flash.h2Set = 1;
		statusRecord++;
	}

	if (flash.h3Set == 0xffff)
	{
		flash.h3Set = 0;
		statusRecord++;
	}

	if (flash.h4Set == 0xffff)
	{
		flash.h4Set = 160;
		statusRecord++;
	}

	if (flash.h5Set == 0xffff)
	{
		flash.h5Set = 300;
		statusRecord++;
	}

	if (statusRecord > 0)
	{
		STMFLASH_Write(FLASH_SAVE_ADDR, (uint16_t *)&flash.tempSet, 7);
	}
}

void mainActivity()
{
	ucKeyCode = bsp_GetKey();	/* 读取键值, 无键按下时返回 KEY_NONE = 0 */

	//bsp_printf("ucKeyCode = %d \n", ucKeyCode);

	if (ucKeyCode != KEY_NONE)
	{
		if (ucKeyCode == KEY_9_LONG)
		{
			displayMode = 1;
		}

		if (displayMode != 0)
		{
			if (ucKeyCode == KEY_9_DOWN)
			{
				displayMode++;
				if (displayMode > 5)
				{
					displayMode = 0;
				}
			}
		}
		
	}

	switch (displayMode)
	{
	case 0:
		displayFloat(0, tempValue);
		displayFloat(1, humiValue);
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
		displayString(0, "H-3");
		displayInt(1, flash.h3Set);
		break;
	case 4:
		displayString(0, "H-4");
		displayFloat(1, flash.h4Set);
		break;
	case 5:
		displayString(0, "H-5");
		displayFloat(1, flash.h5Set);
		break;
	default:
		break;
	}


}