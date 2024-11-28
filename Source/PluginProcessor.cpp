/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TukTukyAudioProcessor::TukTukyAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    // We clear the buffer to avoid interferences with trash samples
    delayBuffer.clear();
}

TukTukyAudioProcessor::~TukTukyAudioProcessor()
{
}

//==============================================================================
const juce::String TukTukyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TukTukyAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TukTukyAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TukTukyAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TukTukyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TukTukyAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TukTukyAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TukTukyAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TukTukyAudioProcessor::getProgramName (int index)
{
    return {};
}

void TukTukyAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TukTukyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Set buffer size to 2 seconds
    delayBufferSize = static_cast<int>((2.0 /* segundos */) * getSampleRate());
    delayBuffer.setSize(2, delayBufferSize);

    // Update params for the first iteration
    updateParams();

    // We put the read pointer delay samples before for the first iteration
    readPtr = (writePtr - static_cast<int>(delayTime * getSampleRate()) + delayBufferSize) % delayBufferSize;
}

void TukTukyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TukTukyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TukTukyAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();
    auto sampleRate = getSampleRate();


    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // We call update params each block in case something has change
    updateParams();

    // Temporal write pointe to keep the position at the beggining of this function
    // This variable is very important to write the samples of each channel from the right position
    int tempWritePtr;
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {

        // For each channel we iterate through the samples 
        auto* channelData = buffer.getWritePointer (channel);
        auto* delayData = delayBuffer.getWritePointer(channel);
        tempWritePtr = writePtr;
        for (int sample = 0; sample < numSamples; sample++)
        {
            //update read pointer position to make it be before writer
            readPtr = (tempWritePtr - static_cast<int>(delayTime * getSampleRate()) + delayBufferSize) % delayBufferSize;


            // Remember delayed sample
            float delayedSample = delayData[readPtr];

            // Write into delay buffer with feedback
            delayData[tempWritePtr] = channelData[sample] + feedback * delayedSample;

            // Mix original sample with delayed one
            channelData[sample] = (1.0f - mix) * channelData[sample] + mix * delayedSample;

            // Increase write pointer
            tempWritePtr = (tempWritePtr + 1) % delayBufferSize;
        }
    }
    // At the end of each block we update write position with the temporal position value
    writePtr = tempWritePtr;
}

//==============================================================================
bool TukTukyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TukTukyAudioProcessor::createEditor()
{
    return new TukTukyAudioProcessorEditor (*this);
}

//==============================================================================
void TukTukyAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TukTukyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid()) {
        apvts.replaceState(tree);
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout TukTukyAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>("Delay", "Delay", juce::NormalisableRange<float>(0.1f, 2.f, 0.1f, 1.f), 0.5f));
    layout.add(std::make_unique<juce::AudioParameterInt>("Delay Sync", "Delay Sync", 0, 6, 6));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Feedback", "Feedback", juce::NormalisableRange<float>(0.f, 1.f, 0.05f, 1.f), 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Mix", "Mix", juce::NormalisableRange<float>(0.f, 1.f, 0.05f, 1.f), 0.5f));

    return layout;
}

// In this function we only update params value if GUI has changed on some way
void TukTukyAudioProcessor::updateParams() {
    switch (mode)
    {
    case 0:
        delayTime = apvts.getRawParameterValue("Delay")->load();
        break;
    case 1:
        if (auto* playHead = getPlayHead()) {
            // Creamos una instancia para almacenar la posición
            juce::AudioPlayHead::CurrentPositionInfo positionInfo;

            // Obtenemos la información actual de posición
            if (playHead->getCurrentPosition(positionInfo))
            {
                // Accedemos al BPM actual
                auto bpm = positionInfo.bpm;
                delayTime = bpm / 60.f * SYNC_FRAC[apvts.getRawParameterValue("Delay Sync")->load()];
            }
        }
    default:
        break;
    }
    mix = apvts.getRawParameterValue("Mix")->load();
    feedback = apvts.getRawParameterValue("Feedback")->load();
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TukTukyAudioProcessor();
}
