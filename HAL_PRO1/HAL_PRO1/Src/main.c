/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define LED0_ON() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET) 
#define LED1_ON() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET) 
#define LED0_OFF() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET) 
#define LED1_OFF() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET) 

#define LED0_TOG() HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8)
#define LED1_TOG() HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_2)

uint8_t str1[] = "============WYF欢迎你！============\r\n";
uint8_t hh = 0, mm = 0, ss = 0, ss05 = 0;
uint8_t str_buf[64];	//用来打印时间、LED灯状态
uint8_t Rx_dat[16];	//用来接受串口信息


void LED_Check()
{
	LED0_ON();
	HAL_Delay(500);
	LED1_ON();
	HAL_Delay(500);
	
	LED0_OFF();
	HAL_Delay(500);
	LED1_OFF();
	HAL_Delay(500);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	LED0_TOG();
	ss05++;
	if (ss05 == 2)
	{
		ss++;
		ss05 = 0;
		if (ss == 60)
		{
			ss = 0;
			mm++;
			if (mm == 60)
			{
				
				hh++;
			}
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
	if (huart->Instance == USART1)
	{
		if (Rx_dat[0] == 0xBF&&Rx_dat[2] == 0xFB)
		{
			switch(Rx_dat[1])
			{
			case 0xA1:
				LED1_ON();
				sprintf((char *)str_buf, "%d:%d:%d  LED1打开！\r\n", hh, mm, ss);
				break;
			case 0xA2:
				LED1_OFF();
				sprintf((char *)str_buf, "%d:%d:%d  LED1关闭！\r\n", hh, mm, ss);
				break;
			default:
				sprintf((char *)str_buf, "%d:%d:%d  这是一个错误的命令！\r\n", hh, mm, ss);
				break;
			}
			HAL_UART_Transmit(&huart1, str_buf, sizeof(str_buf), 10000);
		}
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	LED_Check();		//灯光自检
	HAL_UART_Transmit(&huart1, str1, sizeof(str1), 10000);//发送欢迎字符串
	HAL_TIM_Base_Start_IT(&htim2);//打开定时器2，LED循环闪烁并记录系统运行时间
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_UART_Receive_IT(&huart1, Rx_dat, 3);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
