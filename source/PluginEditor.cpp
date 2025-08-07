#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);

    // addAndMakeVisible (inspectButton);

    // this chunk of code instantiates and opens the melatonin inspector
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }

        inspector->setVisible (true);
    };

    startTimerHz(30);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);


    addAndMakeVisible(verticalMeter);
    addAndMakeVisible(verticalMeter_in);
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    auto area = getLocalBounds();
    g.setColour (juce::Colours::white);
    g.setFont (16.0f);
    auto helloWorld = juce::String ("Hello from ") + PRODUCT_NAME_WITHOUT_VERSION + " v" VERSION + " running in " + CMAKE_BUILD_TYPE;
    g.drawText (helloWorld, area.removeFromTop (100), juce::Justification::centred, false);
}

void PluginEditor::resized()
{
    // 将响度条放在窗口右侧，宽度为30像素，上下留有20像素的边距
    auto bounds = getLocalBounds();
    verticalMeter.setBounds(bounds.getRight() - 40, 20, 30, bounds.getHeight() - 40);

    bounds = getLocalBounds();
    verticalMeter_in.setBounds(20, 20, 30, bounds.getHeight() - 40);
}
void PluginEditor::timerCallback()
{
    // 从处理器获取最新的dB值并设置到响度条上
    verticalMeter.setLevel(processorRef.VolumeBase->getRMS() );
    verticalMeter_in.setLevel(processorRef.VolumeBase_in->getRMS() );
}