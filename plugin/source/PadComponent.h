#pragma once

class PadComponent final : public juce::Component
{
public:
    PadComponent(juce::AudioProcessorValueTreeState& vts) :
            xCcValParam(vts.getParameter("xccval")),
            yCcValParam(vts.getParameter("yccval")),
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
                xCcValParam->getValue() * getLocalBounds().toFloat().getWidth(),
                (1.0f - yCcValParam->getValue()) * getLocalBounds().toFloat().getHeight()
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PadComponent)
};
