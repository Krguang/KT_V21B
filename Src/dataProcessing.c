#include "bsp.h"
#include "main.h"
#include "dataProcessing.h"
#include "controlCenter.h"



void modbusSlave03DataProcess()
{
	localArray[0] = fanSwitch;
	localArray[1] = onDutySwitch;
	localArray[3] = standbySwitch;
	localArray[5] = flash.tempSet;
	localArray[6] = flash.humiSet;
}

void modbusSlave10DataProcess()
{
	flash.tempSet = localArray[5];
	flash.humiSet = localArray[6];
	tempValue = localArray[7];
	humiValue = localArray[8];
	fanStatus = localArray[9];
	onDutyStatus = localArray[10] & 1;
	standbyStatus = (localArray[10] >> 1) & 1;
	unitFault = localArray[11];
	hepaAlarm = localArray[12];

}
