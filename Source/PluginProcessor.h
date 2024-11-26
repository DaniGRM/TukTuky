/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class TukTukyAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    TukTukyAudioProcessor();
    ~TukTukyAudioProcessor() override;

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
    void updateReadPosition(float delayTimeMs, double sampleRate);

    // Function to get parameter layout
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // Parameters value tree
    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", createParameterLayout() };
private:

    //Delay buffer, delay buffer size
    juce::AudioBuffer<float> delayBuffer;
    int delayBufferSize = 0;

    // Pointer to write and read
    int writePtr = 0;
    int readPtr = 0;

    // Params initialized
    float delayTime = 500.f;
    float feedback = 0.5f;
    float mix = 0.5;

    // Function to update params
    void updateParams();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TukTukyAudioProcessor)
};
