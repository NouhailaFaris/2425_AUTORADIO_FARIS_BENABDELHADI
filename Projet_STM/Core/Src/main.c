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
#include "cmsis_os.h"
#include "dma.h"
#include "i2c.h"
#include "sai.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "sgtl5000.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SGTL5000_I2C_ADDR  0x14  // Adresse I2C du SGTL5000
#define CHIP_ID_REG        0x0000 // Adresse du registre CHIP_ID
#define TEST_REGISTER      0x0030//CHIP_ANA_POWER
#define BUFFER_SIZE 256  // Taille du buffer DMA
uint16_t tx_buffer[BUFFER_SIZE];  // Buffer pour les données à transmettre
uint16_t rx_buffer[BUFFER_SIZE];  // Buffer pour les données reçues
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
	return ch;
}

void I2C_Scan(void) {
	printf("Scanning I2C bus\r\n");
	for (uint16_t addr = 0; addr < 128; addr++) {
		if (HAL_I2C_IsDeviceReady(&hi2c2, addr , 2, HAL_MAX_DELAY) == HAL_OK) {
			printf("Périphérique trouvé à l'adresse 0x%02X\r\n", addr);
		}
	}
	printf("Scan terminé.\r\n");
}

void Read_CHIP_ID_Debug(void) {
	uint8_t chip_id[2] = {0};

	printf("Lecture du registre CHIP_ID \r\n");

	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c2, SGTL5000_I2C_ADDR, CHIP_ID_REG, I2C_MEMADD_SIZE_16BIT, chip_id, 2, HAL_MAX_DELAY);
	if (status == HAL_OK) {
		// Appliquer le décalage de 8 bits pour le MSB
		uint16_t chip_id_val = (chip_id[0] << 8) | chip_id[1];
		printf("CHIP_ID lu avec succès : 0x%04X\r\n", chip_id_val);
	} else {
		printf("Erreur : Impossible de lire le CHIP_ID\r\n");
		uint32_t i2c_error = HAL_I2C_GetError(&hi2c2);
		printf("Erreur I2C\r\n", i2c_error);
	}
}
void Read_Write_Test_Register(void) {
	uint8_t read_data[2] = {0};
	uint16_t test_value = 0xABCD; // Exemple de valeur à écrire
	uint8_t test_data[2] = { (test_value >> 8) & 0xFF, test_value & 0xFF };

	printf("Lecture du registre avant écriture\r\n");
	if (HAL_I2C_Mem_Read(&hi2c2, SGTL5000_I2C_ADDR, TEST_REGISTER, I2C_MEMADD_SIZE_16BIT, read_data, 2, HAL_MAX_DELAY) == HAL_OK) {
		uint16_t value = (read_data[0]) | read_data[1];
		printf("Valeur lue avant écriture : 0x%04X\r\n", value);
	} else {
		printf("Erreur lors de la lecture du registre avant écriture\r\n");
	}

	printf("Écriture dans le registre\r\n");
	if (HAL_I2C_Mem_Write(&hi2c2, SGTL5000_I2C_ADDR, TEST_REGISTER, I2C_MEMADD_SIZE_16BIT, test_data, 2, HAL_MAX_DELAY) == HAL_OK) {
		printf("Valeur écrite avec succès : 0x%04X\r\n", test_value);
	} else {
		printf("Erreur lors de l'écriture dans le registre\r\n");
	}

	printf("Lecture du registre après écriture\r\n");
	if (HAL_I2C_Mem_Read(&hi2c2, SGTL5000_I2C_ADDR, TEST_REGISTER, I2C_MEMADD_SIZE_16BIT, read_data, 2, HAL_MAX_DELAY) == HAL_OK) {
		uint16_t value = (read_data[0] << 8) | read_data[1];
		printf("Valeur lue après écriture : 0x%04X\r\n", value);
	} else {
		printf("Erreur lors de la lecture du registre après écriture\r\n");
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

	/* Configure the peripherals common clocks */
	PeriphCommonClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART2_UART_Init();
	MX_SPI3_Init();
	MX_I2C2_Init();
	MX_SAI2_Init();
	/* USER CODE BEGIN 2 */
	__HAL_SAI_ENABLE(&hsai_BlockA2);
	printf("Demarrage du systeme...\r\n");
	I2C_Scan();
	Read_CHIP_ID_Debug();
	Read_Write_Test_Register();
	printf("Début des tests des registres SGTL5000\r\n");

	// Tester chaque registre
	Test_Register(&hi2c2, CHIP_ANA_POWER, 0x6AFF);
	Test_Register(&hi2c2, CHIP_LINREG_CTRL, 0x006C);
	Test_Register(&hi2c2, CHIP_REF_CTRL, 0x004E);
	Test_Register(&hi2c2, CHIP_LINE_OUT_CTRL, 0x0322);
	Test_Register(&hi2c2, CHIP_SHORT_CTRL, 0x1106);
	Test_Register(&hi2c2, CHIP_ANA_CTRL, 0x0133);
	Test_Register(&hi2c2, CHIP_DIG_POWER, 0x0073);
	Test_Register(&hi2c2, CHIP_LINE_OUT_VOL, 0x0505);
	Test_Register(&hi2c2, CHIP_CLK_CTRL, 0x0002);
	Test_Register(&hi2c2, CHIP_I2S_CTRL, 0x0001);
	Test_Register(&hi2c2, CHIP_ADCDAC_CTRL, 0x000C);
	Test_Register(&hi2c2, CHIP_DAC_VOL, 0x3C3C);

	printf("Tests terminés.\r\n");
	printf("Démarrage du système...\r\n");
	/* Démarrer la transmission DMA */
	if (HAL_SAI_Transmit_DMA(&hsai_BlockA2, tx_buffer, BUFFER_SIZE) != HAL_OK) {
		printf("Erreur : Impossible de démarrer la transmission I2S\r\n");
		Error_Handler();
	}

	printf("Transmission I2S démarrée avec succès\r\n");

	/* USER CODE END 2 */

	/* Call init function for freertos objects (in cmsis_os2.c) */
	MX_FREERTOS_Init();

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
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

/**
 * @brief Peripherals Common Clock Configuration
 * @retval None
 */
void PeriphCommonClock_Config(void)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the peripherals clock
	 */
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SAI2;
	PeriphClkInit.Sai2ClockSelection = RCC_SAI2CLKSOURCE_PLLSAI1;
	PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSI;
	PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
	PeriphClkInit.PLLSAI1.PLLSAI1N = 13;
	PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV17;
	PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
