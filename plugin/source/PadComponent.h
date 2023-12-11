#pragma once

class PadComponent final : public juce::Component
{
public:
    PadComponent(juce::AudioProcessorValueTreeState& vts) :
        xPadParam(vts.getParameter("xpad")),
        yPadParam(vts.getParameter("ypad")),
        lockedParam(vts.getParameter("locked"))
    {
    }

    virtual ~PadComponent() override = default;

    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colours::black);
        g.fillRoundedRectangle(getLocalBounds().toFloat(), 10);

        g.setColour(juce::Colours::white);
        g.drawRoundedRectangle(getLocalBounds().toFloat(), 10, 1);

        const float cursorSize = 10;
        const juce::Point<float> cursorPos = {
                xPadParam->getValue() * getLocalBounds().toFloat().getWidth(),
                (1.0f - yPadParam->getValue()) * getLocalBounds().toFloat().getHeight()
        };

        g.setColour(juce::Colours::yellow);
        g.fillEllipse(cursorPos.x - cursorSize * 0.5f, cursorPos.y - cursorSize * 0.5f, cursorSize, cursorSize);
    }

    void mouseMove(const juce::MouseEvent &event) override
    {
        const bool isLocked = lockedParam->getValue() == 1.0f;
        if (isLocked)
        {
            return;
        }

        const float xValue = xPadParam->convertFrom0to1(event.position.x / getLocalBounds().toFloat().getWidth());
        const float yValue = yPadParam->convertFrom0to1((getLocalBounds().toFloat().getHeight() - event.position.y) / getLocalBounds().toFloat().getHeight());

        xPadParam->beginChangeGesture();
        xPadParam->setValueNotifyingHost(xPadParam->convertTo0to1(xValue));
        xPadParam->endChangeGesture();

        yPadParam->beginChangeGesture();
        yPadParam->setValueNotifyingHost(yPadParam->convertTo0to1(yValue));
        yPadParam->endChangeGesture();

        repaint();
    }

    void mouseDown(const juce::MouseEvent &event) override
    {
        lockedParam->beginChangeGesture();
        lockedParam->setValueNotifyingHost(lockedParam->getValue() == 0.0f ? 1.0f : 0.0f);
        lockedParam->endChangeGesture();
    }

private:
    juce::RangedAudioParameter* xPadParam;
    juce::RangedAudioParameter* yPadParam;
    juce::RangedAudioParameter* lockedParam;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PadComponent)
};
