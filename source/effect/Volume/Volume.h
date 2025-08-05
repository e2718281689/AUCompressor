//
// Created by maple on 25-8-4.
//

#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "../ProcessorBase.h"
#include "./cVolume.h"
#include <juce_audio_processors/juce_audio_processors.h>

class Volume  : public ProcessorBase, public juce::AudioProcessorValueTreeState::Listener
{
public:
    Volume(juce::AudioProcessorValueTreeState& apvts):Apvts(apvts)
    {
        Apvts.addParameterListener("rms_time_Slider", this);
    }

    ~Volume()
    {
        Apvts.removeParameterListener("rms_time_Slider", this);
    }

    void parameterChanged(const juce::String& parameterID, float newValue)
    {
        //  Listener parameter
        if (parameterID.equalsIgnoreCase("rms_time_Slider"))
        {
            Compressor_Unit.rms_time = newValue;
        }
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        Compressor_Unit.enable = 1;
        Compressor_Unit.rms_time = Apvts.getParameterAsValue("rms_time_Slider").getValue();
        Compressor_init(&Compressor_Unit,2,sampleRate);
    }

    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override
    {
        size_t numSamples = buffer.getNumSamples();

        auto* pcm_L = buffer.getWritePointer (0);
        auto* pcm_R = buffer.getWritePointer (1);

        float *pcm = new float[2 * numSamples];
        for (size_t i = 0; i < numSamples; ++i)
        {
            pcm[2 * i + 1 ]=pcm_L[i];
            pcm[2 * i + 0 ]=pcm_R[i];
        }

        Compressor_apply(&Compressor_Unit,pcm,pcm, numSamples);

        for (size_t i = 0; i < numSamples; ++i)
        {
            pcm_L[i] = pcm[2 * i + 1 ];
            pcm_R[i] = pcm[2 * i + 0 ];
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

    CompressorUnit Compressor_Unit;

    float  rmsDb = 0;
    juce::AudioProcessorValueTreeState& Apvts;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Volume)
};


#endif //COMPRESSOR_H
