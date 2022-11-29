/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TestTimingAudioProcessorEditor::TestTimingAudioProcessorEditor (TestTimingAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    
    flushButton.setClickingTogglesState(true);
    flushButton.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    flushButton.onClick = [&]()
    {
        const auto message = flushButton.getToggleState() ? "On" : "Off";
        printf(message);
        flushButton.setButtonText(message);
        audioProcessor.flushNow = flushButton.getToggleState();
        
        
    };
    addAndMakeVisible(flushButton);
    
    setSize (400, 300);
}

TestTimingAudioProcessorEditor::~TestTimingAudioProcessorEditor()
{
}

//==============================================================================
void TestTimingAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void TestTimingAudioProcessorEditor::resized()
{
     flushButton.setBounds(0, 184, 160, 50);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
