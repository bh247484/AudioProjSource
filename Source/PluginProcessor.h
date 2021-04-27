/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "myModules/Noise/NoiseProcessor.h"
#include "myModules/Pan/PanProcessor.h"
#include "myModules/Envelope/EnvelopeProcessor.h"

//==============================================================================
/**
*/
class MarkIIIAudioProcessor  : public juce::AudioProcessor
{
public:
    using AudioGraphIOProcessor = juce::AudioProcessorGraph::AudioGraphIOProcessor;
    using Node = juce::AudioProcessorGraph::Node;

    //==============================================================================
    MarkIIIAudioProcessor();
    ~MarkIIIAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Editors
    juce::AudioProcessorEditor* noiseEditor;
    juce::AudioProcessorEditor* panEditor;
    juce::AudioProcessorEditor* envEditor;

private:

    void initGraph()
    {
        apGraph->clear();

        // Ins and Outs
        audioInputNode = apGraph->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioInputNode));
        audioOutputNode = apGraph->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode));
        midiInputNode = apGraph->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::midiInputNode));
        midiOutputNode = apGraph->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::midiOutputNode));

        // My Processors
        noiseNode = apGraph->addNode(std::make_unique<NoiseProcessor>());
        panNode = apGraph->addNode(std::make_unique<PanProcessor>());
        envNode = apGraph->addNode(std::make_unique<EnvelopeProcessor>());

        //Add Processors to Array
        myProcessors.add(noiseNode);
        myProcessors.add(panNode);
        myProcessors.add(envNode);

        // Create Attendant Editors
        noiseEditor = noiseNode->getProcessor()->createEditor();
        panEditor = panNode->getProcessor()->createEditor();
        envEditor = envNode->getProcessor()->createEditor();

        connectAudioNodes();
        connectMidiNodes();
    }

    void updateGraph()
    {
        //slots.add(noiseNode);
        //slots.set(0, centralProcessor->addNode(std::make_unique<NoiseProcessor>()));
        //noiseNode = centralProcessor->addNode(std::make_unique<NoiseProcessor>());
        //slots.getFirst()->

        for (auto node : apGraph->getNodes())
        {
            node->getProcessor()->setPlayConfigDetails(getMainBusNumInputChannels(),
                                                       getMainBusNumOutputChannels(),
                                                       getSampleRate(), getBlockSize());
        }

        // Start loop at 3 to avoid audio + midi input/output nodes
        for (int i = 0; i < myProcessors.size(); i++)
        {
            for (int channel = 0; channel < 2; channel++)
            {
                if (i == 0)
                {
                    // connect the input node to the first of my processor nodes
                    apGraph->addConnection({ {audioInputNode->nodeID, channel},
                                             {myProcessors[i]->nodeID, channel} });
                }
                // else connect it to the previous node
                else
                {
                    apGraph->addConnection({ {myProcessors[i - 1]->nodeID, channel},
                                             {myProcessors[i]->nodeID, channel} });
                }
                // if it's the last of my processor nodes, connect it to the output node
                if (i == (myProcessors.size() - 1))
                {
                    apGraph->addConnection({ {myProcessors[i]->nodeID, channel},
                                             {audioOutputNode->nodeID, channel} });
                }
            }
        }

        connectMidiNodes();

        for (auto node : apGraph->getNodes())
            node->getProcessor()->enableAllBuses();
    }

    void connectAudioNodes()
    {
        for (int channel = 0; channel < 2; channel++)
            apGraph->addConnection({ { audioInputNode->nodeID, channel },
                                     { audioOutputNode->nodeID, channel } });
    }

    void connectMidiNodes()
    {
        apGraph->addConnection({ { midiInputNode->nodeID,  juce::AudioProcessorGraph::midiChannelIndex },
                                 { midiOutputNode->nodeID, juce::AudioProcessorGraph::midiChannelIndex } });
    }

    std::unique_ptr<juce::AudioProcessorGraph> apGraph;

    Node::Ptr audioInputNode;
    Node::Ptr audioOutputNode;
    Node::Ptr midiInputNode;
    Node::Ptr midiOutputNode;

    // Processor Nodes
    Node::Ptr noiseNode;
    Node::Ptr panNode;
    Node::Ptr envNode;

    // MyProcessors Array
    juce::ReferenceCountedArray<Node> myProcessors;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MarkIIIAudioProcessor)
};
