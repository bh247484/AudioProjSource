#pragma once

#include <JuceHeader.h>
#include "NoiseProcessor.h"

//==============================================================================
/**
*/
class NoiseEditor  : public juce::AudioProcessorEditor
{
public:
    NoiseEditor (NoiseProcessor&);
    ~NoiseEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    NoiseProcessor& noiseProc;

    juce::TextButton testBtn;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoiseEditor)
};
