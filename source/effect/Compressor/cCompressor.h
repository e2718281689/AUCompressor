//
// Created by maple on 25-8-4.
//

#ifndef CCOMPRESSOR_H
#define CCOMPRESSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
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
}CompressorUnit;

void Compressor_apply(CompressorUnit *Unit, float *in , float *out, uint32_t n);

#ifdef __cplusplus
}
#endif

#endif //CCOMPRESSOR_H
