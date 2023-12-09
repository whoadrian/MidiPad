#pragma once

#include "PluginProcessor.h"

//==============================================================================
class WhoaAudioPluginEditor final : public juce::AudioProcessorEditor
{
public:
    explicit WhoaAudioPluginEditor (WhoaAudioPluginProcessor&);
    ~WhoaAudioPluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	WhoaAudioPluginProcessor& processorRef;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WhoaAudioPluginEditor)
};
