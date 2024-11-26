/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================
TukTukyAudioProcessorEditor::TukTukyAudioProcessorEditor (TukTukyAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    tukyHeader("TukTuky"),
    delaySlider(*audioProcessor.apvts.getParameter("Delay")),
    feedbackSlider(*audioProcessor.apvts.getParameter("Feedback")),
    mixSlider(*audioProcessor.apvts.getParameter("Mix")),
    delaySliderAttachment(audioProcessor.apvts, "Delay", delaySlider),
    feedbackSliderAttachment(audioProcessor.apvts, "Feedback", feedbackSlider),
    mixSliderAttachment(audioProcessor.apvts, "Mix", mixSlider)
{

    // Make all comps visible
    for (auto* comp : getComps()) {
        addAndMakeVisible(comp);
    }
    // Set size
    setSize (600, 400);
}

TukTukyAudioProcessorEditor::~TukTukyAudioProcessorEditor()
{
}

//==============================================================================
void TukTukyAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Fill with background color
    using namespace juce;
    g.fillAll(TukyUI::Colors::background);
}

void TukTukyAudioProcessorEditor::resized()
{
    // Total bounds
    auto bounds = getLocalBounds();

    // Header height
    auto headerHeight = bounds.getHeight() * 0.1;

    // Header bounds
    auto headerBounds = bounds.removeFromTop(headerHeight);
    tukyHeader.setBounds(headerBounds);

    int labelHeight = 30;
    // Rest area splitted into three horizontal areas
    bounds.removeFromTop(bounds.getHeight() * 0.33 - labelHeight);
    auto firstArea = bounds.removeFromLeft(bounds.getWidth() * 0.33);
    auto secondArea = bounds.removeFromLeft(bounds.getWidth() * 0.5);

    // Set bounds for each slider
    delaySlider.setBounds(firstArea);
    feedbackSlider.setBounds(secondArea);
    mixSlider.setBounds(bounds);

    auto labelY = firstArea.getY() + delaySlider.getSliderBounds().getHeight() + labelHeight;

    // Labels setted under each slider
    setLabel(delayLabel, "DELAY", firstArea, labelHeight, labelY);
    setLabel(feedbackLabel, "FEEDBACK", secondArea, labelHeight, labelY);
    setLabel(mixLabel, "MIX", bounds, labelHeight, labelY);

}

// Function to get reference for each component
std::vector<juce::Component*> TukTukyAudioProcessorEditor::getComps()
{
    return
    {
        &tukyHeader,
        &delaySlider,
        &feedbackSlider,
        &mixSlider,
        &delayLabel,
        &feedbackLabel,
        &mixLabel
    };
}

// Function to set label with given text, on given bounds at given Y
void TukTukyAudioProcessorEditor::setLabel(juce::Label& label, juce::String text, juce::Rectangle<int>bounds, int labelHeight, int labelY) {
    label.setBounds(bounds.withHeight(labelHeight).withY(labelY));
    label.setText(text, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, TukyUI::Colors::blue);
    label.setFont(TukyUI::Fonts::label);
}
