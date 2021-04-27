#include "EnvelopeProcessor.h"
#include "EnvelopeEditor.h"
//#include "PluginEditor.h"

//==============================================================================
EnvelopeEditor::EnvelopeEditor(EnvelopeProcessor& p)
    : AudioProcessorEditor(&p), envProc(p)
{
    addAndMakeVisible(gateButton);
    gateButton.onClick = [this] {envProc.env.getState() == 0 ? envProc.env.gate(true) : envProc.env.gate(false); };
    setSize(100, 100);
}

EnvelopeEditor::~EnvelopeEditor()
{
}

//==============================================================================
void EnvelopeEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::mediumpurple);
    g.setFont(15.0f);
}

void EnvelopeEditor::resized()
{
    gateButton.setBounds(5, 5, 75, 50);
}

