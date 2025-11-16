/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "command.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint8_t readBuffer[10];
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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UARTEx_ReceiveToIdle_IT(&huart2, readBuffer, sizeof(readBuffer));
  uint16_t CommandLength;
  uint8_t command[50];
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    CommandLength = Command_GetCommand(command);
    if(CommandLength != 0)
    {
      /************************** */
      // uint8_t success_msg[] = "/r/nCommand received, CommandLength = ";
      // HAL_UART_Transmit(&huart2 , success_msg, sizeof(success_msg) - 1, HAL_MAX_DELAY); //接收到命令的提示信息

      // // 发送接收到的命令长度
      // uint8_t str[10];
      // sprintf((char*)str, "%d/r/n", CommandLength);
      // HAL_UART_Transmit(&huart2, str, strlen((char*)str), HAL_MAX_DELAY);
      /************************** */

      HAL_UART_Transmit(&huart2, command, CommandLength,HAL_MAX_DELAY);
    }
    else
    {
      /***************** */
      // static int last_tick = 0;
      // if(HAL_GetTick() - last_tick >= 2000) //每2秒
      // {
      //   uint8_t no_com_msg[] = "No command received\r\n";
      //   uint8_t send_size = sizeof(no_com_msg) - 1;
      //   HAL_UART_Transmit(&huart2, no_com_msg, sizeof(no_com_msg) - 1, HAL_MAX_DELAY);

      //   last_tick = HAL_GetTick();

      // }
      /***************** */
    }
    HAL_Delay(10);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART2) {
    uint8_t error_msg[] = "UART Error!\r\n";
    HAL_UART_Transmit(&huart2, error_msg, sizeof(error_msg)-1, HAL_MAX_DELAY);
  }
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if(huart->Instance == USART2) 
  {
    /*********************** */
    // uint8_t debug_msg[] = "\r\n*** CALLBACK ENTERED! Size=";
    // HAL_UART_Transmit(&huart2, debug_msg, sizeof(debug_msg)-1, HAL_MAX_DELAY);
    
    // // 发送接收到的数据大小
    // uint8_t size_str[4];
    // sprintf((char*)size_str, "%d", Size);
    // HAL_UART_Transmit(&huart2, size_str, strlen((char*)size_str), HAL_MAX_DELAY);
    
    // uint8_t newline[] = " ***\r\n";
    // HAL_UART_Transmit(&huart2, newline, sizeof(newline)-1, HAL_MAX_DELAY);
    
    Command_Write(readBuffer, Size);
    // uint8_t write_size = 
    // uint8_t write_size_str[4];
    // uint8_t write_size_msg[] = "readBuffer writen size = ";
    // sprintf((char*)write_size_str, "%d/r/n",write_size);

    // HAL_UART_Transmit(&huart2, write_size_msg, sizeof(write_size_msg) - 1, HAL_MAX_DELAY);
    // HAL_UART_Transmit(&huart2, write_size_str, sizeof(write_size_str) - 1, HAL_MAX_DELAY);
    /*********************** */
    HAL_UARTEx_ReceiveToIdle_IT(&huart2, readBuffer, sizeof(readBuffer));
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
