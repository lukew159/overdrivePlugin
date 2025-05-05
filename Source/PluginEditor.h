/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <juce_gui_basics/juce_gui_basics.h> // For Slider, Component
#include <juce_audio_processors/juce_audio_processors.h> // For SliderAttachment

//==============================================================================
/**
*/
class OverdrivePluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    // Passing AudioProcessor and TreeState
    OverdrivePluginAudioProcessorEditor(OverdrivePluginAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~OverdrivePluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OverdrivePluginAudioProcessor& audioProcessor;

    // Reference to the Value Tree State passed from the processor
    juce::AudioProcessorValueTreeState& valueTreeState;

    // Declare the Gain slider UI element
    juce::Slider gainSlider;

    // Declare the attachment that links the slider to the parameter
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OverdrivePluginAudioProcessorEditor)
};
