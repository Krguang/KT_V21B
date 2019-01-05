#include "bsp.h"
#include "main.h"
#include "dataProcessing.h"
#include "controlCenter.h"


static void internalProtocol03()			//内部协议
{
	localArray[0] = fanSwitch;
	localArray[1] = onDutySwitch;
	localArray[3] = standbySwitch;
	localArray[5] = flash.tempSet;
	localArray[6] = flash.humiSet;
}

static void externalProtocol03()			//外部协议
{
	if (fanSwitch == 1)
	{
		localArray[4] |= 1;
	}
	else
	{
		localArray[4] &= ~1;
	}

	if (onDutySwitch == 1)
	{
		localArray[4] |= (1<<1);
	}
	else
	{
		localArray[4] &= ~(1 << 1);
	}

	if (standbySwitch == 1)
	{
		localArray[4] |= (1 << 2);
	}
	else
	{
		localArray[4] &= ~(1 << 2);
	}

	localArray[5] = flash.tempSet;
	localArray[6] = flash.humiSet;
}

static void internalProtocol10()			//内部协议
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

static void externalProtocol10()
{
	flash.tempSet = localArray[5];
	flash.humiSet = localArray[6];
	fanStatus = localArray[7]&1;
	onDutyStatus = (localArray[7] >> 1) & 1;
	standbyStatus = (localArray[7] >> 2) & 1;
	unitFault = (localArray[7] >> 3) & 1;
	hepaAlarm = (localArray[7] >> 4) & 1;
	tempValue = localArray[8];
	humiValue = localArray[9];
}

/*
	非通讯模式的数据处理，包括“非通讯”和风机盘管
*/
void nonCommunicationMode()		
{
	if (flash.h1Set == 0)			//"非通讯"
	{

		fanStatus = fanStateIn();
		standbyStatus = PN_PressStateIn();
		onDutyStatus = onDutyStateIn();
		hepaAlarm = hepaStateIn();
		unitFault = unitFaultStateIn();

		if (fanSwitch == 1)
		{
			unitRelayOn();
		}
		else
		{
			unitRelayOff();
		}

		if (standbySwitch == 1)
		{
			exhaustFanRelayOn();
		}
		else
		{
			exhaustFanRelayOff();
		}

		if (onDutySwitch == 1)
		{
			sterilampRelayOn();
		}
		else
		{
			sterilampRelayOff();
		}

		getTempIn();
		getHumiIn();
		putTempSetOut(flash.tempSet);
		putHumiSetOut(flash.humiSet);
		
	}
	else if (flash.h1Set == 3)		//风机盘管
	{

	}
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
			externalProtocol03();
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
			externalProtocol10();
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
}
