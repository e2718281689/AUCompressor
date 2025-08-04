//
// Created by maple on 25-8-4.
//

#ifndef LOUDNESSMETER_H
#define LOUDNESSMETER_H

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

class LoudnessMeter  : public juce::Component,
                       public juce::Timer
{
public:
    LoudnessMeter()
    {
        // 设置字体
        scaleFont.setHeight(10.0f); // 新增: 设置标尺字体大小
        startTimerHz(60);
    }

    ~LoudnessMeter() override
    {
        stopTimer();
    }

    void setLevel(const float newLevelDb)
    {
        levelDb.store(newLevelDb);

        if (newLevelDb > peakLevelDb.load())
        {
            peakLevelDb.store(newLevelDb);
            peakHoldCountdown = peakHoldTimeInMs;
        }

        repaint();
    }

    void paint(juce::Graphics& g) override
    {
        // === 1. 定义布局和区域 ===
        auto bounds = getLocalBounds();

        // 新增: 定义标尺区域的宽度
        const int scaleWidth = 25;

        // 新增: 将总区域分割为电平条区域和标尺区域
        auto meterBounds = bounds.removeFromLeft(bounds.getWidth() - scaleWidth);
        auto scaleBounds = bounds; // 剩下的就是标尺区域

        // === 2. 绘制电平条 (与之前类似，但使用新的meterBounds) ===
        const auto meterBoundsF = meterBounds.toFloat();

        // 绘制背景
        g.setColour(juce::Colours::black.withAlpha(0.7f));
        g.fillRoundedRectangle(meterBoundsF, 4.0f);

        // 定义dB范围
        const float minDb = -60.0f;
        const float maxDb = 6.0f;

        // 绘制电平条
        const float currentLevel = levelDb.load();
        const float levelY = juce::jmap(currentLevel, minDb, maxDb, meterBoundsF.getBottom(), meterBoundsF.getY());

        juce::Rectangle<float> meterBar(meterBoundsF.getX(), levelY, meterBoundsF.getWidth(), meterBoundsF.getBottom() - levelY);

        juce::ColourGradient gradient(juce::Colours::green, meterBoundsF.getBottomLeft(),
                                      juce::Colours::red, meterBoundsF.getTopLeft(),
                                      false);
        gradient.addColour(0.7, juce::Colours::yellow);

        g.setGradientFill(gradient);
        g.fillRect(meterBar);

        // 绘制峰值保持线
        const float currentPeakLevel = peakLevelDb.load();
        if (currentPeakLevel > minDb)
        {
            const float peakY = juce::jmap(currentPeakLevel, minDb, maxDb, meterBoundsF.getBottom(), meterBoundsF.getY());
            g.setColour(juce::Colours::white.withAlpha(0.8f));
            g.fillRect(meterBoundsF.withY(peakY).withHeight(2.0f));
        }

        // === 3. 绘制标尺 (新增部分) ===
        g.setColour(juce::Colours::white.withAlpha(0.6f));
        g.setFont(scaleFont);

        // 定义要显示的刻度点
        const float tickValues[] = { 6, 0, -6, -12, -18, -24, -36, -48, -60 };

        for (auto dbValue : tickValues)
        {
            // 计算当前dB值对应的Y坐标
            const float y = juce::jmap(dbValue, minDb, maxDb, (float)scaleBounds.getBottom(), (float)scaleBounds.getY());

            // 绘制刻度线
            // 从电平条右侧边缘开始，向右画一条短线
            g.drawLine(meterBounds.getRight(), y, scaleBounds.getRight() - 20, y, 1.0f);

            // 准备绘制文本
            juce::String text = juce::String(dbValue);

            // 计算文本宽度以使其右对齐
            const int textWidth = scaleFont.getStringWidth(text);

            // 绘制文本，使其垂直居中于刻度线
            g.drawText(text,
                       scaleBounds.getRight() - textWidth - 2, // x: 右对齐
                       y - scaleFont.getHeight() / 2.0f,       // y: 垂直居中
                       textWidth,                                // width
                       scaleFont.getHeight(),                    // height
                       juce::Justification::centredRight);
        }
    }

    void resized() override
    {
        // No specific actions needed here as paint() is fully dynamic.
    }

    void timerCallback() override
    {
        const float currentPeak = peakLevelDb.load();

        if (peakHoldCountdown <= 0)
        {
            const float fallRateDbPerSecond = 12.0f;
            const float fallAmount = fallRateDbPerSecond / getTimerInterval();
            peakLevelDb.store(std::max(currentPeak - fallAmount, -100.0f));
            repaint();
        }
        else
        {
            peakHoldCountdown -= getTimerInterval();
        }
    }

private:
    std::atomic<float> levelDb { -100.0f };
    std::atomic<float> peakLevelDb { -100.0f };

    int peakHoldTimeInMs = 500;
    int peakHoldCountdown = 0;

    juce::Font scaleFont; // 新增: 标尺字体

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoudnessMeter)
};

#endif //LOUDNESSMETER_H
