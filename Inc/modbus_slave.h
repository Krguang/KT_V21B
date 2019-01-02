#ifndef __MODBUS_SLAVE
#define __MODBUS_SLAVE

/* 03H 读保持寄存器 */
/* 06H 写保持寄存器 */
/* 10H 写多个保存寄存器 */
#define SLAVE_REG_P01		0x0301
#define SLAVE_REG_P02		0x0302


/* RTU 应答代码 */
#define RSP_OK				0		/* 成功 */
#define RSP_ERR_CMD			0x01	/* 不支持的功能码 */
#define RSP_ERR_REG_ADDR	0x02	/* 寄存器地址错误 */
#define RSP_ERR_VALUE		0x03	/* 数据值域错误 */
#define RSP_ERR_WRITE		0x04	/* 写入失败 */

#define S_RX_BUF_SIZE		128
#define S_TX_BUF_SIZE		128


typedef struct
{
	uint8_t RxBuf[S_RX_BUF_SIZE];
	uint8_t RxCount;
	uint8_t RxStatus;
	uint8_t RxNewFlag;

	uint8_t RspCode;

	uint8_t TxBuf[S_TX_BUF_SIZE];
	uint8_t TxCount;
}MODS_T;

typedef struct
{
	/* 03H 06H 读写保持寄存器 */
	uint16_t P01;
	uint16_t P02;

	/* 04H 读取模拟量寄存器 */
	uint16_t A01;

	/* 01H 05H 读写单个强制线圈 */
	uint16_t D01;
	uint16_t D02;
	uint16_t D03;
	uint16_t D04;

}VAR_T;

void MODS_Poll(void);

extern MODS_T g_tModS;
extern VAR_T g_tVar;

#endif // !__MODBUS_SLAVE
