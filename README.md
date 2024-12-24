# 2425_AUTORADIO_FARIS_BENABDELHADI-
Réalisé par FARIS et BENABDELHADI

# Objectif TP

Ce TP vise à développer un système d'autoradio en utilisant une carte STM32 NUCLEO_L476RG avec plusieurs périphériques. L'objectif est de maîtriser la configuration matérielle et logicielle, ainsi que l'intégration des protocoles de communication et des fonctionnalités audio.

# 1 Démarrage 

1-Réference GPIO Expander: MCP23S17-E/SO

2-SPI 1 est utilisé sur le STM32 

3/4-Activation de la fonction printf pour le débogage.

![WhatsApp Image 2024-11-29 at 16 49 13](https://github.com/user-attachments/assets/1ea06745-c192-49a0-85aa-93f2810e6a95)

5- Activation et configuration de FreeRTOS en mode CMSIS V1.

![image](https://github.com/user-attachments/assets/54373661-c2ab-4fd3-921c-7e0393af262d)

6) Mise en place d'un shell fonctionnel :

![WhatsApp Image 2024-11-29 at 16 49 13](https://github.com/user-attachments/assets/91c9f921-2b21-42b3-a5ed-78e3a3b188ea)

![WhatsApp Image 2024-11-29 at 16 56 05](https://github.com/user-attachments/assets/f0fa2efb-67ac-48bc-bc9c-b6b3a79fa6ae)

# 2 Le GPIO Expander et le VU-Metre

## 2.1 Configuration

1-

![image](https://github.com/user-attachments/assets/38418675-5ab3-4d69-b5c0-69a321d16b02)

2- Le SPI 1 est deja utilisé par le STM32

3- il faut configurer le SPI MOSI/ SPI MISO/ SPI SCK/ SPI NCS comme dans le modele Kicad

![image](https://github.com/user-attachments/assets/723572ae-4657-4ad3-933d-c4e695a57bdf)

![image](https://github.com/user-attachments/assets/b3cfbfe9-081d-4cc1-a1e4-334830acffca)

4-configuration sur STM32

![image](https://github.com/user-attachments/assets/3271d4c8-f29a-4b7b-a30a-5862cbd4b577)

![image](https://github.com/user-attachments/assets/be5ced25-f650-4c0f-9ea2-898547a8e047)


## 2.2 Tests

**Remarque**

Pour Tester on a fixer notre NRESET à 1 par un GPIO out (PA0).

![WhatsApp Image 2024-12-03 at 10 49 06](https://github.com/user-attachments/assets/a01e39d6-ecfb-473b-add9-7959a40b2433)

![WhatsApp Image 2024-12-03 at 11 24 23](https://github.com/user-attachments/assets/76d48152-f890-4c09-ad66-b5fd9d58ca21)

## 2.3 Driver 


![image](https://github.com/user-attachments/assets/e74f3bcb-2989-41df-b8e6-621c9ef47de4)

![image](https://github.com/user-attachments/assets/4f3ae65a-d6af-47c6-97bd-3d273e28a9d5)


### Utilisation du Driver avec le shell 

2 fonction sont réalisé :

a = allumer tout les leds 

l avec 2 argument =  pour allumer une led specifique ou l'eteindre 

![image](https://github.com/user-attachments/assets/0e95ceca-a1b2-4067-bc5b-c525b3b96929)


## Le CODEC Audio SGTL5000
### Configuration préalables
Pins utilisées pour l’I2C:

. I2C_SCL : PB10

. I2C_SDA : PB11

Ces broches correspondent au I2C2 dans le STM32 L476RG.

### Configuration:

![image](https://github.com/user-attachments/assets/088c63a5-67f1-4265-9644-69022190d343)

![image](https://github.com/user-attachments/assets/1ffd857c-37e3-4d9d-abd4-fd437adf69e1)

### Signal MCLK

![scope_1](https://github.com/user-attachments/assets/15dea245-b1a5-43cf-a54c-e2c7bacb927a)

![image](https://github.com/user-attachments/assets/5ec7929f-fee0-4a5a-a459-cbe490cafe41)

![image](https://github.com/user-attachments/assets/7c6f0cca-d996-4794-b3c7-0dd7e8ae576d)

### Trames I2C 

![scope_0](https://github.com/user-attachments/assets/97d2ad78-aa4f-4438-bae0-850677dc9583)

**5**

— CHIP_ANA_POWER: 0x6AFF

— CHIP_LINREG_CTRL: 0x006C

— CHIP_REF_CTRL: 0x004E

— CHIP_LINE_OUT_CTRL: 0x0322
  
— CHIP_SHORT_CTRL: 0x1106

— CHIP_ANA_CTRL: 0x0133

— CHIP_DIG_POWER: 0x0073

— CHIP_LINE_OUT_VOL: 0x0505

— CHIP_CLK_CTRL: 0x0002

— CHIP_I2S_CTRL: 0x0001

— CHIP_ADCDAC_CTRL:0x000C

— CHIP_DAC_VOL: 0x3C3C

**config**

![image](https://github.com/user-attachments/assets/38343d67-56f7-4a84-867b-46fa23631baa)

### Signaux I2S:
![image](https://github.com/user-attachments/assets/41c3bbe2-54c8-44bb-a17b-c9f75f2f23f0)

### Observez à l’oscilloscope les différents signaux d’horloge:

### Configuration du  DAC (Digital-to-Analog Converter):

Notre microcontrôleur STM32 possède un convertisseur numérique-analogique (DAC) qui vas être utilisé pour générer des formes d'onde analogiques (un signal triangulaire) .

![image](https://github.com/user-attachments/assets/77c5d397-bbdf-416a-b00e-ef1cdd255ab0)

### Bypass numérique:

***lecture les échantillons de l’ADC, et de l'écriture sur le DAC***

![image](https://github.com/user-attachments/assets/e024d653-a6f7-407f-acde-8d398acdc963)

### Visualisation:

```
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
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "i2c.h"
#include "sai.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "Driver_LED.h"
#include <math.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SGTL5000_I2C_ADDR  0x14  // Adresse I2C du SGTL5000
#define CHIP_ID_REG        0x0000 // Adresse du registre CHIP_ID
#define TEST_REGISTER      0x0030//CHIP_ANA_POWER
//#define TRIANGLE_STEPS 256  // Nombre d'échantillons dans un cycle
//#define BUFFER_SIZE 256  // Taille du buffer DMA
//#define NUM_LEDS 8       // Nombre de LEDs disponibles pour l'affichage du volume sonore
//#define MAX_ADC_VALUE 4095
//uint16_t tx_buffer[BUFFER_SIZE];  // Buffer pour les données à transmettre
//uint16_t rx_buffer[BUFFER_SIZE];  // Buffer pour les données reçues
//uint16_t triangle_wave[TRIANGLE_STEPS];
//uint16_t adc_buffer[BUFFER_SIZE];

/*Driver_LED_HandleTypeDef led_driver = {
		.hspi = &hspi3,
		.cs_port = GPIOB,
		.cs_pin = GPIO_PIN_7
};

float calculate_rms(uint16_t *buffer, uint16_t size);
uint8_t map_rms_to_leds(float rms, float max_rms);
void display_volume_on_leds(uint8_t led_count);*/
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
//void Generate_Triangle_Wave(void);
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


/*void Read_Write_Test_Register(void) {
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
}*/
/*void Generate_Triangle_Wave(void) {
	for (int i = 0; i < TRIANGLE_STEPS / 2; i++) {
		triangle_wave[i] = (4095 * i) / (TRIANGLE_STEPS / 2);  // Croissance linéaire
	}
	for (int i = TRIANGLE_STEPS / 2; i < TRIANGLE_STEPS; i++) {
		triangle_wave[i] = 4095 - ((4095 * (i - TRIANGLE_STEPS / 2)) / (TRIANGLE_STEPS / 2));  // Décroissance linéaire
	}

	// Ajout de vérifications avec printf
	printf("Signal triangulaire genere :\r\n");
	for (int i = 0; i < TRIANGLE_STEPS; i++) {
		printf("Echantillon %d : %d\r\n", i, triangle_wave[i]);
	}
}*/
/*float calculate_rms(uint16_t *buffer, uint16_t size) {
	float sum = 0.0;
	for (int i = 0; i < size; i++) {
		sum += buffer[i] * buffer[i];
	}
	return sqrt(sum / size);

}
uint8_t map_rms_to_leds(float rms, float max_rms) {
	return (uint8_t)((rms / max_rms) * NUM_LEDS);
}
void display_volume_on_leds(uint8_t led_count) {
	uint8_t led_value = 1;

	// Activer le bon nombre de LEDs en mettant les bits correspondants à 1
	for (int i = 0; i < led_count; i++) {
		led_value |= (0 << i);
	}

	// Envoyer la valeur calculée au MCP23S17
	Driver_LED_SetLEDA(&led_driver, ~led_value);  // ~ : Inverser pour allumer les LED
}

void visualize_audio_volume(uint16_t *adc_buffer, uint16_t buffer_size) {
	float rms = calculate_rms(adc_buffer, buffer_size);  // Calcul RMS
	uint8_t led_count = map_rms_to_leds(rms, MAX_ADC_VALUE);  // Mapper RMS à LEDs
	display_volume_on_leds(led_count);  // Afficher le niveau sonore
	printf("led count : %d\r\n", led_count);
}*/
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

	/* USER CODE BEGIN 1 */
	HAL_StatusTypeDef status1, status2;
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
	MX_SPI3_Init(); // Utilisation de SPI3
	MX_I2C2_Init();
	MX_SAI2_Init();
	MX_DAC1_Init();
	MX_TIM6_Init();
	MX_ADC1_Init();

	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
	/* USER CODE BEGIN 2 */
	printf("Demarrage du systeme...\r\n");



	//printf("System initialized, starting audio visualization...\n");
	//__HAL_SAI_ENABLE(&hsai_BlockA2); // Activation du MCLK


	// Génération du signal triangulaire
	//Generate_Triangle_Wave();

	// Démarrer l'ADC
	/*status1 = HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, BUFFER_SIZE);
	if (status1 != HAL_OK) {
		printf("Erreur ADC DMA : %d\r\n", status1);
		Error_Handler();
	}

	// Démarrer le DAC
	status2 = HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, (uint32_t *)adc_buffer, BUFFER_SIZE, DAC_ALIGN_12B_R);
	if (status2 != HAL_OK) {
		printf("Erreur DAC DMA : %d\r\n", status2);
		Error_Handler();
	}

	// Démarrer le Timer 6 pour synchroniser les mises à jour du DAC
	if (HAL_TIM_Base_Start(&htim6) != HAL_OK) {
		printf("Erreur : Impossible de démarrer le Timer 6\r\n");
		Error_Handler();
	}
	printf("Initialisation terminée...\r\n");
	//  printf("Signal triangulaire envoyé au DAC...\r\n");*/


	I2C_Scan();
	Read_CHIP_ID_Debug();
	//Read_Write_Test_Register();
	/*printf("Début des tests des registres SGTL5000\r\n");

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
	    printf("Démarrage du système...\r\n");*/


	/* Initialize buffer for I2S transmission */
	/*  for (int i = 0; i < BUFFER_SIZE; i++) {
	        tx_buffer[i] = i;  // Remplir le buffer avec des données d'exemple
	    }

	    printf("Début du test I2S avec DMA...\r\n");

	    /* Activer le MCLK pour le codec */
	 __HAL_SAI_ENABLE(&hsai_BlockA2);

	/* Démarrer la transmission DMA */
	// if (HAL_SAI_Transmit_DMA(&hsai_BlockA2, tx_buffer, BUFFER_SIZE) != HAL_OK) {
	//      printf("Erreur : Impossible de démarrer la transmission I2S\r\n");
	//    Error_Handler();
	//}

	//printf("Transmission I2S démarrée avec succès\r\n");

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
		//visualize_audio_volume(adc_buffer, BUFFER_SIZE);  // Calculer et afficher le volume sonore
		HAL_Delay(100);  // Mise à jour toutes les 100 ms
	/*	Driver_LED_SetLEDA(&led_driver, 0x00);  // Allumer toutes les LEDs
		HAL_Delay(100);
		Driver_LED_SetLEDA(&led_driver, 0xFF);  // Éteindre toutes les LEDs
		HAL_Delay(100);
	}*/

	/* USER CODE END WHILE */

	/* USER CODE BEGIN 3 */

	/* USER CODE END 3 */
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

/**
 * @brief Peripherals Common Clock Configuration
 * @retval None
 */
void PeriphCommonClock_Config(void)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the peripherals clock
	 */
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SAI2|RCC_PERIPHCLK_ADC;
	PeriphClkInit.Sai2ClockSelection = RCC_SAI2CLKSOURCE_PLLSAI1;
	PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
	PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSI;
	PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
	PeriphClkInit.PLLSAI1.PLLSAI1N = 13;
	PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV17;
	PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK|RCC_PLLSAI1_ADC1CLK;
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
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
```
