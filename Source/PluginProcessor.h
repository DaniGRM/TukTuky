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



    const int NORMAL_MODE = 0;
    const int SYNC_MODE = 1;

    const juce::Array<float> SYNC_FRAC = {
        1.f / 16.f,
        1.f / 8.f,
        1.f / 6.f,
        1.f / 4.f,
        1.f / 3.f,
        1.f / 2.f,
        1.f,
    };


    void setMode(int m) {
        mode = m;
        updateParams();
    }

    void setPingPong(bool set) {
        pingPong = set;
    }
private:
    float ramp(int channel, int x) {

        int td = static_cast<int>(delayTime * getSampleRate());

        float y;
        if (channel == pingPongChannel)// ACTIVE CHANNEL
        {
            if (x < (td / 2))
            {
      
                y = 0.5f + (static_cast<float>(x) / td);
            }
            else
            {
                y = 1.5f - (static_cast<float>(x) / td);
            }
        }
        else// INACTIVE CHANNEL
        {
            if (x < (td / 2))
            {
                y = 0.5f - (static_cast<float>(x) / td);
            }
            else
            {
                y = 0.5f * (static_cast<float>(2 * x - td ) / td);
            }
        }

        if (y > 1.0f) {
            return 1.0f;
        }

        if (y < 0.0f) {
            return 0.0f;
        }
        return y;
    }
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


    int mode = NORMAL_MODE;


    int pingPongChannel = 0;
    int pingPongCount = 0;
    bool pingPong = false;
    // Function to update params
    void updateParams();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TukTukyAudioProcessor)
};
