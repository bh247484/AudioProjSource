#include "NoiseProcessor.h"
#include "NoiseEditor.h"

NoiseProcessor::NoiseProcessor()
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
    createWavetables();
}

NoiseProcessor::~NoiseProcessor()
{
}

//==============================================================================
const juce::String NoiseProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NoiseProcessor::acceptsMidi() const
{
    #if JucePlugin_WantsMidiInput
        return true;
    #else
        return false;
    #endif
}

bool NoiseProcessor::producesMidi() const
{
    #if JucePlugin_ProducesMidiOutput
        return true;
    #else
        return false;
    #endif
}

bool NoiseProcessor::isMidiEffect() const
{
    #if JucePlugin_IsMidiEffect
        return true;
    #else
        return false;
    #endif
}

double NoiseProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NoiseProcessor::getNumPrograms()
{
    return 1;
}

int NoiseProcessor::getCurrentProgram()
{
    return 0;
}

void NoiseProcessor::setCurrentProgram(int index)
{
}

const juce::String NoiseProcessor::getProgramName(int index)
{
    return {};
}

void NoiseProcessor::changeProgramName(int index, const juce::String& newName)
{
}

void NoiseProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
}

void NoiseProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NoiseProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
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


void NoiseProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    for (auto sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        float val = interpolateTable(waveTableIndex, triangleBuffer);

        waveTableIndex += tableDelta;
        while (waveTableIndex >= float(sineTableLen))
            waveTableIndex -= (float)sineTableLen;

        buffer.setSample(0, sample, val);
        buffer.setSample(1, sample, val);
    }



    /*for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float val;
            if (toggle)
            {
                val = random.nextFloat() * 0.25f - 0.125f;
            } else {
                val = 0;
            }
            buffer.setSample(channel, sample, val);
        }
    }*/
}

//==============================================================================
bool NoiseProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* NoiseProcessor::createEditor()
{
    return new NoiseEditor(*this);
}

//==============================================================================
void NoiseProcessor::getStateInformation(juce::MemoryBlock& destData)
{
}

void NoiseProcessor::setStateInformation(const void* data, int sizeInBytes)
{
}

void NoiseProcessor::toggleTrip()
{
    toggle = !toggle;
}

void NoiseProcessor::setFrequency(float frequency, float sampleRate)
{
    tableDelta = frequency * (sineTableLen / sampleRate);
}

//juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
//{
//    return new NoiseProcessor();
//}