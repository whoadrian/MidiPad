#pragma once

class MenuComponent final : public juce::Component
{
public:
    MenuComponent(juce::AudioProcessorValueTreeState& vts) :
            xCcValParam(vts.getParameter("xccval")),
            yCcValParam(vts.getParameter("yccval")),
            xCcChParam(vts.getParameter("xccch")),
            yCcChParam(vts.getParameter("yccch")),
            xMidiChParam(vts.getParameter("xmidich")),
            yMidiChParam(vts.getParameter("ymidich")),
            lockParam(vts.getParameter("locked"))
    {
        // Setup titles
        xLabel.setText("X", juce::dontSendNotification);
        xLabel.setEditable(false, false);
        xLabel.setJustificationType(juce::Justification::centredLeft);
        addAndMakeVisible(xLabel);

        yLabel.setText("Y", juce::dontSendNotification);
        yLabel.setEditable(false, false);
        yLabel.setJustificationType(juce::Justification::centredRight);
        addAndMakeVisible(yLabel);

        // Setup Cc Values
        xCcValLabel.setEditable(false, false);
        xCcValLabel.setJustificationType(juce::Justification::centredLeft);
        addAndMakeVisible(xCcValLabel);

        yCcValLabel.setEditable(false, false);
        yCcValLabel.setJustificationType(juce::Justification::centredRight);
        addAndMakeVisible(yCcValLabel);

        xCcValTitle.setText("Cc Val", juce::dontSendNotification);
        xCcValTitle.setEditable(false, false);
        xCcValTitle.setJustificationType(juce::Justification::centredLeft);
        addAndMakeVisible(xCcValTitle);

        yCcValTitle.setText("Cc Val", juce::dontSendNotification);
        yCcValTitle.setEditable(false, false);
        yCcValTitle.setJustificationType(juce::Justification::centredRight);
        addAndMakeVisible(yCcValTitle);

        // Setup Cc Channels
        for (int xValue = (int)xCcChParam->getNormalisableRange().start; xValue <= xCcChParam->getNormalisableRange().end; ++xValue)
            xCcChBox.addItem(std::to_string(xValue), xValue + 1);
        for (int yValue = (int)yCcChParam->getNormalisableRange().start; yValue <= yCcChParam->getNormalisableRange().end; ++yValue)
            yCcChBox.addItem(std::to_string(yValue), yValue + 1);

        xCcChBox.setSelectedId((int)xCcChParam->convertFrom0to1(xCcChParam->getValue()) + 1, juce::dontSendNotification);
        yCcChBox.setSelectedId((int)yCcChParam->convertFrom0to1(yCcChParam->getValue()) + 1, juce::dontSendNotification);

        xCcChBox.onChange = [this] {onXccChange();};
        yCcChBox.onChange = [this] {onYccChange();};

        addAndMakeVisible(yCcChBox);
        addAndMakeVisible(xCcChBox);

        xCcChTitle.setText("Cc Ch", juce::dontSendNotification);
        xCcChTitle.setEditable(false, false);
        xCcChTitle.setJustificationType(juce::Justification::centredLeft);
        addAndMakeVisible(xCcChTitle);

        yCcChTitle.setText("Cc Ch", juce::dontSendNotification);
        yCcChTitle.setEditable(false, false);
        yCcChTitle.setJustificationType(juce::Justification::centredRight);
        addAndMakeVisible(yCcChTitle);

        // Setup Midi Channels
        for (int xValue = (int)xMidiChParam->getNormalisableRange().start; xValue <= xMidiChParam->getNormalisableRange().end; ++xValue)
            xMidiChBox.addItem(std::to_string(xValue), xValue);
        for (int yValue = (int)yMidiChParam->getNormalisableRange().start; yValue <= yMidiChParam->getNormalisableRange().end; ++yValue)
            yMidiChBox.addItem(std::to_string(yValue), yValue);

        xMidiChBox.setSelectedId((int)xMidiChParam->convertFrom0to1(xMidiChParam->getValue()), juce::dontSendNotification);
        yMidiChBox.setSelectedId((int)yMidiChParam->convertFrom0to1(yMidiChParam->getValue()), juce::dontSendNotification);

        xMidiChBox.onChange = [this] {onXMidiChChange();};
        yMidiChBox.onChange = [this] {onYMidiChChange();};

        addAndMakeVisible(xMidiChBox);
        addAndMakeVisible(yMidiChBox);

        xMidiChTitle.setText("Midi Ch", juce::dontSendNotification);
        xMidiChTitle.setEditable(false, false);
        xMidiChTitle.setJustificationType(juce::Justification::centredLeft);
        addAndMakeVisible(xMidiChTitle);

        yMidiChTitle.setText("Midi Ch", juce::dontSendNotification);
        yMidiChTitle.setEditable(false, false);
        yMidiChTitle.setJustificationType(juce::Justification::centredRight);
        addAndMakeVisible(yMidiChTitle);

        // Setup lock
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
        xCcValLabel.setText(std::to_string((int)xCcValParam->convertFrom0to1(xCcValParam->getValue())), juce::dontSendNotification);
        yCcValLabel.setText(std::to_string((int)yCcValParam->convertFrom0to1(yCcValParam->getValue())), juce::dontSendNotification);

        bool isLocked = lockParam->getValue() == 1.0f;
        lockLabel.setText(isLocked ? "LOCKED" : "UNLOCKED", juce::dontSendNotification);

        repaint();
    }

    void resized() override
    {
        auto bounds = getLocalBounds();

        xLabel.setBounds(bounds.removeFromLeft(35));
        yLabel.setBounds(bounds.removeFromRight(35));

        const float rowHeight = bounds.getHeight() * 0.333f;
        const float rowWidth = 120;

        // X Settings
        auto xSettingsBounds = bounds.removeFromLeft(rowWidth);
        auto rowBounds = xSettingsBounds.removeFromTop(rowHeight);
        xCcValTitle.setBounds(rowBounds.removeFromLeft(rowWidth * 0.5f));
        xCcValLabel.setBounds(rowBounds);

        rowBounds = xSettingsBounds.removeFromTop(rowHeight).reduced(0, 2);
        xCcChTitle.setBounds(rowBounds.removeFromLeft(rowWidth * 0.5f));
        xCcChBox.setBounds(rowBounds);

        rowBounds = xSettingsBounds.removeFromTop(rowHeight).reduced(0, 2);
        xMidiChTitle.setBounds(rowBounds.removeFromLeft(rowWidth * 0.5f));
        xMidiChBox.setBounds(rowBounds);

        // Y Settings
        auto ySettingsBounds = bounds.removeFromRight(rowWidth);
        rowBounds = ySettingsBounds.removeFromTop(rowHeight).reduced(0, 2);
        yCcValTitle.setBounds(rowBounds.removeFromRight(rowWidth * 0.5f));
        yCcValLabel.setBounds(rowBounds);

        rowBounds = ySettingsBounds.removeFromTop(rowHeight).reduced(0, 2);
        yCcChTitle.setBounds(rowBounds.removeFromRight(rowWidth * 0.5f));
        yCcChBox.setBounds(rowBounds);

        rowBounds = ySettingsBounds.removeFromTop(rowHeight).reduced(0, 2);
        yMidiChTitle.setBounds(rowBounds.removeFromRight(rowWidth * 0.5f));
        yMidiChBox.setBounds(rowBounds);

        // Lock
        lockLabel.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.8f));
        clickLabel.setBounds(bounds);
    }

private:
    void onXccChange()
    {
        xCcChParam->beginChangeGesture();
        xCcChParam->setValueNotifyingHost(xCcChParam->convertTo0to1((float)xCcChBox.getSelectedId() - 1));
        xCcChParam->endChangeGesture();
    }

    void onYccChange()
    {
        yCcChParam->beginChangeGesture();
        yCcChParam->setValueNotifyingHost(yCcChParam->convertTo0to1((float)yCcChBox.getSelectedId() - 1));
        yCcChParam->endChangeGesture();
    }

    void onXMidiChChange()
    {
        xMidiChParam->beginChangeGesture();
        xMidiChParam->setValueNotifyingHost(xMidiChParam->convertTo0to1((float)xMidiChBox.getSelectedId()));
        xMidiChParam->endChangeGesture();
    }

    void onYMidiChChange()
    {
        yMidiChParam->beginChangeGesture();
        yMidiChParam->setValueNotifyingHost(yMidiChParam->convertTo0to1((float)yMidiChBox.getSelectedId()));
        yMidiChParam->endChangeGesture();
    }

    juce::RangedAudioParameter* xCcValParam;
    juce::RangedAudioParameter* yCcValParam;
    juce::RangedAudioParameter* xCcChParam;
    juce::RangedAudioParameter* yCcChParam;
    juce::RangedAudioParameter* xMidiChParam;
    juce::RangedAudioParameter* yMidiChParam;
    juce::RangedAudioParameter* lockParam;

    juce::Label xLabel;
    juce::Label yLabel;

    juce::Label xCcChTitle;
    juce::Label yCcChTitle;
    juce::ComboBox xCcChBox;
    juce::ComboBox yCcChBox;

    juce::Label xMidiChTitle;
    juce::Label yMidiChTitle;
    juce::ComboBox xMidiChBox;
    juce::ComboBox yMidiChBox;

    juce::Label xCcValTitle;
    juce::Label yCcValTitle;
    juce::Label xCcValLabel;
    juce::Label yCcValLabel;

    juce::Label lockLabel;
    juce::Label clickLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MenuComponent)
};