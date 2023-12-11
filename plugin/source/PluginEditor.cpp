#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WhoaAudioPluginEditor::WhoaAudioPluginEditor (WhoaAudioPluginProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processorRef (p), valueTreeState(vts), padComponent(vts), menuComponent(vts)
{
	juce::ignoreUnused (processorRef);

    setSize (400, 430);
	setResizable(true, true);

    addAndMakeVisible(padComponent);
    addAndMakeVisible(menuComponent);
}

WhoaAudioPluginEditor::~WhoaAudioPluginEditor()
{
}

//==============================================================================
void WhoaAudioPluginEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void WhoaAudioPluginEditor::resized()
{
    auto bounds = getLocalBounds();

    menuComponent.setBounds(bounds.removeFromBottom(30));
    padComponent.setBounds(bounds);
}

