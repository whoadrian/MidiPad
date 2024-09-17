#pragma once

#include "juce_gui_extra/juce_gui_extra.h"

class PadComponent final : public juce::AnimatedAppComponent
{
public:
    PadComponent(juce::AudioProcessorValueTreeState& vts) :
            xCcValParam(vts.getParameter("xccval")),
            yCcValParam(vts.getParameter("yccval")),
            lockedParam(vts.getParameter("locked"))
    {
        setFramesPerSecond(60);

        for(auto & i : trail)
        {
            i = {{0, 0}, 0};
        }
    }

    virtual ~PadComponent() override = default;

    void update() override
    {
        auto dT = getMillisecondsSinceLastUpdate();

        if (cursorPos != lastCursorPos)
        {
            trailIndex++;
            if (trailIndex == trailCount)
                trailIndex = 0;
            trail[trailIndex].pos = lastCursorPos;
            trail[trailIndex].life = 1.0f;
            lastCursorPos = cursorPos;
        }

        for (auto& t : trail)
        {
            t.life -= 0.02f;
            if (t.life < 0)
                t.life = 0;
        }
    }

    void paint(juce::Graphics& g) override
    {
        cursorPos = {
                xCcValParam->getValue() * getLocalBounds().toFloat().getWidth(),
                (1.0f - yCcValParam->getValue()) * getLocalBounds().toFloat().getHeight()
        };
        const float cursorSize = 16;

        // Background
        auto gradient = juce::ColourGradient(
                juce::Colours::black, getLocalBounds().getCentre().toFloat(),
                juce::Colours::white.withBrightness(0.12f), getLocalBounds().getBottomLeft().toFloat(),
                true);
        g.setGradientFill(gradient);
        g.fillRect(getLocalBounds().toFloat());

        // Lines
        g.setColour(juce::Colours::white.withAlpha(0.1f));
        g.drawHorizontalLine((int)cursorPos.y, 0, getLocalBounds().toFloat().getWidth());
        g.drawVerticalLine((int)cursorPos.x, 0, getLocalBounds().toFloat().getHeight());

        // Locked text
        const bool isLocked = lockedParam->getValue() == 1.0f;
        if (isLocked)
        {
            g.setColour(juce::Colours::white.withAlpha(0.15f));
            g.setFont(40);
            g.drawText("LOCKED", getLocalBounds(), juce::Justification::centred);
        }

        // Trail
        for (auto t : trail)
        {
            if (t.life <= 0.0f)
                continue;

            g.setColour(juce::Colours::yellow.withBrightness(0.6f).interpolatedWith(juce::Colours::black.withAlpha(.0f), 1.0f - t.life * t.life));
            const float trailCursorSize = cursorSize * t.life;
            g.fillEllipse(t.pos.x - trailCursorSize * 0.5f, t.pos.y - trailCursorSize * 0.5f, trailCursorSize, trailCursorSize);
        }

        // Cursor
        g.setColour(juce::Colours::yellow);
        g.fillEllipse(cursorPos.x - cursorSize * 0.5f, cursorPos.y - cursorSize * 0.5f, cursorSize, cursorSize);

        // Border
        g.setColour(juce::Colours::white.withBrightness(0.5f));
        g.drawRect(getLocalBounds().toFloat(), 2);
    }

    void mouseMove(const juce::MouseEvent &event) override
    {
        const bool isLocked = lockedParam->getValue() == 1.0f;
        if (isLocked)
        {
            return;
        }

        const float xValue = xCcValParam->convertFrom0to1(event.position.x / getLocalBounds().toFloat().getWidth());
        const float yValue = yCcValParam->convertFrom0to1((getLocalBounds().toFloat().getHeight() - event.position.y) / getLocalBounds().toFloat().getHeight());

        xCcValParam->beginChangeGesture();
        xCcValParam->setValueNotifyingHost(xCcValParam->convertTo0to1(xValue));
        xCcValParam->endChangeGesture();

        yCcValParam->beginChangeGesture();
        yCcValParam->setValueNotifyingHost(yCcValParam->convertTo0to1(yValue));
        yCcValParam->endChangeGesture();

        repaint();
    }

    void mouseDown(const juce::MouseEvent &event) override
    {
        lockedParam->beginChangeGesture();
        lockedParam->setValueNotifyingHost(lockedParam->getValue() == 0.0f ? 1.0f : 0.0f);
        lockedParam->endChangeGesture();
    }

private:
    juce::RangedAudioParameter* xCcValParam;
    juce::RangedAudioParameter* yCcValParam;
    juce::RangedAudioParameter* lockedParam;

    juce::Point<float> cursorPos;
    juce::Point<float> lastCursorPos;

    struct TrailPoint
    {
        juce::Point<float> pos;
        float life;
    };
    static constexpr const int trailCount = 80;
    int trailIndex = 0;
    TrailPoint trail[trailCount];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PadComponent)
};
