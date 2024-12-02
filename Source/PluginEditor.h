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
        delaySyncSlider,
        feedbackSlider,
        mixSlider;

    // Usings to make code more readable
    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    // Slider Attachment for sliders
    Attachment delaySliderAttachment,
        delaySyncSliderAttachment,
        feedbackSliderAttachment,
        mixSliderAttachment;

    // Labels for sliders
    juce::Label delayLabel,
        feedbackLabel,
        mixLabel;

    //Toggle Buttons 
    TukyUI::Components::TukyToggleButton syncButton, pingPongButton;


    // Internal function to get references of all components declared before
    std::vector<juce::Component*> getComps();
    void setLabel(juce::Label& label, juce::String text, juce::Rectangle<int>bounds);

    void syncClicked()
    {
        if (syncButton.getToggleState())
        {
            // SYNC ON -> NORMAL OFF
            audioProcessor.setMode(audioProcessor.SYNC_MODE);
            delaySlider.setVisible(false);
            delaySyncSlider.setVisible(true);
        }
        else
        {
            // SYNC OFF -> NORMAL ON
            audioProcessor.setMode(audioProcessor.NORMAL_MODE);
            delaySlider.setVisible(true);
            delaySyncSlider.setVisible(false);
        }
    }

    void pingPongClicked()
    {
        if (pingPongButton.getToggleState())
        {
            audioProcessor.setPingPong(true);
        }
        else
        {
            audioProcessor.setPingPong(false);
        }
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TukTukyAudioProcessorEditor)
};
