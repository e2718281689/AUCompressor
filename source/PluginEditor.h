#pragma once

#include "PluginProcessor.h"
#include "BinaryData.h"
#include "melatonin_inspector/melatonin_inspector.h"
#include "./GUI/LoudnessMeter.h"

using SliderVts = juce::AudioProcessorValueTreeState::SliderAttachment;

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor
                    ,public juce::Timer // 继承Timer
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    LoudnessMeter verticalMeter;
    LoudnessMeter verticalMeter_in;

private:


    void timerCallback() override; // 声明回调

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processorRef;
    std::unique_ptr<melatonin::Inspector> inspector;

    juce::TextButton inspectButton { "Inspect the UI" };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
