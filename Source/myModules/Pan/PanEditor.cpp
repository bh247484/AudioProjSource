#include "PanProcessor.h"
#include "PanEditor.h"
//#include "PluginEditor.h"

//==============================================================================
PanEditor::PanEditor(PanProcessor& p)
    : AudioProcessorEditor(&p), panProc(p)
{
    addAndMakeVisible(panPot);
    panPot.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    panPot.setRange(-1.0, 1.0);
    panPot.setValue(0.0);
    panPot.onValueChange = [this] { panProc.setPanAmt(panPot.getValue());};
    setSize(100, 100);
}

PanEditor::~PanEditor()
{
}

//==============================================================================
void PanEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::mediumpurple);
    g.setFont(15.0f);
}

void PanEditor::resized()
{
    panPot.setBounds(5, 5, 75, 50);
}
