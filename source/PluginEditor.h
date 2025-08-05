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

    juce::Slider gainSlider;
    juce::Slider rms_time_Slider;
    LoudnessMeter verticalMeter;

private:


    void timerCallback() override; // 声明回调

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processorRef;
    std::unique_ptr<melatonin::Inspector> inspector;

    SliderVts sliderAttachment{ processorRef.apvts, "gainSlider", gainSlider };
    SliderVts rms_time_slider_Attachment{ processorRef.apvts, "rms_time_Slider", rms_time_Slider };


    juce::TextButton inspectButton { "Inspect the UI" };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
