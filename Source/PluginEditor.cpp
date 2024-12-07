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
    lowCutSlider(*audioProcessor.apvts.getParameter("Low Freq")),
    highCutSlider(*audioProcessor.apvts.getParameter("High Freq")),
    mixFilterSlider(*audioProcessor.apvts.getParameter("Filter Mix")),
    mixSlider(*audioProcessor.apvts.getParameter("Mix")),
    gainSlider(*audioProcessor.apvts.getParameter("Mix")),
    delaySliderAttachment(audioProcessor.apvts, "Delay", delaySlider),
    delaySyncSliderAttachment(audioProcessor.apvts, "Delay Sync", delaySyncSlider),
    feedbackSliderAttachment(audioProcessor.apvts, "Feedback", feedbackSlider),
    mixSliderAttachment(audioProcessor.apvts, "Mix", mixSlider),
    lowCutSliderAttachment(audioProcessor.apvts, "Low Freq", lowCutSlider),
    highCutSliderAttachment(audioProcessor.apvts, "High Freq", highCutSlider),
    mixFilterSliderAttachment(audioProcessor.apvts, "Filter Mix", mixFilterSlider)
{

    delaySlider.setMarks({"0.1s", "2s"});
    delaySyncSlider.setMarks({"1/16", "1/8", "1/6", "1/4", "1/3", "1/2", "1"});
    feedbackSlider.setMarks({"0", "1"});
    mixSlider.setMarks({"0", "1"});
    gainSlider.setMarks({"0", "1"});

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

    auto bounds = getLocalBounds();

    auto firstArea = bounds.removeFromLeft(bounds.getWidth() * 0.33);


    auto secondArea = bounds.removeFromLeft(bounds.getWidth() * 0.5);

    g.setColour(TukyUI::Colors::bluegray);
    g.drawLine(static_cast<float>(secondArea.getX()),
        static_cast<float>(secondArea.getY() + 50.f),
        static_cast<float>(secondArea.getX()),
        static_cast<float>(secondArea.getBottom() - 10.f), 0.5f);

    g.drawLine(static_cast<float>(bounds.getX()),
        static_cast<float>(bounds.getY() + 50.f),
        static_cast<float>(bounds.getX()),
        static_cast<float>(bounds.getBottom() - 10.f), 0.5f);
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
    auto toggleHeight = 14.f;

    // AREA 1/3
    auto firstArea = bounds.removeFromLeft(bounds.getWidth() * 0.33);

    auto modeArea = firstArea.removeFromBottom(firstArea.getHeight() * 0.2);

    modeArea.removeFromLeft(toggleHeight).removeFromRight(toggleHeight);
    auto syncArea = modeArea.removeFromLeft(modeArea.getWidth() * 0.5);
    syncButton.setBounds(syncArea.removeFromLeft(syncArea.getWidth()).withHeight(toggleHeight).withY(syncArea.getY() + (syncArea.getHeight() - toggleHeight) / 2));
    pingPongButton.setBounds(modeArea.removeFromLeft(modeArea.getWidth()).withHeight(toggleHeight).withY(syncArea.getY() + (syncArea.getHeight() - toggleHeight) / 2));
    auto feedbackArea = firstArea.removeFromBottom(firstArea.getHeight() * 0.5);
    feedbackSlider.setBounds(feedbackArea.removeFromTop(feedbackArea.getHeight() * 0.88));
    setLabel(feedbackLabel, "FEEDBACK", feedbackArea);

    auto delayArea = firstArea.removeFromTop(firstArea.getHeight() * 0.88);
    delaySlider.setBounds(delayArea);
    delaySyncSlider.setBounds(delayArea);
    setLabel(delayLabel, "DELAY", firstArea);



    // AREA 2/3
    auto secondArea = bounds.removeFromLeft(bounds.getWidth() * 0.5);

    secondArea.removeFromBottom(secondArea.getHeight() * 0.1);
    secondArea.removeFromTop(secondArea.getHeight() * 0.05);


    auto lowArea = secondArea.removeFromTop(secondArea.getHeight() * 0.33f);
    auto highArea = secondArea.removeFromTop(secondArea.getHeight() * 0.5f);


    auto lowLabelArea = lowArea.removeFromRight(lowArea.getWidth() * 0.5f);
    auto highLabelArea = highArea.removeFromLeft(highArea.getWidth() * 0.5f);
    auto filterLabelArea = secondArea.removeFromRight(secondArea.getWidth() * 0.5f);
    /*setLabel(lowCutLabel, "LOW CUT", lowLabelArea.withY(lowLabelArea.getY() - lowLabelArea.getHeight() * 0.5f + 7.f));
    setLabel(highCutLabel, "HIGH CUT", highLabelArea.withY(highLabelArea.getY() - highLabelArea.getHeight() * 0.5f + 7.f));
    setLabel(mixFilterLabel, "FILTER", filterLabelArea.withY(filterLabelArea.getY() -  filterLabelArea.getHeight() * 0.5f + 7.f));*/


    lowCutSlider.setBounds(lowArea);
    highCutSlider.setBounds(highArea);
    mixFilterSlider.setBounds(secondArea);
    //auto pArea = secondArea.removeFromBottom(secondArea.getHeight() * 0.25);

    //pArea.removeFromLeft(toggleHeight).removeFromRight(toggleHeight);
    //auto pingArea = pArea.removeFromLeft(pArea.getWidth() * 0.5);
    //pingPongButton.setBounds(pingArea.withHeight(toggleHeight).withY(pingArea.getY() + (pingArea.getHeight() - toggleHeight) / 2));
    //setLabel(feedbackLabel, "FEEDBACK", secondArea.removeFromTop(secondArea.getHeight() * 0.3));
    //feedbackSlider.setBounds(secondArea);

    // AREA 3/3
    bounds.removeFromBottom(bounds.getHeight() * 0.2);

    auto gainArea = bounds.removeFromBottom(bounds.getHeight() * 0.5);
    gainSlider.setBounds(gainArea.removeFromTop(gainArea.getHeight() * 0.88));
    setLabel(gainLabel, "GAIN", gainArea);

    auto mixArea = bounds.removeFromTop(bounds.getHeight() * 0.88);
    mixSlider.setBounds(mixArea);
    setLabel(mixLabel, "MIX", bounds);



    //bounds.removeFromBottom(bounds.getHeight() * 0.25);
    //setLabel(mixLabel, "MIX", bounds.removeFromTop(bounds.getHeight() * 0.3));
    //mixSlider.setBounds(bounds);

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
        &lowCutSlider,
        &highCutSlider,
        &mixFilterSlider,
        &mixSlider,
        &gainSlider ,
        &delayLabel,
        &feedbackLabel,
        &mixLabel,
        &lowCutLabel,
        &highCutLabel,
        &mixFilterLabel,
        &gainLabel,
        &syncButton,
        &pingPongButton,
    };
}

// Function to set label with given text, on given bounds at given Y
void TukTukyAudioProcessorEditor::setLabel(juce::Label& label, juce::String text, juce::Rectangle<int>bounds) {
    label.setBounds(bounds);
    label.setText(text, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centredBottom);
    label.setColour(juce::Label::textColourId, TukyUI::Colors::blue);
    label.setFont(TukyUI::Fonts::smLabel);
}
