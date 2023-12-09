#pragma once

#include "PluginProcessor.h"

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
	
	// parameters
	juce::AudioProcessorValueTreeState& valueTreeState;
	
	// gain
	juce::Label gainLabel;
	juce::Slider gainSlider;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WhoaAudioPluginEditor)
};
