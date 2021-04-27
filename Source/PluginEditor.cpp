/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MarkIIIAudioProcessorEditor::MarkIIIAudioProcessorEditor (MarkIIIAudioProcessor& p)
    : AudioProcessorEditor (&p), mainProcessor (p)
{
    addAndMakeVisible(mainProcessor.noiseEditor);
    addAndMakeVisible(mainProcessor.panEditor);
    addAndMakeVisible(mainProcessor.envEditor);
    setSize (800, 600);
}

MarkIIIAudioProcessorEditor::~MarkIIIAudioProcessorEditor()
{
}

//==============================================================================
void MarkIIIAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void MarkIIIAudioProcessorEditor::resized()
{
    mainProcessor.noiseEditor->setBounds(10, 10, 200, 100);
    mainProcessor.panEditor->setBounds(10, 250, 200, 100);
    mainProcessor.envEditor->setBounds(10, 500, 200, 100);
}
