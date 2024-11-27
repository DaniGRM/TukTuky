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

    // Header for all Tuky plugins
    TukyUI::Components::TukyHeader tukyHeader;

    // Sliders for params
    TukyUI::Components::TukyRotarySlider delaySlider,
        feedbackSlider,
        mixSlider;

    // Usings to make code more readable
    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    // Slider Attachment for sliders
    Attachment delaySliderAttachment,
        feedbackSliderAttachment,
        mixSliderAttachment;

    // Labels for sliders
    juce::Label delayLabel,
        feedbackLabel,
        mixLabel;

    //Toggle Buttons 
    TukyUI::Components::TukyToggleButton normalButton, syncButton;


    // Internal function to get references of all components declared before
    std::vector<juce::Component*> getComps();
    void setLabel(juce::Label& label, juce::String text, juce::Rectangle<int>bounds);

    void normalClicked()
    {
        
        if (normalButton.getToggleState())
        {
            // Normal ON -> SYNC OFF
            syncButton.setToggleState(false, true);
            audioProcessor.setMode(audioProcessor.NORMAL_MODE);
        }
        else
        {
            // Normal OFF -> SYNC ON
            syncButton.setToggleState(true, true);
            audioProcessor.setMode(audioProcessor.SYNC_MODE);
        }
    }

    void syncClicked()
    {
        if (syncButton.getToggleState())
        {
            // SYNC ON -> NORMAL OFF
            normalButton.setToggleState(false, true);
        }
        else
        {
            // SYNC OFF -> NORMAL ON
            normalButton.setToggleState(true, true);
        }
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TukTukyAudioProcessorEditor)
};
