//
// Created by maple on 25-8-4.
//

#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "../ProcessorBase.h"
#include "./cCompressor.h"
#include <juce_audio_processors/juce_audio_processors.h>

class Compressor  : public ProcessorBase, public juce::AudioProcessorValueTreeState::Listener
{
public:
    Compressor(juce::AudioProcessorValueTreeState& apvts):Apvts(apvts)
    {

    }

    ~Compressor()
    {

    }

    void parameterChanged(const juce::String& parameterID, float newValue)
    {

    }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        Compressor_Unit.enable = 1;
        Compressor_Unit.sample_rate = sampleRate;
        Compressor_Unit.channel = 1;

        Compressor_Unit.attack =  0.5;
        Compressor_Unit.release = 0.002;
        Compressor_Unit.threshold = -6;
        Compressor_Unit.ratio = 5;

        Compressor_Unit.rms_time = Apvts.getParameterAsValue("rms_time_Slider").getValue();
        Compressor_init(&Compressor_Unit,1,sampleRate);
    }

    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override
    {
        size_t numSamples = buffer.getNumSamples();

        auto* pcm_L = buffer.getWritePointer (0);
        auto* pcm_R = buffer.getWritePointer (1);

        float *pcm = new float[2 * numSamples];
        for (size_t i = 0; i < numSamples; ++i)
        {
            pcm[i]=pcm_L[i] + pcm_R[i];
        }

        Compressor_apply(&Compressor_Unit,pcm,pcm, numSamples);

        for (size_t i = 0; i < numSamples; ++i)
        {
            pcm_L[i] = pcm[i];
            pcm_R[i] = pcm[i];
        }

        delete[] pcm;

    }

    void reset() override
    {
        rmsDb = 0;
    }

    float getRMS() const
    {
        return Compressor_Unit.rms;
    }

    const juce::String getName() const override { return "Compressor"; }

private:

    CompressorUnit Compressor_Unit = {0};

    float  rmsDb = 0;
    juce::AudioProcessorValueTreeState& Apvts;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Compressor)
};


#endif //COMPRESSOR_H
