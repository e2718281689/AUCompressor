//
// Created by maple on 25-8-5.
//

#ifndef VOLUME_H
#define VOLUME_H

#include "../ProcessorBase.h"
#include <juce_audio_processors/juce_audio_processors.h>

class VolumeProc  : public ProcessorBase, public juce::AudioProcessorValueTreeState::Listener
{
public:
    VolumeProc(juce::AudioProcessorValueTreeState& apvts):Apvts(apvts)
    {

    }

    ~VolumeProc()
    {

    }

    void parameterChanged(const juce::String& parameterID, float newValue)
    {

    }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {

    }

    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override
    {
        size_t numSamples = buffer.getNumSamples();

        float rms1 = buffer.getRMSLevel (0,1,numSamples);
        float rms2 = buffer.getRMSLevel (0,2,numSamples);

        rmsDB = juce::Decibels::gainToDecibels(rms1 > rms2? rms1 : rms2);
    }

    void reset() override
    {
        rmsDB = 0;
    }

    float getRMS() const
    {
        return rmsDB;
    }

    const juce::String getName() const override { return "VolumeProcessor"; }

private:

    float rmsDB;
    juce::AudioProcessorValueTreeState& Apvts;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VolumeProc)
};

#endif //VOLUME_H
