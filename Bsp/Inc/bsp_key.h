#ifndef __BSP_KEY
#define __BSP_KEY

#define KEY_COUNT    10	   					/* 按键个数, 9个独立建 + 1个组合键 */

/* 按键ID, 主要用于bsp_KeyState()函数的入口参数 */
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




#endif // !__BSP_KEY
