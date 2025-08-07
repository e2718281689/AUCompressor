//
// Created by maple on 25-8-4.
//
#include "cCompressor.h"
#include "juce_core/zip/zlib/deflate.h"
#include "math.h"

float audio_temp0[1024] = {0};
float audio_temp1[1024] = {0};
float audio_temp2[1024] = {0};
float audio_temp3[1024] = {0};
// 能量测算
void calcAudioLevel_dB(CompressorUnit *Unit, float *wav_block, float *level_dB_block, uint32_t n )
{

    float wav_pow_block = 0;
    float c1 = Unit->level_lp_const, c2 = 1.0f - c1; //prepare constants

    for(size_t i = 0; i < n ;i++)
    {
        //平方 得到功率值
        wav_pow_block = wav_block[i] * wav_block[i];

        // 平滑处理
        wav_pow_block = c1 * Unit->prev_level_lp_pow + c2 * wav_pow_block;
        Unit->prev_level_lp_pow = wav_pow_block;

        if (Unit->prev_level_lp_pow < (1.0E-13))
            Unit->prev_level_lp_pow = 1.0E-13;


        //转db 因为是功率的db所以/10
        level_dB_block[i] = log10(wav_pow_block)*10.0f;

    }

}


void calc_Instantaneous_Target_Gain(CompressorUnit *Unit, float *audio_level_dB_block, float *inst_targ_gain_dB_block,  uint32_t n )
{
    for(size_t i = 0; i < n ;i++)
    {
        //计算需要压缩的量
        inst_targ_gain_dB_block[i] =  ( audio_level_dB_block[i] - Unit->thresh_dBFS ) * (1.0f / Unit->comp_ratio - 1);

        //选择出需要压缩的
        if(inst_targ_gain_dB_block[i] > 0)
            inst_targ_gain_dB_block[i] = 0;
    }

}
void calcSmoothedGain_dB(CompressorUnit *Unit, float *inst_targ_gain_dB_block, float *gain_dB_block, uint32_t n )
{
    float gain_dB;
    float one_minus_attack_const = 1.0f - Unit->attack_const;
    float one_minus_release_const = 1.0f - Unit->release_const;
    for (int i = 0; i < n; i++)
    {
        gain_dB = inst_targ_gain_dB_block[i];

        //smooth the gain using the attack or release constants
        if (gain_dB < Unit->prev_gain_dB) {  //are we in the attack phase?
            gain_dB_block[i] = Unit->attack_const * Unit->prev_gain_dB + one_minus_attack_const*gain_dB;
        } else {   //or, we're in the release phase
            gain_dB_block[i] = Unit->release_const * Unit->prev_gain_dB + one_minus_release_const*gain_dB;
        }

        //save value for the next time through this loop
        Unit->prev_gain_dB = gain_dB_block[i];
    }
}

void setThresh_dBFS(CompressorUnit *Unit, float val)
{
    Unit->thresh_dBFS = val;
}

void setCompressionRatio(CompressorUnit *Unit, float cr)
{
    Unit->comp_ratio = max(0.001, cr); //limit to positive values
    // updateThresholdAndCompRatioConstants();
}

void setLevelTimeConst_sec(CompressorUnit *Unit, float t_sec, float fs_Hz) {
    const float min_t_sec = 0.002f;  //this is the minimum allowed value
    Unit->level_lp_sec = max(min_t_sec,t_sec);
    Unit->level_lp_const = expf(-1.0f / (Unit->level_lp_sec * fs_Hz)); //expf() is much faster than exp()
}

void setAttack_sec(CompressorUnit *Unit, float a, float fs_Hz)
{
    Unit->attack_sec = a;
    Unit->attack_const = exp(-1.0f / (Unit->attack_sec * fs_Hz)); //expf() is much faster than exp()

    //also update the time constant for the envelope extraction
    setLevelTimeConst_sec(Unit, min(Unit->attack_sec,Unit->release_sec) / 5.0, fs_Hz);  //make the level time-constant one-fifth the gain time constants
}
void setRelease_sec(CompressorUnit *Unit, float r, float fs_Hz) {
    Unit->release_sec = r;
    Unit->release_const = expf(-1.0f / (Unit->release_sec * fs_Hz)); //expf() is much faster than exp()

    //also update the time constant for the envelope extraction
    setLevelTimeConst_sec(Unit, min(Unit->attack_sec,Unit->release_sec) / 5.0, fs_Hz);  //make the level time-constant one-fifth the gain time constants
}

void calcGain(CompressorUnit *Unit, float *audio_level_dB_block, float *gain_block, uint32_t n )
{
    float *inst_targ_gain_dB_block = audio_temp2;
    //计算所需的压缩量
    calc_Instantaneous_Target_Gain(Unit, audio_level_dB_block, inst_targ_gain_dB_block, n);

    float *gain_dB_block  = audio_temp3;
    //平滑处理,加入启动时间和攻击时间
    calcSmoothedGain_dB(Unit, inst_targ_gain_dB_block, gain_dB_block, n);

    for(size_t i = 0; i < n ;i++)
    {
        //db转倍数
        gain_block[i] = pow(10.0f, gain_dB_block[i] * (1.0f/20.0f));
    }

    memset(audio_temp2,0,sizeof(audio_temp2));
    memset(audio_temp3,0,sizeof(audio_temp3));

}

void Compressor_apply(CompressorUnit *Unit, float *in , float *out, uint32_t n)
{
    if (Unit->enable == 1)
    {

        float *audio_level_dB_block = audio_temp0;
        //测试能量值
        calcAudioLevel_dB(Unit, in,  audio_level_dB_block, n);

        float *gain_block = audio_temp1;
        //得到施加的增益大小
        calcGain(Unit, audio_level_dB_block,  gain_block, n);

        //施加增益
        for(size_t i = 0; i < n ;i++)
        {
            out[i] = gain_block[i] * in[i];
        }

        memset(audio_temp0,0,sizeof(audio_temp1));
        memset(audio_temp1,0,sizeof(audio_temp0));

    }
}
void Compressor_init(CompressorUnit *Unit, uint8_t channel, uint32_t sampleRate)
{
    if (Unit->enable == 1)
    {

        Unit->channel     = channel;
        Unit->sample_rate = sampleRate;

        memset(audio_temp0,0,sizeof(audio_temp0));
        memset(audio_temp1,0,sizeof(audio_temp1));
        memset(audio_temp2,0,sizeof(audio_temp2));
        memset(audio_temp3,0,sizeof(audio_temp3));

        setThresh_dBFS(Unit, Unit->threshold);     //set the default value for the threshold for compression
        setCompressionRatio(Unit, Unit->ratio);  //set the default copression ratio
        setAttack_sec(Unit, Unit->attack, Unit->sample_rate);  //default to this value
        setRelease_sec(Unit, Unit->release, Unit->sample_rate); //default to this value

    }
}

