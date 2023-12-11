#pragma once

#include "PluginProcessor.h"
#include "PadComponent.h"
#include "MenuComponent.h"

//==============================================================================
class WhoaAudioPluginEditor final : public juce::AudioProcessorEditor
{
public:
	explicit WhoaAudioPluginEditor (WhoaAudioPluginProcessor&, juce::AudioProcessorValueTreeState& vts);
    ~WhoaAudioPluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	WhoaAudioPluginProcessor& processorRef;

	juce::AudioProcessorValueTreeState& valueTreeState;

	PadComponent padComponent;
    MenuComponent menuComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WhoaAudioPluginEditor)
};
