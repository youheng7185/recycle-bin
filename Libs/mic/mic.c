//
// Created by lapchong on 3/3/25.
//
#include "stm32h7xx_hal.h"
#include "mic.h"
#include "fatfs.h"
#include <string.h>
#include <stdio.h>
#include "my_print.h"
#include "main.h"
#include "i2s.h"
#include "dma.h"

FATFS fatfs;
FRESULT fresult;
FIL fil;
extern SD_HandleTypeDef hsd1;

void sd_init() {
    if (BSP_SD_Init() == MSD_OK) {
        my_printf("SD card initialized successfully!\r\n");
    } else {
        my_printf("SD card initialization failed!\r\n");
    }

    fresult = f_mount(&fatfs, "", 1);
    if(fresult != FR_OK) {
        my_printf("f_mount failed, %d\r\n", fresult);
        while(1){
            HAL_Delay(1000);
        }
    }

}
