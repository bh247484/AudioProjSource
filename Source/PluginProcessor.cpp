/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//#include "./myModules/Noise/NoiseProcessor.h"

//==============================================================================
MarkIIIAudioProcessor::MarkIIIAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    apGraph (new juce::AudioProcessorGraph())
#endif
{
}

MarkIIIAudioProcessor::~MarkIIIAudioProcessor()
{
    delete noiseEditor;
    delete panEditor;
    delete envEditor;
}

//==============================================================================
const juce::String MarkIIIAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MarkIIIAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MarkIIIAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MarkIIIAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MarkIIIAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MarkIIIAudioProcessor::getNumPrograms()
{
    return 1;
}

int MarkIIIAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MarkIIIAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MarkIIIAudioProcessor::getProgramName (int index)
{
    return {};
}

void MarkIIIAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MarkIIIAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    apGraph->setPlayConfigDetails(getMainBusNumInputChannels(),
                                           getMainBusNumOutputChannels(),
                                           sampleRate, samplesPerBlock);
    
    apGraph->prepareToPlay(sampleRate, samplesPerBlock);

    initGraph();
}

void MarkIIIAudioProcessor::releaseResources()
{
    apGraph->releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MarkIIIAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
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

void MarkIIIAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; i++)
        buffer.clear(i, 0, buffer.getNumSamples());

    updateGraph();

    apGraph->processBlock(buffer, midiMessages);
}

//==============================================================================
bool MarkIIIAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* MarkIIIAudioProcessor::createEditor()
{
    return new MarkIIIAudioProcessorEditor (*this);
}

//==============================================================================
void MarkIIIAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void MarkIIIAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MarkIIIAudioProcessor();
}
