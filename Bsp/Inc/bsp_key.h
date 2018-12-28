#ifndef __BSP_KEY
#define __BSP_KEY

#define KEY_COUNT    10	   					/* ��������, 9�������� + 1����ϼ� */

/* ����ID, ��Ҫ����bsp_KeyState()��������ڲ��� */
typedef enum
{
	KID_K1 = 0,
	KID_K2,
	KID_K3,
	KID_K4,
	KID_TEMP_UP,
	KID_TEMP_DOWN,
	KID_HUMI_UP,
	KID_HUMI_DOWN,
	KID_MODE_SET
}KEY_ID_E;

/*
�����˲�ʱ��50ms, ��λ10ms��
ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
��ʹ������·����Ӳ���˲������˲�����Ҳ���Ա�֤�ɿ��ؼ�⵽�����¼�
*/
#define KEY_FILTER_TIME   5
#define KEY_LONG_TIME     100			/* ��λ10ms�� ����1�룬��Ϊ�����¼� */

/*
ÿ��������Ӧ1��ȫ�ֵĽṹ�������
*/
typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	uint8_t(*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

	uint8_t  Count;			/* �˲��������� */
	uint16_t LongCount;		/* ���������� */
	uint16_t LongTime;		/* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint8_t  State;			/* ������ǰ״̬�����»��ǵ��� */
	uint8_t  RepeatSpeed;	/* ������������ */
	uint8_t  RepeatCount;	/* �������������� */
}KEY_T;

/*
�����ֵ����, ���밴���´���ʱÿ�����İ��¡�����ͳ����¼�

�Ƽ�ʹ��enum, ����#define��ԭ��
(1) ����������ֵ,�������˳��ʹ���뿴���������
(2) �������ɰ����Ǳ����ֵ�ظ���
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	KEY_1_DOWN,				/* 1������ */
	KEY_1_UP,				/* 1������ */
	KEY_1_LONG,				/* 1������ */

	KEY_2_DOWN,				/* 2������ */
	KEY_2_UP,				/* 2������ */
	KEY_2_LONG,				/* 2������ */

	KEY_3_DOWN,				/* 3������ */
	KEY_3_UP,				/* 3������ */
	KEY_3_LONG,				/* 3������ */

	KEY_4_DOWN,				/* 4������ */
	KEY_4_UP,				/* 4������ */
	KEY_4_LONG,				/* 4������ */

	KEY_5_DOWN,				/* 5������ */
	KEY_5_UP,				/* 5������ */
	KEY_5_LONG,				/* 5������ */

	KEY_6_DOWN,				/* 6������ */
	KEY_6_UP,				/* 6������ */
	KEY_6_LONG,				/* 6������ */

	KEY_7_DOWN,				/* 7������ */
	KEY_7_UP,				/* 7������ */
	KEY_7_LONG,				/* 7������ */

	KEY_8_DOWN,				/* 8������ */
	KEY_8_UP,				/* 8������ */
	KEY_8_LONG,				/* 8������ */

	KEY_9_DOWN,				/* 9������ */
	KEY_9_UP,				/* 9������ */
	KEY_9_LONG,				/* 9������ */

	/* ��ϼ� */
	KEY_10_DOWN,			/* 10������ */
	KEY_10_UP,				/* 10������ */
	KEY_10_LONG,			/* 10������ */
}KEY_ENUM;

/* ����FIFO�õ����� */
#define KEY_FIFO_SIZE	10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ��1 */
	uint8_t Write;					/* ������дָ�� */
	uint8_t Read2;					/* ��������ָ��2 */
}KEY_FIFO_T;

/* ���ⲿ���õĺ������� */
void bsp_InitKey(void);
void bsp_KeyScan(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
uint8_t bsp_GetKey2(void);
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID);
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
void bsp_ClearKey(void);


#endif // !__BSP_KEY
