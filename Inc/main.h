/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define led_uart2_state_Pin GPIO_PIN_2
#define led_uart2_state_GPIO_Port GPIOE
#define key_humi_down_Pin GPIO_PIN_0
#define key_humi_down_GPIO_Port GPIOC
#define key_humi_up_Pin GPIO_PIN_1
#define key_humi_up_GPIO_Port GPIOC
#define led_key4_Pin GPIO_PIN_2
#define led_key4_GPIO_Port GPIOC
#define key_4_Pin GPIO_PIN_3
#define key_4_GPIO_Port GPIOC
#define led_key3_Pin GPIO_PIN_0
#define led_key3_GPIO_Port GPIOA
#define key_3_Pin GPIO_PIN_1
#define key_3_GPIO_Port GPIOA
#define temp_dac_out_Pin GPIO_PIN_4
#define temp_dac_out_GPIO_Port GPIOA
#define humi_dac_out_Pin GPIO_PIN_5
#define humi_dac_out_GPIO_Port GPIOA
#define temp_adc_in_Pin GPIO_PIN_6
#define temp_adc_in_GPIO_Port GPIOA
#define humi_adv_in_Pin GPIO_PIN_7
#define humi_adv_in_GPIO_Port GPIOA
#define press_adc_in_Pin GPIO_PIN_4
#define press_adc_in_GPIO_Port GPIOC
#define key_2_Pin GPIO_PIN_5
#define key_2_GPIO_Port GPIOC
#define led_key2_Pin GPIO_PIN_0
#define led_key2_GPIO_Port GPIOB
#define switch_in1_Pin GPIO_PIN_1
#define switch_in1_GPIO_Port GPIOB
#define switch_in2_Pin GPIO_PIN_7
#define switch_in2_GPIO_Port GPIOE
#define switch_in3_Pin GPIO_PIN_8
#define switch_in3_GPIO_Port GPIOE
#define switch_in4_Pin GPIO_PIN_9
#define switch_in4_GPIO_Port GPIOE
#define switch_in5_Pin GPIO_PIN_10
#define switch_in5_GPIO_Port GPIOE
#define relay5_Pin GPIO_PIN_11
#define relay5_GPIO_Port GPIOE
#define relay4_Pin GPIO_PIN_12
#define relay4_GPIO_Port GPIOE
#define relay3_Pin GPIO_PIN_13
#define relay3_GPIO_Port GPIOE
#define relay2_Pin GPIO_PIN_14
#define relay2_GPIO_Port GPIOE
#define relay1_Pin GPIO_PIN_15
#define relay1_GPIO_Port GPIOE
#define key_1_Pin GPIO_PIN_10
#define key_1_GPIO_Port GPIOB
#define led_key1_Pin GPIO_PIN_11
#define led_key1_GPIO_Port GPIOB
#define key_temp_up_Pin GPIO_PIN_12
#define key_temp_up_GPIO_Port GPIOB
#define led_state_5_Pin GPIO_PIN_13
#define led_state_5_GPIO_Port GPIOB
#define led_state_4_Pin GPIO_PIN_14
#define led_state_4_GPIO_Port GPIOB
#define led_state_3_Pin GPIO_PIN_8
#define led_state_3_GPIO_Port GPIOD
#define led_state_2_Pin GPIO_PIN_9
#define led_state_2_GPIO_Port GPIOD
#define led_state_1_Pin GPIO_PIN_10
#define led_state_1_GPIO_Port GPIOD
#define key_temp_down_Pin GPIO_PIN_11
#define key_temp_down_GPIO_Port GPIOD
#define led_uart1_state_Pin GPIO_PIN_12
#define led_uart1_state_GPIO_Port GPIOD
#define baud_set_Pin GPIO_PIN_8
#define baud_set_GPIO_Port GPIOA
#define key_mode_set_Pin GPIO_PIN_12
#define key_mode_set_GPIO_Port GPIOA
#define digtal_led_dp_Pin GPIO_PIN_10
#define digtal_led_dp_GPIO_Port GPIOC
#define digtal_led_g_Pin GPIO_PIN_11
#define digtal_led_g_GPIO_Port GPIOC
#define digtal_led_f_Pin GPIO_PIN_12
#define digtal_led_f_GPIO_Port GPIOC
#define digtal_led_e_Pin GPIO_PIN_2
#define digtal_led_e_GPIO_Port GPIOD
#define digtal_led_d_Pin GPIO_PIN_3
#define digtal_led_d_GPIO_Port GPIOD
#define digtal_led_c_Pin GPIO_PIN_4
#define digtal_led_c_GPIO_Port GPIOD
#define digtal_led_b_Pin GPIO_PIN_5
#define digtal_led_b_GPIO_Port GPIOD
#define digtal_led_a_Pin GPIO_PIN_6
#define digtal_led_a_GPIO_Port GPIOD
#define digtal_led_6_Pin GPIO_PIN_5
#define digtal_led_6_GPIO_Port GPIOB
#define digtal_led_5_Pin GPIO_PIN_6
#define digtal_led_5_GPIO_Port GPIOB
#define digtal_led_4_Pin GPIO_PIN_7
#define digtal_led_4_GPIO_Port GPIOB
#define digtal_led_3_Pin GPIO_PIN_8
#define digtal_led_3_GPIO_Port GPIOB
#define digtal_led_2_Pin GPIO_PIN_9
#define digtal_led_2_GPIO_Port GPIOB
#define digtal_led_1_Pin GPIO_PIN_0
#define digtal_led_1_GPIO_Port GPIOE
#define beep_Pin GPIO_PIN_1
#define beep_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
