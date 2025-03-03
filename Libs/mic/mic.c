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
#include <stdint.h>

FATFS fatfs;
FRESULT fresult;
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

const uint16_t SAMPLE_RATE = 44100;
const uint16_t BITS_PER_SAMPLE = 16;
const uint16_t CHANNELS = 1;
const uint16_t SAMPLE_SIZE = (SAMPLE_RATE * 10); // assume this record for 10 secomds

#define I2S_BUFFER_SIZE 512
int16_t i2s_rx_buffer[I2S_BUFFER_SIZE];
uint32_t total_samples = 0;
FIL audioFile;
uint8_t recording = 0;

extern I2S_HandleTypeDef hi2s1;

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s);
void start_recording();
void stop_recording();
void write_wav_header(FIL *file);
void append_pcm_data(FIL *file, int16_t *data, uint32_t size);

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s) {
  if(recording) {
    append_pcm_data(&audioFile, i2s_rx_buffer, I2S_BUFFER_SIZE);
    total_samples += I2S_BUFFER_SIZE / 2;
    if (total_samples >= SAMPLE_SIZE) {
        stop_recording();
    }
  }
}

void start_recording() {
  if (f_open(&audioFile, "recording.wav", FA_READ | FA_CREATE_ALWAYS) == FR_OK) {
    write_wav_header(&audioFile);
    recording = 1;
    total_samples = 0;
    HAL_I2S_Receive_DMA(&hi2s1, (uint16_t *)i2s_rx_buffer, I2S_BUFFER_SIZE);
  }
}

void stop_recording() {
  recording = 0;
  HAL_I2S_DMAStop(&hi2s1);

  // Update WAV file header with correct file size
  f_lseek(&audioFile, 4);
  uint32_t fileSize = (total_samples * 2) + 36;
  f_write(&audioFile, &fileSize, 4, NULL);

  f_lseek(&audioFile, 40);
  uint32_t dataSize = total_samples * 2;
  f_write(&audioFile, &dataSize, 4, NULL);

  f_close(&audioFile);
}

void write_wav_header(FIL *file) {
  uint8_t header[44];
  uint32_t fileSize = 36 + SAMPLE_SIZE * 2;
  uint32_t byteRate = SAMPLE_RATE * CHANNELS * (BITS_PER_SAMPLE / 8);
  uint16_t blockAlign = CHANNELS * (BITS_PER_SAMPLE / 8);

  memcpy(header, "RIFF", 4);
  memcpy(header + 4, &fileSize, 4);
  memcpy(header + 8, "WAVEfmt ", 8);

  uint32_t subChunk1Size = 16;
  uint16_t audioFormat = 1;
  memcpy(header + 16, &subChunk1Size, 4);
  memcpy(header + 20, &audioFormat, 2);
  memcpy(header + 22, &CHANNELS, 2);
  memcpy(header + 24, &SAMPLE_RATE, 4);
  memcpy(header + 28, &byteRate, 4);
  memcpy(header + 32, &blockAlign, 2);
  memcpy(header + 34, &BITS_PER_SAMPLE, 2);
  memcpy(header + 36, "data", 4);

  uint32_t dataSize = SAMPLE_SIZE * 2;
  memcpy(header + 40, &dataSize, 4);

  f_write(file, header, 44, NULL);
}

void append_pcm_data(FIL *file, int16_t *data, uint32_t size) {
  UINT bytesWritten;
  f_write(file, data, size * 2, &bytesWritten);
}
