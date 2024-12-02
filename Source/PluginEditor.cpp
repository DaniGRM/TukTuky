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
    delaySyncSlider(*audioProcessor.apvts.getParameter("Delay Sync")),
    feedbackSlider(*audioProcessor.apvts.getParameter("Feedback")),
    mixSlider(*audioProcessor.apvts.getParameter("Mix")),
    delaySliderAttachment(audioProcessor.apvts, "Delay", delaySlider),
    delaySyncSliderAttachment(audioProcessor.apvts, "Delay Sync", delaySyncSlider),
    feedbackSliderAttachment(audioProcessor.apvts, "Feedback", feedbackSlider),
    mixSliderAttachment(audioProcessor.apvts, "Mix", mixSlider)
{

    delaySlider.setMarks({"0.1s", "2s"});
    delaySyncSlider.setMarks({"1/16", "1/8", "1/6", "1/4", "1/3", "1/2", "1"});
    feedbackSlider.setMarks({"0", "1"});
    mixSlider.setMarks({"0", "1"});

    syncButton.setButtonText("Sync");
    pingPongButton.setButtonText("PingPong");
    syncButton.onClick = [this]() { syncClicked(); };
    pingPongButton.onClick = [this]() { pingPongClicked(); };
    // Make all comps visible
    for (auto* comp : getComps()) {
        addAndMakeVisible(comp);
    }

    delaySyncSlider.setVisible(false);
    // Set size
    setSize (600, 300);
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
    auto headerHeight = 40;

    // Header bounds
    auto headerBounds = bounds.removeFromTop(headerHeight);
    tukyHeader.setBounds(headerBounds);

    // Rest area splitted into three horizontal areas

    auto toggleHeight = 15.f;

    // AREA 1/3
    auto firstArea = bounds.removeFromLeft(bounds.getWidth() * 0.33);
    auto modeArea = firstArea.removeFromBottom(firstArea.getHeight() * 0.25);

    modeArea.removeFromLeft(toggleHeight).removeFromRight(toggleHeight);
    auto syncArea = modeArea.removeFromLeft(modeArea.getWidth() * 0.5);
    syncButton.setBounds(syncArea.withHeight(toggleHeight).withY(syncArea.getY() + (syncArea.getHeight() - toggleHeight) / 2));
    setLabel(delayLabel, "DELAY", firstArea.removeFromTop(firstArea.getHeight() * 0.3));
    delaySlider.setBounds(firstArea);
    delaySyncSlider.setBounds(firstArea);
    
    // AREA 2/3
    auto secondArea = bounds.removeFromLeft(bounds.getWidth() * 0.5);
    auto pArea = secondArea.removeFromBottom(secondArea.getHeight() * 0.25);

    pArea.removeFromLeft(toggleHeight).removeFromRight(toggleHeight);
    auto pingArea = pArea.removeFromLeft(pArea.getWidth() * 0.5);
    pingPongButton.setBounds(pingArea.withHeight(toggleHeight).withY(pingArea.getY() + (pingArea.getHeight() - toggleHeight) / 2));
    setLabel(feedbackLabel, "FEEDBACK", secondArea.removeFromTop(secondArea.getHeight() * 0.3));
    feedbackSlider.setBounds(secondArea);

    // AREA 3/3
    bounds.removeFromBottom(bounds.getHeight() * 0.25);
    setLabel(mixLabel, "MIX", bounds.removeFromTop(bounds.getHeight() * 0.3));
    mixSlider.setBounds(bounds);

}

// Function to get reference for each component
std::vector<juce::Component*> TukTukyAudioProcessorEditor::getComps()
{
    return
    {
        &tukyHeader,
        &delaySlider,
        &delaySyncSlider,
        &feedbackSlider,
        &mixSlider,
        &delayLabel,
        &feedbackLabel,
        &mixLabel,
        &syncButton,
        &pingPongButton,
    };
}

// Function to set label with given text, on given bounds at given Y
void TukTukyAudioProcessorEditor::setLabel(juce::Label& label, juce::String text, juce::Rectangle<int>bounds) {
    label.setBounds(bounds);
    label.setText(text, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, TukyUI::Colors::blue);
    label.setFont(TukyUI::Fonts::label);
}
