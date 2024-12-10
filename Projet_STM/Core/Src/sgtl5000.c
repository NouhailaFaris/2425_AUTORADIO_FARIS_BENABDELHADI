/*
 * sgtl5000.c
 *
 *  Created on: Dec 10, 2024
 *      Author: Mohamed
 */

#include "sgtl5000.h"
#include <stdio.h>

// Fonction pour écrire dans un registre
HAL_StatusTypeDef SGTL5000_WriteRegister(I2C_HandleTypeDef *hi2c, uint16_t reg, uint16_t value) {
    uint8_t data[2] = {
        (value >> 8) & 0xFF, // MSB de la valeur
        value & 0xFF         // LSB de la valeur
    };

    // Utilisation de HAL_I2C_Mem_Write
    return HAL_I2C_Mem_Write(hi2c, SGTL5000_I2C_ADDR, reg, I2C_MEMADD_SIZE_16BIT, data, 2, HAL_MAX_DELAY);
}

// Fonction pour lire un registre
HAL_StatusTypeDef SGTL5000_ReadRegister(I2C_HandleTypeDef *hi2c, uint16_t reg, uint16_t *value) {
    uint8_t data[2] = { 0 };

    // Utilisation de HAL_I2C_Mem_Read
    if (HAL_I2C_Mem_Read(hi2c, SGTL5000_I2C_ADDR, reg, I2C_MEMADD_SIZE_16BIT, data, 2, HAL_MAX_DELAY) != HAL_OK) {
        return HAL_ERROR;
    }

    // Combiner les octets pour former une valeur 16 bits
    *value = (data[0] << 8) | data[1];
    return HAL_OK;
}

// Fonction d'initialisation
HAL_StatusTypeDef SGTL5000_Init(I2C_HandleTypeDef *hi2c) {
    HAL_StatusTypeDef status = HAL_OK;

    // Initialisation des registres
    status |= SGTL5000_WriteRegister(hi2c, CHIP_ANA_POWER, 0x6AFF);
    status |= SGTL5000_WriteRegister(hi2c, CHIP_LINREG_CTRL, 0x006C);
    status |= SGTL5000_WriteRegister(hi2c, CHIP_REF_CTRL,  0x004E);
    status |= SGTL5000_WriteRegister(hi2c, CHIP_LINE_OUT_CTRL, 0x0322);
    status |= SGTL5000_WriteRegister(hi2c, CHIP_SHORT_CTRL, 0x1106);
    status |= SGTL5000_WriteRegister(hi2c, CHIP_ANA_CTRL, 0x0133);
    status |= SGTL5000_WriteRegister(hi2c, CHIP_DIG_POWER, 0x0073);
    status |= SGTL5000_WriteRegister(hi2c, CHIP_LINE_OUT_VOL, 0x0505);
    status |= SGTL5000_WriteRegister(hi2c, CHIP_CLK_CTRL, 0x0002);
    status |= SGTL5000_WriteRegister(hi2c, CHIP_I2S_CTRL, 0x0001);
    status |= SGTL5000_WriteRegister(hi2c, CHIP_ADCDAC_CTRL, 0x000C);
    status |= SGTL5000_WriteRegister(hi2c, CHIP_DAC_VOL, 0x3C3C);

    return status;
}

// Fonction de test d'un registre
void Test_Register(I2C_HandleTypeDef *hi2c, uint16_t reg, uint16_t write_value) {
    uint16_t read_value = 0;

    // Écriture dans le registre
    if (SGTL5000_WriteRegister(hi2c, reg, write_value) == HAL_OK) {
        printf("Écriture réussie dans le registre 0x%04X avec la valeur 0x%04X\r\n", reg, write_value);
    } else {
        printf("Erreur lors de l'écriture dans le registre 0x%04X\r\n", reg);
        return;
    }

    // Lecture du registre
    if (SGTL5000_ReadRegister(hi2c, reg, &read_value) == HAL_OK) {
        printf("Lecture réussie du registre 0x%04X. Valeur lue : 0x%04X\r\n", reg, read_value);

        // Vérification
        if (read_value == write_value) {
            printf("Test réussi pour le registre 0x%04X. La valeur correspond.\r\n", reg);
        } else {
            printf("Erreur : La valeur lue 0x%04X ne correspond pas à la valeur écrite 0x%04X.\r\n", read_value, write_value);
        }
    } else {
        printf("Erreur lors de la lecture du registre 0x%04X\r\n", reg);
    }
}
