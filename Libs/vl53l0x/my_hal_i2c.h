//
// Created by lapchong on 2/28/25.
//

#ifndef MY_HAL_I2C_H
#define MY_HAL_I2C_H

#include "stm32h7xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

void i2c_set_slave_address(uint8_t addr);
bool i2c_read_addr8_data8(uint8_t addr, uint8_t *data);
bool i2c_read_addr8_data16(uint8_t addr, uint16_t *data);
bool i2c_write_addr8_data8(uint8_t addr, uint8_t data);
bool i2c_read_addr8_data32(uint8_t addr, uint32_t *data);
bool i2c_write_addr8_bytes(uint8_t addr, const uint8_t *data, uint8_t length);
bool i2c_read_addr8_bytes(uint8_t addr, uint8_t *data, uint8_t length);

#endif //MY_HAL_I2C_H
