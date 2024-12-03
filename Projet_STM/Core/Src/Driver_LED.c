#include "Driver_LED.h"

// Prototypes des fonctions internes
static void Driver_LED_WriteRegister(Driver_LED_HandleTypeDef *dev, uint8_t reg, uint8_t value);
static uint8_t Driver_LED_ReadRegister(Driver_LED_HandleTypeDef *dev, uint8_t reg);

// Configuration en mode BANK 1
void MCP23S17_SetBank1(Driver_LED_HandleTypeDef *dev) {
    // Lire la valeur actuelle de IOCON
    uint8_t iocon = Driver_LED_ReadRegister(dev, MCP23S17_IOCON);

    // Activer le bit BANK (position 7)
    iocon |= (1 << 7);  // Mettre le bit BANK à 1

    // Écrire la nouvelle valeur dans IOCON
    Driver_LED_WriteRegister(dev, MCP23S17_IOCON, iocon);
}

// Sélectionne le périphérique SPI
static void Driver_LED_Select(Driver_LED_HandleTypeDef *dev) {
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
}

// Désélectionne le périphérique SPI
static void Driver_LED_Deselect(Driver_LED_HandleTypeDef *dev) {
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);
}

// Écrit dans un registre du MCP23S17
static void Driver_LED_WriteRegister(Driver_LED_HandleTypeDef *dev, uint8_t reg, uint8_t value) {
    uint8_t data[3] = {MCP23S17_WRITE_OPCODE, reg, value};

    Driver_LED_Select(dev);
    HAL_SPI_Transmit(dev->hspi, data, 3, HAL_MAX_DELAY);
    HAL_Delay(1);
    Driver_LED_Deselect(dev);
}

// Lit un registre du MCP23S17
static uint8_t Driver_LED_ReadRegister(Driver_LED_HandleTypeDef *dev, uint8_t reg) {
    uint8_t txData[2] = {MCP23S17_WRITE_OPCODE | 0x01, reg};
    uint8_t rxData = 0;

    Driver_LED_Select(dev);
    HAL_SPI_Transmit(dev->hspi, txData, 2, HAL_MAX_DELAY);
    HAL_SPI_Receive(dev->hspi, &rxData, 1, HAL_MAX_DELAY);
    HAL_Delay(1);
    Driver_LED_Deselect(dev);

    return rxData;
}

// Initialisation du MCP23S17 pour piloter les LEDs
void Driver_LED_Init(Driver_LED_HandleTypeDef *dev) {
    // Configurer le mode BANK 1
    MCP23S17_SetBank1(dev);

    // Configurer PORTA et PORTB comme sorties
    Driver_LED_WriteRegister(dev, MCP23S17_IODIRA, 0x00); // PORTA en sortie
    Driver_LED_WriteRegister(dev, MCP23S17_IODIRB, 0x00); // PORTB en sortie

    // Initialiser PORTA et PORTB à 0xFF (LEDs éteintes)
    Driver_LED_WriteRegister(dev, MCP23S17_GPIOA, 0xFF);
    Driver_LED_WriteRegister(dev, MCP23S17_GPIOB, 0xFF);
}

// Allume les LEDs sur PORTA
void Driver_LED_SetLEDA(Driver_LED_HandleTypeDef *dev, uint8_t value) {
    Driver_LED_WriteRegister(dev, MCP23S17_GPIOA, value);
}

// Allume les LEDs sur PORTB
void Driver_LED_SetLEDB(Driver_LED_HandleTypeDef *dev, uint8_t value) {
    Driver_LED_WriteRegister(dev, MCP23S17_GPIOB, value);
}

// Lit l'état des LEDs sur PORTA
uint8_t Driver_LED_ReadLEDA(Driver_LED_HandleTypeDef *dev) {
    return Driver_LED_ReadRegister(dev, MCP23S17_GPIOA);
}

// Lit l'état des LEDs sur PORTB
uint8_t Driver_LED_ReadLEDB(Driver_LED_HandleTypeDef *dev) {
    return Driver_LED_ReadRegister(dev, MCP23S17_GPIOB);
}
