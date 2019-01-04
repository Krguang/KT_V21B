#include "bsp.h"
#include "main.h"
#include "modbus_slave.h"
#include "controlCenter.h"
#include "usart.h"
#include "dataProcessing.h"

static void MODS_SendWithCRC(uint8_t *_pBuf, uint8_t _ucLen);
static void MODS_SendAckOk(void);
static void MODS_SendAckErr(uint8_t _ucErrCode);
static void MODS_AnalyzeApp(void);


static void MODS_03H(void);
static void MODS_06H(void);
static void MODS_10H(void);


MODS_T g_tModS;
VAR_T g_tVar;

/*
*********************************************************************************************************
*	函 数 名: MODS_Poll
*	功能说明: 解析数据包. 在主程序中轮流调用。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MODS_Poll(void)
{
	uint16_t addr;
	uint16_t crc1;
	

	if (1 == usart1_rx_flag)
	{

		memcpy(g_tModS.RxBuf, usart1_rx_buffer, usart1_rx_len);
		g_tModS.RxCount = usart1_rx_len;

		if (g_tModS.RxCount < 4)				/* 接收到的数据小于4个字节就认为错误 */
		{
			goto err_ret;
		}

		/* 计算CRC校验和 */
		crc1 = CRC16_Modbus(g_tModS.RxBuf, g_tModS.RxCount);
		if (crc1 != 0)
		{
			goto err_ret;
		}

		/* 站地址 (1字节） */
		addr = usart1_rx_buffer[0];				/* 第1字节 站号 */
		if (addr != flash.h2Set)		 			/* 判断主机发送的命令地址是否符合 */
		{
			goto err_ret;
		}

		/* 分析应用层协议 */
		MODS_AnalyzeApp();

	err_ret:
		usart1_rx_flag = 0;					/* 必须清零计数器，方便下次帧同步 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: MODS_SendWithCRC
*	功能说明: 发送一串数据, 自动追加2字节CRC
*	形    参: _pBuf 数据；
*			  _ucLen 数据长度（不带CRC）
*	返 回 值: 无
*********************************************************************************************************
*/
static void MODS_SendWithCRC(uint8_t *_pBuf, uint8_t _ucLen)
{
	uint16_t crc;
	uint8_t buf[S_TX_BUF_SIZE];

	memcpy(buf, _pBuf, _ucLen);
	crc = CRC16_Modbus(_pBuf, _ucLen);
	buf[_ucLen++] = crc >> 8;
	buf[_ucLen++] = crc;
	usart1_dma_send(buf, _ucLen);
	//HAL_UART_Transmit(&huart1, buf, _ucLen,0xff);
}



/*
*********************************************************************************************************
*	函 数 名: MODS_SendAckErr
*	功能说明: 发送错误应答
*	形    参: _ucErrCode : 错误代码
*	返 回 值: 无
*********************************************************************************************************
*/
static void MODS_SendAckErr(uint8_t _ucErrCode)
{
	uint8_t txbuf[3];

	txbuf[0] = g_tModS.RxBuf[0];					/* 485地址 */
	txbuf[1] = g_tModS.RxBuf[1] | 0x80;				/* 异常的功能码 */
	txbuf[2] = _ucErrCode;							/* 错误代码(01,02,03,04) */

	MODS_SendWithCRC(txbuf, 3);
}

/*
*********************************************************************************************************
*	函 数 名: MODS_SendAckOk
*	功能说明: 发送正确的应答.
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MODS_SendAckOk(void)
{
	uint8_t txbuf[6];
	uint8_t i;

	for (i = 0; i < 6; i++)
	{
		txbuf[i] = g_tModS.RxBuf[i];
	}
	MODS_SendWithCRC(txbuf, 6);
}


/*
*********************************************************************************************************
*	函 数 名: MODS_ReadRegValue
*	功能说明: 读取保持寄存器的值
*	形    参: reg_addr 寄存器地址
*			  reg_value 存放寄存器结果
*	返 回 值: 1表示OK 0表示错误
*********************************************************************************************************
*/
static uint8_t MODS_ReadRegValue(uint16_t reg_addr, uint8_t *reg_value)
{
	uint16_t value;

	switch (reg_addr)									/* 判断寄存器地址 */
	{
	case SLAVE_REG_P01:
		value = g_tVar.P01;
		break;

	case SLAVE_REG_P02:
		value = g_tVar.P02;							/* 将寄存器值读出 */
		break;

	default:
		return 0;									/* 参数异常，返回 0 */
	}

	reg_value[0] = value >> 8;
	reg_value[1] = value;

	return 1;											/* 读取成功 */
}

/*
*********************************************************************************************************
*	函 数 名: MODS_WriteRegValue
*	功能说明: 读取保持寄存器的值
*	形    参: reg_addr 寄存器地址
*			  reg_value 寄存器值
*	返 回 值: 1表示OK 0表示错误
*********************************************************************************************************
*/
static uint8_t MODS_WriteRegValue(uint16_t reg_addr, uint16_t reg_value)
{
	switch (reg_addr)							/* 判断寄存器地址 */
	{
	case SLAVE_REG_P01:
		g_tVar.P01 = reg_value;				/* 将值写入保存寄存器 */
		break;

	case SLAVE_REG_P02:
		g_tVar.P02 = reg_value;				/* 将值写入保存寄存器 */
		break;

	default:
		return 0;		/* 参数异常，返回 0 */
	}

	return 1;		/* 读取成功 */
}

/*
*********************************************************************************************************
*	函 数 名: MODS_AnalyzeApp
*	功能说明: 分析应用层协议
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MODS_AnalyzeApp(void)
{
	switch (usart1_rx_buffer[1])				/* 第2个字节 功能码 */
	{
	case 0x01:							/* 读取线圈状态（此例程用led代替）*/
		//MODS_01H();
		break;

	case 0x02:							/* 读取输入状态（按键状态）*/
		//MODS_02H();
		break;

	case 0x03:							/* 读取保持寄存器（此例程存在g_tVar中）*/
		MODS_03H();
		break;

	case 0x04:							/* 读取输入寄存器（ADC的值）*/
		//MODS_04H();
		//bsp_PutMsg(MSG_MODS_04H, 0);
		break;

	case 0x05:							/* 强制单线圈（设置led）*/
		//MODS_05H();
		//bsp_PutMsg(MSG_MODS_05H, 0);
		break;

	case 0x06:							/* 写单个保存寄存器（此例程改写g_tVar中的参数）*/
		MODS_06H();
		//bsp_PutMsg(MSG_MODS_06H, 0);
		break;

	case 0x10:							/* 写多个保存寄存器（此例程存在g_tVar中的参数）*/
		MODS_10H();
		break;

	default:
		g_tModS.RspCode = RSP_ERR_CMD;
		MODS_SendAckErr(g_tModS.RspCode);	/* 告诉主机命令错误 */
		break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: MODS_03H
*	功能说明: 读取保持寄存器 在一个或多个保持寄存器中取得当前的二进制值
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MODS_03H(void)
{
	/*
	从机地址为11H。保持寄存器的起始地址为006BH，结束地址为006DH。该次查询总共访问3个保持寄存器。

	主机发送:
	11 从机地址
	03 功能码
	00 寄存器地址高字节
	6B 寄存器地址低字节
	00 寄存器数量高字节
	03 寄存器数量低字节
	76 CRC高字节
	87 CRC低字节

	从机应答: 	保持寄存器的长度为2个字节。对于单个保持寄存器而言，寄存器高字节数据先被传输，
	低字节数据后被传输。保持寄存器之间，低地址寄存器先被传输，高地址寄存器后被传输。
	11 从机地址
	03 功能码
	06 字节数
	00 数据1高字节(006BH)
	6B 数据1低字节(006BH)
	00 数据2高字节(006CH)
	13 数据2 低字节(006CH)
	00 数据3高字节(006DH)
	00 数据3低字节(006DH)
	38 CRC高字节
	B9 CRC低字节

	例子:
	01 03 30 06 00 01  6B0B      ---- 读 3006H, 触发电流
	01 03 4000 0010 51C6         ---- 读 4000H 倒数第1条浪涌记录 32字节
	01 03 4001 0010 0006         ---- 读 4001H 倒数第1条浪涌记录 32字节

	01 03 F000 0008 770C         ---- 读 F000H 倒数第1条告警记录 16字节
	01 03 F001 0008 26CC         ---- 读 F001H 倒数第2条告警记录 16字节

	01 03 7000 0020 5ED2         ---- 读 7000H 倒数第1条波形记录第1段 64字节
	01 03 7001 0020 0F12         ---- 读 7001H 倒数第1条波形记录第2段 64字节

	01 03 7040 0020 5F06         ---- 读 7040H 倒数第2条波形记录第1段 64字节
	*/
	uint16_t reg;
	uint16_t num;
	uint16_t i;
	//uint8_t reg_value[64];
	uint8_t temp[8];
	uint16_t LocalStatusArrayTemp;

	g_tModS.RspCode = RSP_OK;

	if (g_tModS.RxCount != 8)								/* 03H命令必须是8个字节 */
	{
		g_tModS.RspCode = RSP_ERR_VALUE;					/* 数据值域错误 */
		goto err_ret;
	}

	reg = BEBufToUint16(&g_tModS.RxBuf[2]); 				/* 寄存器号 */
	num = BEBufToUint16(&g_tModS.RxBuf[4]);					/* 寄存器个数 */
	if (num > LOCAL_ARRAY_LENGTH)
	{
		g_tModS.RspCode = RSP_ERR_VALUE;					/* 数据值域错误 */
		goto err_ret;
	}

err_ret:
	if (g_tModS.RspCode == RSP_OK)							/* 正确应答 */
	{
		g_tModS.TxCount = 0;
		g_tModS.TxBuf[g_tModS.TxCount++] = g_tModS.RxBuf[0];
		g_tModS.TxBuf[g_tModS.TxCount++] = g_tModS.RxBuf[1];
		g_tModS.TxBuf[g_tModS.TxCount++] = num * 2;			/* 返回字节数 */

		for (i = 0; i < num; i++)
		{
			LocalStatusArrayTemp = localArray[reg + i];	//读取的是16位数组，转换为2个8位数据存入发送数组
			g_tModS.TxBuf[g_tModS.TxCount++] = LocalStatusArrayTemp >> 8;
			g_tModS.TxBuf[g_tModS.TxCount++] = LocalStatusArrayTemp & 0xff;
		}
		MODS_SendWithCRC(g_tModS.TxBuf, g_tModS.TxCount);	/* 发送正确应答 */
		usart1_led_toggle();
		modbusSlave03DataProcess();
	}
	else
	{
		MODS_SendAckErr(g_tModS.RspCode);					/* 发送错误应答 */
	}
}


/*
*********************************************************************************************************
*	函 数 名: MODS_06H
*	功能说明: 写单个寄存器
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MODS_06H(void)
{

	/*
	写保持寄存器。注意06指令只能操作单个保持寄存器，16指令可以设置单个或多个保持寄存器

	主机发送:
	11 从机地址
	06 功能码
	00 寄存器地址高字节
	01 寄存器地址低字节
	00 数据1高字节
	01 数据1低字节
	9A CRC校验高字节
	9B CRC校验低字节

	从机响应:
	11 从机地址
	06 功能码
	00 寄存器地址高字节
	01 寄存器地址低字节
	00 数据1高字节
	01 数据1低字节
	1B CRC校验高字节
	5A	CRC校验低字节

	例子:
	01 06 30 06 00 25  A710    ---- 触发电流设置为 2.5
	01 06 30 06 00 10  6707    ---- 触发电流设置为 1.0


	01 06 30 1B 00 00  F6CD    ---- SMA 滤波系数 = 0 关闭滤波
	01 06 30 1B 00 01  370D    ---- SMA 滤波系数 = 1
	01 06 30 1B 00 02  770C    ---- SMA 滤波系数 = 2
	01 06 30 1B 00 05  36CE    ---- SMA 滤波系数 = 5

	01 06 30 07 00 01  F6CB    ---- 测试模式修改为 T1
	01 06 30 07 00 02  B6CA    ---- 测试模式修改为 T2

	01 06 31 00 00 00  8736    ---- 擦除浪涌记录区
	01 06 31 01 00 00  D6F6    ---- 擦除告警记录区

	*/

	uint16_t reg;
	uint16_t value;

	g_tModS.RspCode = RSP_OK;

	if (g_tModS.RxCount != 8)
	{
		g_tModS.RspCode = RSP_ERR_VALUE;		/* 数据值域错误 */
		goto err_ret;
	}

	reg = BEBufToUint16(&g_tModS.RxBuf[2]); 	/* 寄存器号 */
	value = BEBufToUint16(&g_tModS.RxBuf[4]);	/* 寄存器值 */

err_ret:
	if (g_tModS.RspCode == RSP_OK)				/* 正确应答 */
	{
		MODS_SendAckOk();
	}
	else
	{
		MODS_SendAckErr(g_tModS.RspCode);		/* 告诉主机命令错误 */
	}
}


/*
*********************************************************************************************************
*	函 数 名: MODS_10H
*	功能说明: 连续写多个寄存器.  进用于改写时钟
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MODS_10H(void)
{
	/*
	从机地址为11H。保持寄存器的其实地址为0001H，寄存器的结束地址为0002H。总共访问2个寄存器。
	保持寄存器0001H的内容为000AH，保持寄存器0002H的内容为0102H。

	主机发送:
	11 从机地址
	10 功能码
	00 寄存器起始地址高字节
	01 寄存器起始地址低字节
	00 寄存器数量高字节
	02 寄存器数量低字节
	04 字节数
	00 数据1高字节
	0A 数据1低字节
	01 数据2高字节
	02 数据2低字节
	C6 CRC校验高字节
	F0 CRC校验低字节

	从机响应:
	11 从机地址
	06 功能码
	00 寄存器地址高字节
	01 寄存器地址低字节
	00 数据1高字节
	01 数据1低字节
	1B CRC校验高字节
	5A	CRC校验低字节

	例子:
	01 10 30 00 00 06 0C  07 DE  00 0A  00 01  00 08  00 0C  00 00     389A    ---- 写时钟 2014-10-01 08:12:00
	01 10 30 00 00 06 0C  07 DF  00 01  00 1F  00 17  00 3B  00 39     5549    ---- 写时钟 2015-01-31 23:59:57

	*/
	uint16_t reg_addr;
	uint16_t reg_num;
	uint8_t byte_num;
	uint8_t i;
	uint16_t value;

	g_tModS.RspCode = RSP_OK;

	if (g_tModS.RxCount < 11)
	{
		g_tModS.RspCode = RSP_ERR_VALUE;			/* 数据值域错误 */
		goto err_ret;
	}

	reg_addr = BEBufToUint16(&g_tModS.RxBuf[2]); 	/* 寄存器号 */
	reg_num = BEBufToUint16(&g_tModS.RxBuf[4]);		/* 寄存器个数 */
	byte_num = g_tModS.RxBuf[6];					/* 后面的数据体字节数 */

	if (byte_num != 2 * reg_num)
	{
		;
	}

	for (i = 0; i < reg_num; i++)
	{
		value = BEBufToUint16(&g_tModS.RxBuf[7 + 2 * i]);	/* 寄存器值 */
		localArray[reg_addr + i] = value;
	}

err_ret:
	if (g_tModS.RspCode == RSP_OK)					/* 正确应答 */
	{
		MODS_SendAckOk();
		modbusSlave10DataProcess();
	}
	else
	{
		MODS_SendAckErr(g_tModS.RspCode);			/* 告诉主机命令错误 */
	}
}