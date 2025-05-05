/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OverdrivePluginAudioProcessorEditor::OverdrivePluginAudioProcessorEditor (OverdrivePluginAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts)
{
    // Configure the Gain Slider:
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary); // Or LinearHorizontal, etc.
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 20); // Show value box below

    // Make the slider visible in the editor window
    addAndMakeVisible(gainSlider);

    // Create the attachment:
    // This automatically connects the slider to the "gain" parameter in the valueTreeState.
    // It handles sending value changes from the slider to the parameter,
    // and updating the slider's position when the parameter changes (e.g., via automation).
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState, // The state tree
        "gain",         // The ID of the parameter to attach to
        gainSlider);    // The slider UI element

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

OverdrivePluginAudioProcessorEditor::~OverdrivePluginAudioProcessorEditor()
{
}

//==============================================================================
void OverdrivePluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void OverdrivePluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // Set the bounds (position and size) of the gain slider
    // Example: Place it in the center horizontally, offset from the top
    auto border = 20;
    auto sliderWidth = getWidth() - 2 * border;
    auto sliderHeight = getHeight() - 2 * border; // Use most of the space for a rotary
    gainSlider.setBounds(border, border, sliderWidth, sliderHeight);
}
