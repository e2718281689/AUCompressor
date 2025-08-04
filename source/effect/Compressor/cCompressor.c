//
// Created by maple on 25-8-4.
//
#include "cCompressor.h"
#include "math.h"

float get_rms_level(float *in, uint32_t len)
{
    uint32_t i = 0;
    float rms = 0.0f;
    float sum_squares = 0.0f;
    for (i = 0; i < len; i++)
    {
        sum_squares+= (in[i] * in[i]);
    }

    rms = sqrt(sum_squares/len);
    return rms;
}

float multiply_2_db(float multiply)
{
    return 20 * log10(multiply);
}
void Compressor_apply(CompressorUnit *Unit, float *in , float *out, uint32_t n)
{
    Unit->rms = multiply_2_db(get_rms_level(in, n));


}

