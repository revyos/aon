/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     analysis_wav.c
 * @brief    write_wav_init
 * @version  V1.0
 * @date     14. Jun 2019
 ******************************************************************************/

#include "drv/eflash.h"
#include "stdio.h"
struct wav_format {
    uint32_t ChunkID;       /* "RIFF" */
    uint32_t ChunkSize;     /* 36 + Subchunk2Size */
    uint32_t Format;        /* "WAVE" */
    /* sub-chunk "fmt" */
    uint32_t Subchunk1ID;   /* "fmt " */
    uint32_t Subchunk1Size; /* 16 for PCM */
    uint16_t AudioFormat;   /* PCM = 1*/
    uint16_t NumChannels;   /* Mono = 1, Stereo = 2, etc. */
    uint32_t SampleRate;    /* 8000, 44100, etc. */
    uint32_t ByteRate;      /* = SampleRate * NumChannels * BitsPerSample/8 */
    uint16_t BlockAlign;    /* = NumChannels * BitsPerSample/8 */
    uint16_t BitsPerSample; /* 8bits, 16bits, etc. */
    /* sub-chunk "data" */
    uint32_t Subchunk2ID;   /* "data" */
    uint32_t Subchunk2Size; /* data size */
};
static eflash_handle_t eflash_handle = NULL;
static uint32_t wav_date_baseaddr;

/* addr form data text for wav file */
int32_t write_wav_data(uint32_t addr, const void *data, uint32_t cnt)
{
    int32_t ret = csi_eflash_program(eflash_handle, addr, (uint8_t *)data, cnt);

    if (ret < 0) {
        printf("program error ret is %x the addr is %x \r\n", ret, addr);
        return -1;
    }

    return 0;
}

int32_t write_wav_init(void)
{
    eflash_handle = csi_eflash_initialize(0, NULL);

    if (eflash_handle == NULL) {
        printf("csi_eflash_initialize error\n");
        return -1;
    }

    eflash_info_t *info = csi_eflash_get_info(eflash_handle);
    uint32_t sector_num = 100 * 1024 / info->sector_size;
    int32_t ret;
    int j;
    uint32_t addr = 0x10010000;

    for (j = 0; j < sector_num; j++) {
        ret = csi_eflash_erase_sector(eflash_handle, addr);
        //printf("sector %d j %daddr 0x%x\n", sector_num, j, addr);

        if (ret != 0) {
            printf("the ret is %x the addr is %x \r\n", ret, addr);
            return -1;
        }

        addr += info->sector_size;
    }

    return 0;
}

/* addr form data text for wav file */
int32_t read_wav_data(uint32_t addr, void *data, uint32_t cnt)
{
    csi_eflash_read(eflash_handle, wav_date_baseaddr + addr, data, cnt);
    return 0;
}

int32_t read_wav_init(uint32_t wav_buf, int32_t *samplerate, int32_t *channle, int32_t *samplewidth, int32_t *wav_size)
{
    struct wav_format format;

    eflash_handle = csi_eflash_initialize(0, NULL);

    if (eflash_handle == NULL) {
        printf("csi_eflash_initialize error\n");
        return -1;
    }

    csi_eflash_read(eflash_handle, wav_buf, &format, sizeof(format));

    *samplerate     = format.SampleRate;
    *channle        = format.NumChannels;
    *samplewidth    = format.BitsPerSample;
    *wav_size       = format.Subchunk2Size;

    wav_date_baseaddr = wav_buf + sizeof(format);
    return 0;
}
