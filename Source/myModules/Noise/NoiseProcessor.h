/*
  ==============================================================================

    NoiseProcessor.h
    Created: 13 Nov 2020 6:00:14pm
    Author:  bh247

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class NoiseProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    NoiseProcessor();
    ~NoiseProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    void toggleTrip();
    void setFrequency(float frequency, float sampleRate);

private:

    float interpolateTable(float tableIndex, juce::AudioSampleBuffer& waveTable)
    {
        auto lowerIndex = (unsigned int) tableIndex;
        auto upperIndex = lowerIndex == (sineTableLen - 1) ? (unsigned int)0 : lowerIndex + 1;

        // Subtract interstital float index from down-rounded lower index
        auto fracDelta = tableIndex - (float)lowerIndex;

        auto lowerVal = waveTable.getSample(0, lowerIndex);
        auto upperVal = waveTable.getSample(0, upperIndex);

        auto interpolatedSample = lowerVal + fracDelta * (upperVal - lowerVal);

        return interpolatedSample;
    }
    
    void createSine()
    {
        sineBuffer.setSize(1, (int)sineTableLen);
        auto angleDelta = juce::MathConstants<double>::twoPi / (double)(sineTableLen);
        auto theta = 0.0;

        for (int i = 0; i < sineBuffer.getNumSamples(); i++)
        {
            auto sampleVal = std::sin(theta);
            sineBuffer.setSample(0, i, (float)sampleVal);
            theta += angleDelta;
        }
    }

    void createSquare()
    {
        squareBuffer.setSize(1, (int)sineTableLen);
        auto angleDelta = juce::MathConstants<double>::twoPi / (double)(sineTableLen);
        auto theta = 0.0;
        double sampleVal;

        for (int i = 0; i < squareBuffer.getNumSamples(); i++)
        {
            if (theta < juce::MathConstants<double>::pi)
            {
                sampleVal = 1;
            }
            if (theta > juce::MathConstants<double>::pi)
            {
                sampleVal = 0;
            }
            squareBuffer.setSample(0, i, (float)sampleVal);
            theta += angleDelta;
        }
    }

    void createSawtooth()
    {
        sawtoothBuffer.setSize(1, (int)sineTableLen);
        auto angleDelta = juce::MathConstants<double>::twoPi / (double)(sineTableLen);
        auto theta = 0.0;
        double sampleVal;

        for (int i = 0; i < sawtoothBuffer.getNumSamples(); i++)
        {
            sampleVal = 1 - (1 / juce::MathConstants<double>::pi * theta);
            sawtoothBuffer.setSample(0, i, (float)sampleVal);
            theta += angleDelta;
        }
    }

    void createTriangle()
    {
        triangleBuffer.setSize(1, (int)sineTableLen);
        auto angleDelta = juce::MathConstants<double>::twoPi / (double)(sineTableLen);
        auto theta = 0.0;
        double sampleVal;

        for (int i = 0; i < triangleBuffer.getNumSamples(); i++)
        {
            if (theta < juce::MathConstants<double>::pi)
            {
                sampleVal = -1 + (2 * 1 / juce::MathConstants<double>::pi) * theta;
            }
            if (theta > juce::MathConstants<double>::pi)
            {
                sampleVal = 3 - (2 * 1 / juce::MathConstants<double>::pi) * theta;
            }
            triangleBuffer.setSample(0, i, (float)sampleVal);
            theta += angleDelta;
        }
    }

    void createWavetables()
    {
        createSine();
        createSquare();
        createSawtooth();
        createTriangle();
    }

    bool toggle = true;
    
    // Wavetables
    juce::AudioSampleBuffer sineBuffer;
    juce::AudioSampleBuffer triangleBuffer;
    juce::AudioSampleBuffer squareBuffer;
    juce::AudioSampleBuffer sawtoothBuffer;
    const unsigned int sineTableLen = 1 << 10;
    
    float frequency = 440.0f;
    //float tableDelta = frequency * ((float)sineTableLen / 48000.0f);
    float tableDelta = (float)sineTableLen * frequency / 48000.0f;
    float waveTableIndex = 0.0f;

    juce::Random random;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoiseProcessor)
};