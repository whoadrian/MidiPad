#pragma once

class MenuComponent final : public juce::Component
{
public:
    MenuComponent(juce::AudioProcessorValueTreeState& vts) :
        xCcParam(vts.getParameter("xcc")),
        yCcParam(vts.getParameter("ycc")),
        xPadParam(vts.getParameter("xpad")),
        yPadParam(vts.getParameter("ypad")),
        lockParam(vts.getParameter("locked"))
    {
        for (int xValue = (int)xCcParam->getNormalisableRange().start; xValue <= xCcParam->getNormalisableRange().end; ++xValue)
            xCcBox.addItem(std::to_string(xValue), xValue + 1);

        for (int yValue = (int)yCcParam->getNormalisableRange().start; yValue <= yCcParam->getNormalisableRange().end; ++yValue)
            yCcBox.addItem(std::to_string(yValue), yValue + 1);

        xCcBox.setSelectedId((int)xCcParam->convertFrom0to1(xCcParam->getValue()) + 1, juce::dontSendNotification);
        yCcBox.setSelectedId((int)yCcParam->convertFrom0to1(yCcParam->getValue()) + 1, juce::dontSendNotification);

        xCcBox.onChange = [this] {onXccChange();};
        yCcBox.onChange = [this] {onYccChange();};

        addAndMakeVisible(yCcBox);
        addAndMakeVisible(xCcBox);

        xLabel.setText("X :", juce::dontSendNotification);
        xLabel.setEditable(false, false);
        xLabel.setJustificationType(juce::Justification::centredLeft);
        addAndMakeVisible(xLabel);

        yLabel.setText(": Y", juce::dontSendNotification);
        yLabel.setEditable(false, false);
        yLabel.setJustificationType(juce::Justification::centredRight);
        addAndMakeVisible(yLabel);

        xValueLabel.setEditable(false, false);
        xValueLabel.setJustificationType(juce::Justification::centredLeft);
        addAndMakeVisible(xValueLabel);

        yValueLabel.setEditable(false, false);
        yValueLabel.setJustificationType(juce::Justification::centredRight);
        addAndMakeVisible(yValueLabel);

        lockLabel.setEditable(false, false);
        lockLabel.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(lockLabel);

        clickLabel.setEditable(false, false);
        clickLabel.setJustificationType(juce::Justification::centred);
        clickLabel.setText("(click)", juce::dontSendNotification);
        clickLabel.setFont(10);
        addAndMakeVisible(clickLabel);
    }

    virtual ~MenuComponent() override = default;

    void paint(juce::Graphics &g) override
    {
        xValueLabel.setText(std::to_string((int)xPadParam->convertFrom0to1(xPadParam->getValue())), juce::dontSendNotification);
        yValueLabel.setText(std::to_string((int)yPadParam->convertFrom0to1(yPadParam->getValue())), juce::dontSendNotification);

        bool isLocked = lockParam->getValue() == 1.0f;
        lockLabel.setText(isLocked ? "LOCKED" : "UNLOCKED", juce::dontSendNotification);

        repaint();
    }

    void resized() override
    {
        auto bounds = getLocalBounds();
        bounds.reduce(0, 2);

        xLabel.setBounds(bounds.removeFromLeft(25));
        yLabel.setBounds(bounds.removeFromRight(25));

        xValueLabel.setBounds(bounds.removeFromLeft(30));
        yValueLabel.setBounds(bounds.removeFromRight(30));

        xCcBox.setBounds(bounds.removeFromLeft(50).reduced(0, 3));
        yCcBox.setBounds(bounds.removeFromRight(50).reduced(0, 3));

        lockLabel.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.8f));
        clickLabel.setBounds(bounds);
    }

private:
    void onXccChange()
    {
        xCcParam->beginChangeGesture();
        xCcParam->setValueNotifyingHost(xCcParam->convertTo0to1((float)xCcBox.getSelectedId() - 1));
        xCcParam->endChangeGesture();
    }

    void onYccChange()
    {
        yCcParam->beginChangeGesture();
        yCcParam->setValueNotifyingHost(yCcParam->convertTo0to1((float)yCcBox.getSelectedId() - 1));
        yCcParam->endChangeGesture();
    }

    juce::RangedAudioParameter* xCcParam;
    juce::RangedAudioParameter* yCcParam;
    juce::RangedAudioParameter* xPadParam;
    juce::RangedAudioParameter* yPadParam;
    juce::RangedAudioParameter* lockParam;

    juce::ComboBox xCcBox;
    juce::ComboBox yCcBox;

    juce::Label xLabel;
    juce::Label yLabel;

    juce::Label xValueLabel;
    juce::Label yValueLabel;

    juce::Label lockLabel;
    juce::Label clickLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MenuComponent)
};