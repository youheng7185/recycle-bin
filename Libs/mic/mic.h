//
// Created by lapchong on 3/3/25.
//

#ifndef MIC_H
#define MIC_H

#include "fatfs.h"

void sd_init();
int get_next_audio_filename();
void list_directory(const char *path, uint8_t depth);
void write_wav_header(FIL *file);


#endif //MIC_H
