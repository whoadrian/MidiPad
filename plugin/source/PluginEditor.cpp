#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WhoaAudioPluginEditor::WhoaAudioPluginEditor (WhoaAudioPluginProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processorRef (p), valueTreeState (vts)
{
	juce::ignoreUnused (processorRef);
	
	gainLabel.setText ("Gain", juce::dontSendNotification);
	addAndMakeVisible (gainLabel);
	
	addAndMakeVisible (gainSlider);
	gainAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (valueTreeState, "gain", gainSlider));
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

WhoaAudioPluginEditor::~WhoaAudioPluginEditor()
{
}

//==============================================================================
void WhoaAudioPluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void WhoaAudioPluginEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	gainSlider.setBounds(getBounds().removeFromRight(getBounds().getWidth() * 0.8f));
	gainLabel.setBounds(getBounds());
}
