#include "EnvelopeProcessor.h"
#include "EnvelopeEditor.h"

EnvelopeProcessor::EnvelopeProcessor()
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
    env.setAttackRate(10.0 * 48000.0);
    juce::Logger::outputDebugString((juce::String) getSampleRate());
    env.setDecayRate(.3 * 48000.0);
    env.setReleaseRate(10.0 * 48000.0);
    env.setSustainLevel(.8);
    env.gate(false);
}

EnvelopeProcessor::~EnvelopeProcessor()
{
}

//==============================================================================
const juce::String EnvelopeProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EnvelopeProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool EnvelopeProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool EnvelopeProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double EnvelopeProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EnvelopeProcessor::getNumPrograms()
{
    return 1;
}

int EnvelopeProcessor::getCurrentProgram()
{
    return 0;
}

void EnvelopeProcessor::setCurrentProgram(int index)
{
}

const juce::String EnvelopeProcessor::getProgramName(int index)
{
    return {};
}

void EnvelopeProcessor::changeProgramName(int index, const juce::String& newName)
{
}

void EnvelopeProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
}

void EnvelopeProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EnvelopeProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
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


void EnvelopeProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            if (channel == 0)
            {
                float bufferVal = buffer.getSample(channel, sample);
                float envVal = bufferVal * env.process();
                buffer.setSample(channel, sample, envVal);
            }
            if (channel == 1)
            {
                float bufferVal = buffer.getSample(channel, sample);
                float envVal = bufferVal * env.process();
                buffer.setSample(channel, sample, envVal);
            }
        }
    }
}

//==============================================================================
bool EnvelopeProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* EnvelopeProcessor::createEditor()
{
    return new EnvelopeEditor(*this);
}

//==============================================================================
void EnvelopeProcessor::getStateInformation(juce::MemoryBlock& destData)
{
}

void EnvelopeProcessor::setStateInformation(const void* data, int sizeInBytes)
{
}

//juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
//{
//    return new PanProcessor();
//}

