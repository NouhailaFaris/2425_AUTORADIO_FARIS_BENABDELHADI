/*
 * Driver_LED.h
 *
 *  Created on: Dec 3, 2024
 *      Author: Mohamed
 */

#ifndef DRIVER_LED_H
#define DRIVER_LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"  // Inclut la configuration correcte de HAL


/* Macros pour les registres du MCP23S17 */
#define MCP23S17_WRITE_OPCODE  0x40
#define MCP23S17_IODIRA        0x00
#define MCP23S17_IODIRB        0x10
#define MCP23S17_GPIOA         0x09
#define MCP23S17_GPIOB         0x19
#define MCP23S17_IOCON 0x0A
/* Structure pour piloter les LEDs */
typedef struct {
    SPI_HandleTypeDef *hspi;  // Handle SPI utilisé
    GPIO_TypeDef *cs_port;    // Port GPIO pour CS
    uint16_t cs_pin;          // Pin GPIO pour CS
} Driver_LED_HandleTypeDef;

/* Prototypes des fonctions */
void Driver_LED_Init(Driver_LED_HandleTypeDef *dev);
void Driver_LED_SetLEDA(Driver_LED_HandleTypeDef *dev, uint8_t value);
void Driver_LED_SetLEDB(Driver_LED_HandleTypeDef *dev, uint8_t value);
uint8_t Driver_LED_ReadLEDA(Driver_LED_HandleTypeDef *dev);
uint8_t Driver_LED_ReadLEDB(Driver_LED_HandleTypeDef *dev);

#ifdef __cplusplus
}
#endif

#endif /* DRIVER_LED_H */
