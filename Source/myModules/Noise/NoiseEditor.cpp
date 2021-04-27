#include "NoiseProcessor.h"
#include "NoiseEditor.h"
//#include "PluginEditor.h"

//==============================================================================
NoiseEditor::NoiseEditor (NoiseProcessor& p)
    : AudioProcessorEditor (&p), noiseProc(p)
{
    addAndMakeVisible(testBtn);
    testBtn.setButtonText("Test...");
    testBtn.onClick = [this] { noiseProc.toggleTrip(); };
    setSize (100, 100);
}

NoiseEditor::~NoiseEditor()
{
}

//==============================================================================
void NoiseEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::mediumpurple);
    g.setFont (15.0f);
}

void NoiseEditor::resized()
{
    testBtn.setBounds(5, 5, 75, 50);
}
