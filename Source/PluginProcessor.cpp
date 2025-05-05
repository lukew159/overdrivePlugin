/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OverdrivePluginAudioProcessor::OverdrivePluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    // Initalise treeState
    treeState(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
    // Raw pointer to gain value after TreeState is initalised 
    gainParameter = treeState.getRawParameterValue("gain");
}

OverdrivePluginAudioProcessor::~OverdrivePluginAudioProcessor()
{
}

// Define the layout creation function
juce::AudioProcessorValueTreeState::ParameterLayout OverdrivePluginAudioProcessor::createParameterLayout()
{
    // Create a vector (list) to hold parameter unique pointers
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Create our Gain parameter:
    // ID: "gain" (used to identify it)
    // Name: "Gain" (shown in host automation)
    // Range: 0.0f (silent) to 2.0f (double volume, +6dB)
    // Default Value: 1.0f (unity gain, no change)
    auto gainParam = std::make_unique<juce::AudioParameterFloat>("gain",        // Parameter ID
        "Gain",        // Parameter Name
        0.0f,          // Minimum value
        2.0f,          // Maximum value
        1.0f);         // Default value

    // Add the parameter to our list
    params.push_back(std::move(gainParam));

    // Return the layout containing our parameter(s)
    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String OverdrivePluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OverdrivePluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OverdrivePluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OverdrivePluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OverdrivePluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OverdrivePluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OverdrivePluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OverdrivePluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String OverdrivePluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void OverdrivePluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void OverdrivePluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void OverdrivePluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OverdrivePluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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
#endif

void OverdrivePluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Get the gain value from our atomic float pointer.
    // Using std::atomic ensures thread safety when the value is updated
    // by the UI thread or host automation.
    float currentGain = gainParameter->load();

    // Apply the gain to the buffer
    // This applies the gain to all channels and all samples efficiently.
    buffer.applyGain(currentGain);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool OverdrivePluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OverdrivePluginAudioProcessor::createEditor()
{
    // Pass processor to treeState and the editor
    return new OverdrivePluginAudioProcessorEditor (*this, treeState);
}

//==============================================================================
void OverdrivePluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void OverdrivePluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OverdrivePluginAudioProcessor();
}
