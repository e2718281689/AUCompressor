//
// Created by maple on 25-8-4.
//

#ifndef CCOMPRESSOR_H
#define CCOMPRESSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "circular_buffer.h"

typedef struct _Compressortext
{
    int xxx;
}Compressortext;

typedef struct _CompressorUnit
{

    Compressortext      *ct;
    uint8_t              channel;
    uint32_t 			 sample_rate;
    uint32_t      		 enable;

    float                rms;
    float                rms_time;

    circular_buffer_float_t cb1;
    circular_buffer_float_t cb2;

    float               level_lp_const;
    float               prev_level_lp_pow;
    float               thresh_dBFS;
    float               comp_ratio;
    float               attack_const;
    float               release_const;
    float               prev_gain_dB;

    float               attack_sec;
    float               release_sec;
    float               level_lp_sec;


    float               attack;
    float               release;
    float               threshold;
    float               ratio;

    float buffer_mem1[22050];
    float buffer_mem2[22050];

}CompressorUnit;

void Compressor_apply(CompressorUnit *Unit, float *in , float *out, uint32_t n);
void Compressor_init(CompressorUnit *Unit, uint8_t channel, uint32_t sampleRate);
#ifdef __cplusplus
}
#endif

#endif //CCOMPRESSOR_H
