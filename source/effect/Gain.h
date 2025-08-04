//
// Created by maple on 25-5-19.
//

#ifndef GAIN_H
#define GAIN_H

#include "./ProcessorBase.h"
#include <juce_audio_processors/juce_audio_processors.h>

class Gain  : public ProcessorBase, public juce::AudioProcessorValueTreeState::Listener
{
public:
    Gain(juce::AudioProcessorValueTreeState& apvts):Apvts(apvts)
    {
        // add Listener
        Apvts.addParameterListener("gainSlider", this);
    }

    ~Gain()
    {
        // remove Listener
        Apvts.removeParameterListener("gainSlider", this);
    }

    void parameterChanged(const juce::String& parameterID, float newValue)
    {
        //  Listener parameter
        if (parameterID.equalsIgnoreCase("gainSlider"))
        {
            juce::Logger::outputDebugString("gain =" + juce::String(gain));
            gain = newValue;
        }
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        //  get parameter
        gain = Apvts.getParameterAsValue("gainSlider").getValue();
    }

    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override
    {
        //  process
        buffer.applyGain (gain);
    }

    void reset() override
    {
        gain = 0;
    }


    const juce::String getName() const override { return "Gain"; }

private:

    float  gain;
    juce::AudioProcessorValueTreeState& Apvts;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Gain)
};


#endif //GAIN_H
