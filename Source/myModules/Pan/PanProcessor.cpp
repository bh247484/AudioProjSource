#include "PanProcessor.h"
#include "PanEditor.h"

PanProcessor::PanProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
}

PanProcessor::~PanProcessor()
{
}

//==============================================================================
const juce::String PanProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PanProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PanProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PanProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PanProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PanProcessor::getNumPrograms()
{
    return 1;
}

int PanProcessor::getCurrentProgram()
{
    return 0;
}

void PanProcessor::setCurrentProgram(int index)
{
}

const juce::String PanProcessor::getProgramName(int index)
{
    return {};
}

void PanProcessor::changeProgramName(int index, const juce::String& newName)
{
}

void PanProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
}

void PanProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PanProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif


void PanProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    float pi = juce::MathConstants<float>::pi;
    float panAngle = (pi * (panAmt + 1)) / 4;

    for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            if (channel == 0)
            {
                float pannedVal = (buffer.getSample(channel, sample)) * cos(panAngle);
                buffer.setSample(channel, sample, pannedVal);
            }
            if (channel == 1)
            {
                float pannedVal = (buffer.getSample(channel, sample)) * sin(panAngle);
                buffer.setSample(channel, sample, pannedVal);
            }
        }
    }
}

//==============================================================================
bool PanProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* PanProcessor::createEditor()
{
    return new PanEditor(*this);
}

//==============================================================================
void PanProcessor::getStateInformation(juce::MemoryBlock& destData)
{
}

void PanProcessor::setStateInformation(const void* data, int sizeInBytes)
{
}

void PanProcessor::setPanAmt(float incomingPanAmt)
{
    panAmt = incomingPanAmt;
}

//juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
//{
//    return new PanProcessor();
//}
