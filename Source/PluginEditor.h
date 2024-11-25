/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "TukyUI.h"

//==============================================================================
/**
*/
class TukTukyAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TukTukyAudioProcessorEditor (TukTukyAudioProcessor&);
    ~TukTukyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TukTukyAudioProcessor& audioProcessor;

    TukyUI::Components::TukyHeader tukyHeader;

    TukyUI::Components::TukyRotarySlider delaySlider,
        feedbackSlider,
        mixSlider;

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment delaySliderAttachment,
        feedbackSliderAttachment,
        mixSliderAttachment;

    juce::Label delayLabel,
        feedbackLabel,
        mixLabel,
        titleLabel;
    std::vector<juce::Component*> getComps();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TukTukyAudioProcessorEditor)
};
