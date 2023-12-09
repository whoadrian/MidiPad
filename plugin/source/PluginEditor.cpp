#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WhoaAudioPluginEditor::WhoaAudioPluginEditor (WhoaAudioPluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
	juce::ignoreUnused (processorRef);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
	setResizable(true, true);
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
}
