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
#include <stdlib.h>
#include "Driver_LED.h"
#include "shell.h"
#include "FreeRTOS.h"
#include "task.h"



/* Private defines -----------------------------------------------------------*/
#define TASK_SHELL_STACK_DEPTH 512
#define TASK_SHELL_PRIORITY 1
#define BUFFER_SIZE 64
static char print_buffer[BUFFER_SIZE];
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);

TaskHandle_t h_task_shell = NULL;
Driver_LED_HandleTypeDef led_driver = {
		.hspi = &hspi3,
		.cs_port = GPIOB,
		.cs_pin = GPIO_PIN_7
};/* Redirect printf to UART */

int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
	return ch;
}
static int uart_write(char *s, uint16_t size) {
	HAL_UART_Transmit(&huart2, (uint8_t*)s, size, HAL_MAX_DELAY);
	return size;
}
static int sh_led(int argc, char **argv) {
	if (argc < 3) { // Vérifie si le nombre d'arguments est suffisant
		int size = snprintf(print_buffer, BUFFER_SIZE, "Usage: l <led_number> <state>\r\n");
		uart_write(print_buffer, size);
		return -1;
	}

	int led_number = atoi(argv[1]); // Convertit le numéro de la LED en entier
	int state = atoi(argv[2]);      // Convertit l'état en entier

	if (led_number < 0 || led_number > 15) { // Vérifie si le numéro de LED est valide
		int size = snprintf(print_buffer, BUFFER_SIZE, "Error: Invalid LED number. Must be between 0 and 15.\r\n");
		uart_write(print_buffer, size);
		return -1;
	}

	if (state != 0 && state != 1) { // Vérifie si l'état est valide
		int size = snprintf(print_buffer, BUFFER_SIZE, "Error: Invalid state. Must be 0 or 1.\r\n");
		uart_write(print_buffer, size);
		return -1;
	}

	// Détermine si la LED est sur GPIOA ou GPIOB
	if (led_number < 8) {
		uint8_t mask = 1 << led_number;
		uint8_t current_state = Driver_LED_ReadLEDA(&led_driver);

		if (state == 1) {
			Driver_LED_SetLEDA(&led_driver, current_state & ~mask); // Allume la LED
		} else {
			Driver_LED_SetLEDA(&led_driver, current_state | mask);  // Éteint la LED
		}
	} else {
		uint8_t mask = 1 << (led_number - 8);
		uint8_t current_state = Driver_LED_ReadLEDB(&led_driver);

		if (state == 1) {
			Driver_LED_SetLEDB(&led_driver, current_state & ~mask); // Allume la LED
		} else {
			Driver_LED_SetLEDB(&led_driver, current_state | mask);  // Éteint la LED
		}
	}

	int size = snprintf(print_buffer, BUFFER_SIZE, "LED %d set to %d\r\n", led_number, state);
	uart_write(print_buffer, size);

	return 0;
}

static int sh_led_all_on(int argc, char **argv) {
	Driver_LED_SetLEDA(&led_driver, 0x00); // Allume toutes les LEDs sur PORTA
	Driver_LED_SetLEDB(&led_driver, 0x00); // Allume toutes les LEDs sur PORTB

	int size = snprintf(print_buffer, BUFFER_SIZE, "All LEDs are ON\r\n");
	uart_write(print_buffer, size);
	return 0;
}

void task_shell(void * unused)
{
	shell_init();
	shell_add('l', sh_led, "Led <led_number> <state> - Control LED");
	shell_add('a', sh_led_all_on, "Allumer toutes les LEDs");
	shell_run();	// boucle infinie
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

	// Initialiser le driver avant d'exécuter le shell
	Driver_LED_Init(&led_driver);


	if (xTaskCreate(task_shell, "Shell", TASK_SHELL_STACK_DEPTH, NULL, TASK_SHELL_PRIORITY, &h_task_shell) != pdPASS) {
		printf("Error creating task shell\r\n");
		Error_Handler();
	}

	vTaskStartScheduler();

	/* Infinite loop */
	while (1) {


		//				Driver_LED_SetLEDB(&led_driver, 0x00); // Toutes les LEDs sur PORTA ON
		//				HAL_Delay(100);
		//
		//				// Éteindre toutes les LEDs sur PORTA
		//				Driver_LED_SetLEDB(&led_driver, 0xFF); // Toutes les LEDs sur PORTA OFF
		//				HAL_Delay(100);
		//
		//				Driver_LED_SetLEDA(&led_driver, 0x00); // Toutes les LEDs sur PORTA ON
		//				HAL_Delay(100);
		//
		//				// Éteindre toutes les LEDs sur PORTA
		//				Driver_LED_SetLEDA(&led_driver, 0xFF); // Toutes les LEDs sur PORTA OFF
		//				HAL_Delay(100);
		//
		//				// Boucle pour allumer les LEDs sur PORTA (LEDs 1 à 8)
		//				for (uint8_t i = 0; i < 8; i++) {
		//					Driver_LED_SetLEDA(&led_driver, ~(1 << i)); // Active bas : ~ pour inverser
		//					HAL_Delay(500);
		//					Driver_LED_SetLEDA(&led_driver, 0xFF); // Éteindre après chaque étape
		//				}
		//
		//				// Boucle pour allumer les LEDs sur PORTB (LEDs 9 à 16)
		//				for (uint8_t i = 0; i < 8; i++) {
		//					Driver_LED_SetLEDB(&led_driver, ~(1 << i)); // Active bas : ~ pour inverser
		//					HAL_Delay(500);
		//					Driver_LED_SetLEDB(&led_driver, 0xFF); // Éteindre après chaque étape
		//				}

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
