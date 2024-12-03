/* USER CODE BEGIN Header */
/**
 **********
 * @file           : main.c
 * @brief          : Main program body
 **********
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 **********
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>
#include "Driver_LED.h"

/* Private defines -----------------------------------------------------------*/
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

Driver_LED_HandleTypeDef led_driver;
/* Redirect printf to UART */
int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
	return ch;
}

/* USER CODE BEGIN 0 */


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


	Driver_LED_HandleTypeDef led_driver = {
			.hspi = &hspi3,
			.cs_port = GPIOB,
			.cs_pin = GPIO_PIN_7
	};
	/* Initialize the GPIO Expander */
	Driver_LED_Init(&led_driver);



	/* Infinite loop */
	while (1) {


		Driver_LED_SetLEDB(&led_driver, 0x00); // Toutes les LEDs sur PORTA ON
		HAL_Delay(100);

		// Éteindre toutes les LEDs sur PORTA
		Driver_LED_SetLEDB(&led_driver, 0xFF); // Toutes les LEDs sur PORTA OFF
		HAL_Delay(100);

		Driver_LED_SetLEDA(&led_driver, 0x00); // Toutes les LEDs sur PORTA ON
		HAL_Delay(100);

		// Éteindre toutes les LEDs sur PORTA
		Driver_LED_SetLEDA(&led_driver, 0xFF); // Toutes les LEDs sur PORTA OFF
		HAL_Delay(100);

		// Boucle pour allumer les LEDs sur PORTA (LEDs 1 à 8)
		for (uint8_t i = 0; i < 8; i++) {
			Driver_LED_SetLEDA(&led_driver, ~(1 << i)); // Active bas : ~ pour inverser
			HAL_Delay(500);
			Driver_LED_SetLEDA(&led_driver, 0xFF); // Éteindre après chaque étape
		}

		// Boucle pour allumer les LEDs sur PORTB (LEDs 9 à 16)
		for (uint8_t i = 0; i < 8; i++) {
			Driver_LED_SetLEDB(&led_driver, ~(1 << i)); // Active bas : ~ pour inverser
			HAL_Delay(500);
			Driver_LED_SetLEDB(&led_driver, 0xFF); // Éteindre après chaque étape
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
