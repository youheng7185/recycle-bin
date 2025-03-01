//
// Created by lapchong on 2/28/25.
//
#include "my_hal_i2c.h"
#include "stm32h7xx_hal_i2c.h"
#include "main.h"
#include "i2c.h"
#include <stdint.h>
#include <string.h>
#include "my_print.h"
#include <stdio.h>

extern I2C_HandleTypeDef hi2c1;

uint8_t i2c_addr = 0;

void i2c_set_slave_address(uint8_t addr) {
    i2c_addr = addr;
}

bool i2c_read_addr8_data8(uint8_t addr, uint8_t *data) {
    if (HAL_I2C_Master_Transmit(&hi2c1, i2c_addr << 1, &addr, 1, HAL_MAX_DELAY) != HAL_OK) {
        my_printf("[I2C] ERROR: Transmit failed i2c_read_addr8_data8 (register 0x%X)\r\n", addr);
        return false;
    }
    if (HAL_I2C_Master_Receive(&hi2c1, i2c_addr << 1, data, 1, HAL_MAX_DELAY) != HAL_OK) {
        my_printf("[I2C] ERROR: Receive failed i2c_read_addr8_data8 (register 0x%X)\r\n", addr);
        return false;
    }
    return true;
}

bool i2c_read_addr8_data16(uint8_t addr, uint16_t *data) {
    if (HAL_I2C_Master_Transmit(&hi2c1, i2c_addr << 1, &addr, 1, HAL_MAX_DELAY) != HAL_OK) {
        my_printf("[I2C] ERROR: Transmit failed i2c_read_addr8_data16 (register 0x%X)\r\n", addr);
        return false;
    }
    uint8_t buf[2];
    if (HAL_I2C_Master_Receive(&hi2c1, i2c_addr << 1, buf, 2, HAL_MAX_DELAY) != HAL_OK) {
        my_printf("[I2C] ERROR: Receive failed i2c_read_addr8_data16 (register 0x%X)\r\n", addr);
        return false;
    }
    *data = (buf[0] << 8) | buf[1];
    return true;
}

bool i2c_write_addr8_data8(uint8_t addr, uint8_t data) {
    uint8_t buf[2] = {addr, data};
    if (HAL_I2C_Master_Transmit(&hi2c1, i2c_addr << 1, buf, 2, HAL_MAX_DELAY) != HAL_OK) {
        my_printf("[I2C] ERROR: Write failed i2c_write_addr8_data8 (register 0x%X) on device 0x%X\r\n", addr, i2c_addr);
        return false;
    }
    return true;
}

bool i2c_read_addr8_data32(uint8_t addr, uint32_t *data) {
    if (HAL_I2C_Master_Transmit(&hi2c1, i2c_addr << 1, &addr, 1, HAL_MAX_DELAY) != HAL_OK) {
        my_printf("[I2C] ERROR: Transmit failed i2c_read_addr8_data32 (register 0x%X)\r\n", addr);
        return false;
    }
    uint8_t buf[4];
    if (HAL_I2C_Master_Receive(&hi2c1, i2c_addr << 1, buf, 4, HAL_MAX_DELAY) != HAL_OK) {
        my_printf("[I2C] ERROR: Receive failed i2c_read_addr8_data32 (register 0x%X)\r\n", addr);
        return false;
    }
    *data = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
    return true;
}

bool i2c_write_addr8_bytes(uint8_t addr, const uint8_t *data, uint8_t length) {
    uint8_t buf[1 + length];
    buf[0] = addr;
    memcpy(&buf[1], data, length);

    if (HAL_I2C_Master_Transmit(&hi2c1, i2c_addr << 1, buf, length + 1, HAL_MAX_DELAY) != HAL_OK) {
        my_printf("[I2C] ERROR: Write failed i2c_write_addr8_bytes (register 0x%X)\r\n", addr);
        return false;
    }
    return true;
}

bool i2c_read_addr8_bytes(uint8_t addr, uint8_t *data, uint8_t length) {
    if (HAL_I2C_Master_Transmit(&hi2c1, i2c_addr << 1, &addr, 1, HAL_MAX_DELAY) != HAL_OK) {
        my_printf("[I2C] ERROR: Transmit failed i2c_read_addr8_bytes (register 0x%X)\r\n", addr);
        return false;
    }
    if (HAL_I2C_Master_Receive(&hi2c1, i2c_addr << 1, data, length, HAL_MAX_DELAY) != HAL_OK) {
        my_printf("[I2C] ERROR: Receive failed i2c_read_addr8_bytes (register 0x%X)\r\n", addr);
        return false;
    }
    return true;
}
