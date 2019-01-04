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
*	�� �� ��: MODS_Poll
*	����˵��: �������ݰ�. �����������������á�
*	��    ��: ��
*	�� �� ֵ: ��
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

		if (g_tModS.RxCount < 4)				/* ���յ�������С��4���ֽھ���Ϊ���� */
		{
			goto err_ret;
		}

		/* ����CRCУ��� */
		crc1 = CRC16_Modbus(g_tModS.RxBuf, g_tModS.RxCount);
		if (crc1 != 0)
		{
			goto err_ret;
		}

		/* վ��ַ (1�ֽڣ� */
		addr = usart1_rx_buffer[0];				/* ��1�ֽ� վ�� */
		if (addr != flash.h2Set)		 			/* �ж��������͵������ַ�Ƿ���� */
		{
			goto err_ret;
		}

		/* ����Ӧ�ò�Э�� */
		MODS_AnalyzeApp();

	err_ret:
		usart1_rx_flag = 0;					/* ��������������������´�֡ͬ�� */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: MODS_SendWithCRC
*	����˵��: ����һ������, �Զ�׷��2�ֽ�CRC
*	��    ��: _pBuf ���ݣ�
*			  _ucLen ���ݳ��ȣ�����CRC��
*	�� �� ֵ: ��
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
*	�� �� ��: MODS_SendAckErr
*	����˵��: ���ʹ���Ӧ��
*	��    ��: _ucErrCode : �������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void MODS_SendAckErr(uint8_t _ucErrCode)
{
	uint8_t txbuf[3];

	txbuf[0] = g_tModS.RxBuf[0];					/* 485��ַ */
	txbuf[1] = g_tModS.RxBuf[1] | 0x80;				/* �쳣�Ĺ����� */
	txbuf[2] = _ucErrCode;							/* �������(01,02,03,04) */

	MODS_SendWithCRC(txbuf, 3);
}

/*
*********************************************************************************************************
*	�� �� ��: MODS_SendAckOk
*	����˵��: ������ȷ��Ӧ��.
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: MODS_ReadRegValue
*	����˵��: ��ȡ���ּĴ�����ֵ
*	��    ��: reg_addr �Ĵ�����ַ
*			  reg_value ��żĴ������
*	�� �� ֵ: 1��ʾOK 0��ʾ����
*********************************************************************************************************
*/
static uint8_t MODS_ReadRegValue(uint16_t reg_addr, uint8_t *reg_value)
{
	uint16_t value;

	switch (reg_addr)									/* �жϼĴ�����ַ */
	{
	case SLAVE_REG_P01:
		value = g_tVar.P01;
		break;

	case SLAVE_REG_P02:
		value = g_tVar.P02;							/* ���Ĵ���ֵ���� */
		break;

	default:
		return 0;									/* �����쳣������ 0 */
	}

	reg_value[0] = value >> 8;
	reg_value[1] = value;

	return 1;											/* ��ȡ�ɹ� */
}

/*
*********************************************************************************************************
*	�� �� ��: MODS_WriteRegValue
*	����˵��: ��ȡ���ּĴ�����ֵ
*	��    ��: reg_addr �Ĵ�����ַ
*			  reg_value �Ĵ���ֵ
*	�� �� ֵ: 1��ʾOK 0��ʾ����
*********************************************************************************************************
*/
static uint8_t MODS_WriteRegValue(uint16_t reg_addr, uint16_t reg_value)
{
	switch (reg_addr)							/* �жϼĴ�����ַ */
	{
	case SLAVE_REG_P01:
		g_tVar.P01 = reg_value;				/* ��ֵд�뱣��Ĵ��� */
		break;

	case SLAVE_REG_P02:
		g_tVar.P02 = reg_value;				/* ��ֵд�뱣��Ĵ��� */
		break;

	default:
		return 0;		/* �����쳣������ 0 */
	}

	return 1;		/* ��ȡ�ɹ� */
}

/*
*********************************************************************************************************
*	�� �� ��: MODS_AnalyzeApp
*	����˵��: ����Ӧ�ò�Э��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void MODS_AnalyzeApp(void)
{
	switch (usart1_rx_buffer[1])				/* ��2���ֽ� ������ */
	{
	case 0x01:							/* ��ȡ��Ȧ״̬����������led���棩*/
		//MODS_01H();
		break;

	case 0x02:							/* ��ȡ����״̬������״̬��*/
		//MODS_02H();
		break;

	case 0x03:							/* ��ȡ���ּĴ����������̴���g_tVar�У�*/
		MODS_03H();
		break;

	case 0x04:							/* ��ȡ����Ĵ�����ADC��ֵ��*/
		//MODS_04H();
		//bsp_PutMsg(MSG_MODS_04H, 0);
		break;

	case 0x05:							/* ǿ�Ƶ���Ȧ������led��*/
		//MODS_05H();
		//bsp_PutMsg(MSG_MODS_05H, 0);
		break;

	case 0x06:							/* д��������Ĵ����������̸�дg_tVar�еĲ�����*/
		MODS_06H();
		//bsp_PutMsg(MSG_MODS_06H, 0);
		break;

	case 0x10:							/* д�������Ĵ����������̴���g_tVar�еĲ�����*/
		MODS_10H();
		break;

	default:
		g_tModS.RspCode = RSP_ERR_CMD;
		MODS_SendAckErr(g_tModS.RspCode);	/* ��������������� */
		break;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: MODS_03H
*	����˵��: ��ȡ���ּĴ��� ��һ���������ּĴ�����ȡ�õ�ǰ�Ķ�����ֵ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void MODS_03H(void)
{
	/*
	�ӻ���ַΪ11H�����ּĴ�������ʼ��ַΪ006BH��������ַΪ006DH���ôβ�ѯ�ܹ�����3�����ּĴ�����

	��������:
	11 �ӻ���ַ
	03 ������
	00 �Ĵ�����ַ���ֽ�
	6B �Ĵ�����ַ���ֽ�
	00 �Ĵ����������ֽ�
	03 �Ĵ����������ֽ�
	76 CRC���ֽ�
	87 CRC���ֽ�

	�ӻ�Ӧ��: 	���ּĴ����ĳ���Ϊ2���ֽڡ����ڵ������ּĴ������ԣ��Ĵ������ֽ������ȱ����䣬
	���ֽ����ݺ󱻴��䡣���ּĴ���֮�䣬�͵�ַ�Ĵ����ȱ����䣬�ߵ�ַ�Ĵ����󱻴��䡣
	11 �ӻ���ַ
	03 ������
	06 �ֽ���
	00 ����1���ֽ�(006BH)
	6B ����1���ֽ�(006BH)
	00 ����2���ֽ�(006CH)
	13 ����2 ���ֽ�(006CH)
	00 ����3���ֽ�(006DH)
	00 ����3���ֽ�(006DH)
	38 CRC���ֽ�
	B9 CRC���ֽ�

	����:
	01 03 30 06 00 01  6B0B      ---- �� 3006H, ��������
	01 03 4000 0010 51C6         ---- �� 4000H ������1����ӿ��¼ 32�ֽ�
	01 03 4001 0010 0006         ---- �� 4001H ������1����ӿ��¼ 32�ֽ�

	01 03 F000 0008 770C         ---- �� F000H ������1���澯��¼ 16�ֽ�
	01 03 F001 0008 26CC         ---- �� F001H ������2���澯��¼ 16�ֽ�

	01 03 7000 0020 5ED2         ---- �� 7000H ������1�����μ�¼��1�� 64�ֽ�
	01 03 7001 0020 0F12         ---- �� 7001H ������1�����μ�¼��2�� 64�ֽ�

	01 03 7040 0020 5F06         ---- �� 7040H ������2�����μ�¼��1�� 64�ֽ�
	*/
	uint16_t reg;
	uint16_t num;
	uint16_t i;
	//uint8_t reg_value[64];
	uint8_t temp[8];
	uint16_t LocalStatusArrayTemp;

	g_tModS.RspCode = RSP_OK;

	if (g_tModS.RxCount != 8)								/* 03H���������8���ֽ� */
	{
		g_tModS.RspCode = RSP_ERR_VALUE;					/* ����ֵ����� */
		goto err_ret;
	}

	reg = BEBufToUint16(&g_tModS.RxBuf[2]); 				/* �Ĵ����� */
	num = BEBufToUint16(&g_tModS.RxBuf[4]);					/* �Ĵ������� */
	if (num > LOCAL_ARRAY_LENGTH)
	{
		g_tModS.RspCode = RSP_ERR_VALUE;					/* ����ֵ����� */
		goto err_ret;
	}

err_ret:
	if (g_tModS.RspCode == RSP_OK)							/* ��ȷӦ�� */
	{
		g_tModS.TxCount = 0;
		g_tModS.TxBuf[g_tModS.TxCount++] = g_tModS.RxBuf[0];
		g_tModS.TxBuf[g_tModS.TxCount++] = g_tModS.RxBuf[1];
		g_tModS.TxBuf[g_tModS.TxCount++] = num * 2;			/* �����ֽ��� */

		for (i = 0; i < num; i++)
		{
			LocalStatusArrayTemp = localArray[reg + i];	//��ȡ����16λ���飬ת��Ϊ2��8λ���ݴ��뷢������
			g_tModS.TxBuf[g_tModS.TxCount++] = LocalStatusArrayTemp >> 8;
			g_tModS.TxBuf[g_tModS.TxCount++] = LocalStatusArrayTemp & 0xff;
		}
		MODS_SendWithCRC(g_tModS.TxBuf, g_tModS.TxCount);	/* ������ȷӦ�� */
		usart1_led_toggle();
		modbusSlave03DataProcess();
	}
	else
	{
		MODS_SendAckErr(g_tModS.RspCode);					/* ���ʹ���Ӧ�� */
	}
}


/*
*********************************************************************************************************
*	�� �� ��: MODS_06H
*	����˵��: д�����Ĵ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void MODS_06H(void)
{

	/*
	д���ּĴ�����ע��06ָ��ֻ�ܲ����������ּĴ�����16ָ��������õ����������ּĴ���

	��������:
	11 �ӻ���ַ
	06 ������
	00 �Ĵ�����ַ���ֽ�
	01 �Ĵ�����ַ���ֽ�
	00 ����1���ֽ�
	01 ����1���ֽ�
	9A CRCУ����ֽ�
	9B CRCУ����ֽ�

	�ӻ���Ӧ:
	11 �ӻ���ַ
	06 ������
	00 �Ĵ�����ַ���ֽ�
	01 �Ĵ�����ַ���ֽ�
	00 ����1���ֽ�
	01 ����1���ֽ�
	1B CRCУ����ֽ�
	5A	CRCУ����ֽ�

	����:
	01 06 30 06 00 25  A710    ---- ������������Ϊ 2.5
	01 06 30 06 00 10  6707    ---- ������������Ϊ 1.0


	01 06 30 1B 00 00  F6CD    ---- SMA �˲�ϵ�� = 0 �ر��˲�
	01 06 30 1B 00 01  370D    ---- SMA �˲�ϵ�� = 1
	01 06 30 1B 00 02  770C    ---- SMA �˲�ϵ�� = 2
	01 06 30 1B 00 05  36CE    ---- SMA �˲�ϵ�� = 5

	01 06 30 07 00 01  F6CB    ---- ����ģʽ�޸�Ϊ T1
	01 06 30 07 00 02  B6CA    ---- ����ģʽ�޸�Ϊ T2

	01 06 31 00 00 00  8736    ---- ������ӿ��¼��
	01 06 31 01 00 00  D6F6    ---- �����澯��¼��

	*/

	uint16_t reg;
	uint16_t value;

	g_tModS.RspCode = RSP_OK;

	if (g_tModS.RxCount != 8)
	{
		g_tModS.RspCode = RSP_ERR_VALUE;		/* ����ֵ����� */
		goto err_ret;
	}

	reg = BEBufToUint16(&g_tModS.RxBuf[2]); 	/* �Ĵ����� */
	value = BEBufToUint16(&g_tModS.RxBuf[4]);	/* �Ĵ���ֵ */

err_ret:
	if (g_tModS.RspCode == RSP_OK)				/* ��ȷӦ�� */
	{
		MODS_SendAckOk();
	}
	else
	{
		MODS_SendAckErr(g_tModS.RspCode);		/* ��������������� */
	}
}


/*
*********************************************************************************************************
*	�� �� ��: MODS_10H
*	����˵��: ����д����Ĵ���.  �����ڸ�дʱ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void MODS_10H(void)
{
	/*
	�ӻ���ַΪ11H�����ּĴ�������ʵ��ַΪ0001H���Ĵ����Ľ�����ַΪ0002H���ܹ�����2���Ĵ�����
	���ּĴ���0001H������Ϊ000AH�����ּĴ���0002H������Ϊ0102H��

	��������:
	11 �ӻ���ַ
	10 ������
	00 �Ĵ�����ʼ��ַ���ֽ�
	01 �Ĵ�����ʼ��ַ���ֽ�
	00 �Ĵ����������ֽ�
	02 �Ĵ����������ֽ�
	04 �ֽ���
	00 ����1���ֽ�
	0A ����1���ֽ�
	01 ����2���ֽ�
	02 ����2���ֽ�
	C6 CRCУ����ֽ�
	F0 CRCУ����ֽ�

	�ӻ���Ӧ:
	11 �ӻ���ַ
	06 ������
	00 �Ĵ�����ַ���ֽ�
	01 �Ĵ�����ַ���ֽ�
	00 ����1���ֽ�
	01 ����1���ֽ�
	1B CRCУ����ֽ�
	5A	CRCУ����ֽ�

	����:
	01 10 30 00 00 06 0C  07 DE  00 0A  00 01  00 08  00 0C  00 00     389A    ---- дʱ�� 2014-10-01 08:12:00
	01 10 30 00 00 06 0C  07 DF  00 01  00 1F  00 17  00 3B  00 39     5549    ---- дʱ�� 2015-01-31 23:59:57

	*/
	uint16_t reg_addr;
	uint16_t reg_num;
	uint8_t byte_num;
	uint8_t i;
	uint16_t value;

	g_tModS.RspCode = RSP_OK;

	if (g_tModS.RxCount < 11)
	{
		g_tModS.RspCode = RSP_ERR_VALUE;			/* ����ֵ����� */
		goto err_ret;
	}

	reg_addr = BEBufToUint16(&g_tModS.RxBuf[2]); 	/* �Ĵ����� */
	reg_num = BEBufToUint16(&g_tModS.RxBuf[4]);		/* �Ĵ������� */
	byte_num = g_tModS.RxBuf[6];					/* ������������ֽ��� */

	if (byte_num != 2 * reg_num)
	{
		;
	}

	for (i = 0; i < reg_num; i++)
	{
		value = BEBufToUint16(&g_tModS.RxBuf[7 + 2 * i]);	/* �Ĵ���ֵ */
		localArray[reg_addr + i] = value;
	}

err_ret:
	if (g_tModS.RspCode == RSP_OK)					/* ��ȷӦ�� */
	{
		MODS_SendAckOk();
		modbusSlave10DataProcess();
	}
	else
	{
		MODS_SendAckErr(g_tModS.RspCode);			/* ��������������� */
	}
}