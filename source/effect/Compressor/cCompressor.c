//
// Created by maple on 25-8-4.
//
#include "cCompressor.h"
#include "math.h"

float date_L[1024]={0};
float date_R[1024]={0};
float rms_date[22050]={0};

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

    for (uint32_t i = 0; i < n; i++)
    {
        date_L[i] = in[ 2 * i + 0 ];
        date_R[i] = in[ 2 * i + 1 ];
    }

    circular_buffer_float_write(&Unit->cb1, date_L, n);
    circular_buffer_float_write(&Unit->cb2, date_R, n);

    float rms_time = Unit->rms_time;

    // if (circular_buffer_float_get_size(&Unit->cb1) >= rms_time && circular_buffer_float_get_size(&Unit->cb2) >= rms_time)
    // {
    //     circular_buffer_float_read(&Unit->cb1,rms_date,rms_time);
    //     float rms1 = get_rms_level(rms_date, rms_time);
    //
    //     circular_buffer_float_read(&Unit->cb2,rms_date,rms_time);
    //     float rms2 = get_rms_level(rms_date, rms_time);
    //
    //     float rms = rms1 > rms2 ? rms1 : rms2;
    //     Unit->rms = multiply_2_db(rms);
    // }

    // Unit->rms = multiply_2_db(get_rms_level(in, n));

    float count_rms_time = 0;

    count_rms_time = circular_buffer_float_peek(&Unit->cb1, rms_date,rms_time);
    float rms1 = get_rms_level(rms_date, count_rms_time);

    count_rms_time = circular_buffer_float_peek(&Unit->cb2, rms_date,rms_time);
    float rms2 = get_rms_level(rms_date, count_rms_time);

    float rms = rms1 > rms2 ? rms1 : rms2;
    Unit->rms = multiply_2_db(rms);
    Unit->rms = multiply_2_db(get_rms_level(in, n));

}
void Compressor_init(CompressorUnit *Unit, uint8_t channel, uint8_t sampleRate)
{
    if (Unit->enable == 1)
    {
        circular_buffer_float_init(&Unit->cb1, Unit->buffer_mem1, 22050);
        circular_buffer_float_init(&Unit->cb2, Unit->buffer_mem2, 22050);
    }
}

