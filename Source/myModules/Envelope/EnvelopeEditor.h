#pragma once

#include <JuceHeader.h>
#include "EnvelopeProcessor.h"

//==============================================================================
/**
*/
class EnvelopeEditor : public juce::AudioProcessorEditor
{
public:
    EnvelopeEditor(EnvelopeProcessor&);
    ~EnvelopeEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:

    EnvelopeProcessor& envProc;
    juce::TextButton gateButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeEditor)
};

