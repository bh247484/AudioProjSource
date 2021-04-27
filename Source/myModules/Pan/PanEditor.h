#pragma once

#include <JuceHeader.h>
#include "PanProcessor.h"

//==============================================================================
/**
*/
class PanEditor : public juce::AudioProcessorEditor
{
public:
    PanEditor(PanProcessor&);
    ~PanEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    PanProcessor& panProc;

    juce::Slider panPot;

    //juce::TextButton testBtn;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PanEditor)
};

