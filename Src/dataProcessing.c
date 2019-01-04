#include "bsp.h"
#include "main.h"
#include "dataProcessing.h"
#include "controlCenter.h"


static void internalProtocol03()
{
	localArray[0] = fanSwitch;
	localArray[1] = onDutySwitch;
	localArray[3] = standbySwitch;
	localArray[5] = flash.tempSet;
	localArray[6] = flash.humiSet;
}

static void internalProtocol10()
{
	flash.tempSet = localArray[5];
	flash.humiSet = localArray[6];
	tempKeyChangeTemp = flash.tempSet;
	humiKeyChangeTemp = flash.humiSet;

	tempValue = localArray[7];
	humiValue = localArray[8];
	fanStatus = localArray[9];
	onDutyStatus = localArray[10] & 1;
	standbyStatus = (localArray[10] >> 1) & 1;
	unitFault = localArray[11];
	hepaAlarm = localArray[12];
}

void modbusSlave03DataProcess()
{
	if ((flash.h1Set == 1) || (flash.h1Set == 2))	//通讯模式
	{
		switch (flash.h3Set)
		{
		case 0:
			internalProtocol03();
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		default:
			break;
		}
	}
	else if (flash.h1Set == 0)		//非通讯
	{

	}
	else if (flash.h1Set == 3)		//风机盘管
	{

	}
}

void modbusSlave10DataProcess()
{
	if ((flash.h1Set == 1) || (flash.h1Set == 2))	//通讯模式
	{
		switch (flash.h3Set)
		{
		case 0:
			internalProtocol10();
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		default:
			break;
		}
	}
	else if (flash.h1Set == 0)		//非通讯
	{

	}
	else if (flash.h1Set == 3)		//风机盘管
	{

	}
}
