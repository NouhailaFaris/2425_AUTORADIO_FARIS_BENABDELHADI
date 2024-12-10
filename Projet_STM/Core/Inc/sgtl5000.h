/*
 * sgtl5000.h
 *
 *  Created on: Dec 10, 2024
 *      Author: Mohamed
 */

#ifndef INC_SGTL5000_H_
#define INC_SGTL5000_H_

#include "i2c.h"

// Adresse I2C du SGTL5000
#define SGTL5000_I2C_ADDR 0x14 // Adresse 0x1A en décimal

// Adresses des registres
#define CHIP_ANA_POWER     0x0030
#define CHIP_LINREG_CTRL   0x0026
#define CHIP_REF_CTRL      0x0028
#define CHIP_LINE_OUT_CTRL 0x002C
#define CHIP_SHORT_CTRL    0x003C
#define CHIP_ANA_CTRL      0x0024
#define CHIP_DIG_POWER     0x0002
#define CHIP_LINE_OUT_VOL  0x002E
#define CHIP_CLK_CTRL      0x0004
#define CHIP_I2S_CTRL      0x0006
#define CHIP_ADCDAC_CTRL   0x000E
#define CHIP_DAC_VOL       0x0010
void Test_Register(I2C_HandleTypeDef *hi2c, uint16_t reg, uint16_t write_value);
// Prototypes
HAL_StatusTypeDef SGTL5000_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef SGTL5000_WriteRegister(I2C_HandleTypeDef *hi2c, uint16_t reg, uint16_t value);
HAL_StatusTypeDef SGTL5000_ReadRegister(I2C_HandleTypeDef *hi2c, uint16_t reg, uint16_t *value);

#endif /* INC_SGTL5000_H_ */
