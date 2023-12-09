#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WhoaAudioPluginProcessor::WhoaAudioPluginProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       //.withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       //.withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
{
}

WhoaAudioPluginProcessor::~WhoaAudioPluginProcessor()
{
}

//==============================================================================
const juce::String WhoaAudioPluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WhoaAudioPluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WhoaAudioPluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WhoaAudioPluginProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WhoaAudioPluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WhoaAudioPluginProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WhoaAudioPluginProcessor::getCurrentProgram()
{
    return 0;
}

void WhoaAudioPluginProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String WhoaAudioPluginProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void WhoaAudioPluginProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void WhoaAudioPluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void WhoaAudioPluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool WhoaAudioPluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void WhoaAudioPluginProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals noDenormals;
}

//==============================================================================
bool WhoaAudioPluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WhoaAudioPluginProcessor::createEditor()
{
    return new WhoaAudioPluginEditor (*this);
}

//==============================================================================
void WhoaAudioPluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
	juce::ignoreUnused (destData);
}

void WhoaAudioPluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WhoaAudioPluginProcessor();
}
