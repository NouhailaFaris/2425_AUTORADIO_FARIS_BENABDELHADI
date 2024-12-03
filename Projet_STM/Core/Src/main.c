/* USER CODE BEGIN Header */
/**
 **************************
 * @file           : main.c
 * @brief          : Main program body
 **************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 **************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>

/* Private defines -----------------------------------------------------------*/
#define MCP23S17_WRITE_OPCODE  0x40  // Opcode pour écriture dans le MCP23S17
#define MCP23S17_IODIRB        0x01  // Registre pour configurer les broches du PORTB
#define MCP23S17_GPIOB         0x13  // Registre pour contrôler les LEDs sur PORTB
#define MCP23S17_IODIRA        0x00  // Registre pour configurer les broches du PORTA
#define MCP23S17_GPIOA         0x12  // Registre pour contrôler les LEDs sur PORTA

#define GPIO_EXPANDER_CS_PIN   GPIO_PIN_7
#define GPIO_EXPANDER_CS_PORT  GPIOB

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void GPIOExpander_Select(void);
void GPIOExpander_Deselect(void);
void GPIOExpander_WriteRegister(uint8_t reg, uint8_t value);
void GPIOExpander_Init(void);
void GPIOExpander_SetLED(uint8_t value);

/* Redirect printf to UART */
int __io_putchar(int ch) {
    HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

/* USER CODE BEGIN 0 */

// Fonction pour sélectionner le GPIO Expander
void GPIOExpander_Select(void) {

    HAL_GPIO_WritePin(GPIO_EXPANDER_CS_PORT, GPIO_EXPANDER_CS_PIN, GPIO_PIN_RESET);
    printf("CS LOW\r\n");
}

// Fonction pour désélectionner le GPIO Expander
void GPIOExpander_Deselect(void) {

    HAL_GPIO_WritePin(GPIO_EXPANDER_CS_PORT, GPIO_EXPANDER_CS_PIN, GPIO_PIN_SET);
    printf("CS HIGH\r\n");
}

// Fonction pour écrire dans un registre du MCP23S17
void GPIOExpander_WriteRegister(uint8_t reg, uint8_t value) {
    uint8_t data[3] = {MCP23S17_WRITE_OPCODE, reg, value};

    GPIOExpander_Select();
    HAL_SPI_Transmit(&hspi3, data, 3, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi3, &value, 3, HAL_MAX_DELAY);
    HAL_Delay(1);
    GPIOExpander_Deselect();
    HAL_Delay(1);
    printf("Valeur lue : 0x%02X\r\n", value);
}

// Initialisation du MCP23S17
void GPIOExpander_Init(void) {
    printf("Initialisation du GPIO Expander...\r\n");
    GPIOExpander_WriteRegister(MCP23S17_IODIRB, 0x00);
    GPIOExpander_WriteRegister(MCP23S17_GPIOB, 0x00);
    GPIOExpander_WriteRegister(MCP23S17_IODIRA, 0x00);
    GPIOExpander_WriteRegister(MCP23S17_GPIOA, 0x00);
    printf("Initialisation terminée\r\n");
}

// Fonction pour contrôler les LEDs via GPIOA
void GPIOExpander_SetLEDA(uint8_t value) {


    GPIOExpander_WriteRegister(MCP23S17_GPIOA, value);
}
void GPIOExpander_SetLEDB(uint8_t value) {

    GPIOExpander_WriteRegister(MCP23S17_GPIOB, value);
   }
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* MCU Configuration--------------------------------------------------------*/
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_SPI3_Init();
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);

    /* Initialize the GPIO Expander */
    GPIOExpander_Init();

    printf("Démarrage de l'effet chenillard\r\n");

    /* Infinite loop */
    while (1) {


            // Chenillard sur PORTB
            for (int i = 0; i < 8; i++) {
                GPIOExpander_SetLEDA(1 << i);
                HAL_Delay(500);
            }
            for (int i = 0; i < 8; i++) {
                            GPIOExpander_SetLEDB(1 << i);
                            HAL_Delay(500);
                        }
    }
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
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
	__disable_irq();
	while (1)
	{
	}
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
