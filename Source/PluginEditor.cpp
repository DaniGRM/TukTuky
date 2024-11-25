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
    delaySlider(*audioProcessor.apvts.getParameter("Delay")),
    feedbackSlider(*audioProcessor.apvts.getParameter("Feedback")),
    mixSlider(*audioProcessor.apvts.getParameter("Mix")),
    delaySliderAttachment(audioProcessor.apvts, "Delay", delaySlider),
    feedbackSliderAttachment(audioProcessor.apvts, "Feedback", feedbackSlider),
    mixSliderAttachment(audioProcessor.apvts, "Mix", mixSlider)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    for (auto* comp : getComps()) {
        addAndMakeVisible(comp);
    }
    setSize (600, 400);
}

TukTukyAudioProcessorEditor::~TukTukyAudioProcessorEditor()
{
}

//==============================================================================
void TukTukyAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    using namespace juce;
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(TukyUI::Colors::background);
}

void TukTukyAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds();

    auto headerHeight = bounds.getHeight() * 0.1;

    auto headerBounds = bounds.removeFromTop(headerHeight);
    tukyHeader.setBounds(headerBounds);

    auto header = bounds.removeFromTop(bounds.getHeight() * 0.33);

    header.removeFromLeft(header.getWidth() * 0.33);
    header.removeFromRight(header.getWidth() * 0.5);

    auto firstArea = bounds.removeFromLeft(bounds.getWidth() * 0.33);
    auto secondArea = bounds.removeFromLeft(bounds.getWidth() * 0.5);


    auto delayLabelArea =  firstArea.removeFromBottom(firstArea.getHeight() * 0.5);

    auto feedbackLabelArea = secondArea.removeFromBottom(secondArea.getHeight() * 0.5);

    auto mixLabelArea = bounds.removeFromBottom(bounds.getHeight() * 0.5);

    delaySlider.setBounds(
        firstArea
    );

    feedbackSlider.setBounds(
        secondArea
    );
    mixSlider.setBounds(
        bounds
    );

    int labelHeight = 30;

    delayLabel.setBounds(delayLabelArea.withHeight(labelHeight).withY(firstArea.getBottom() - 20));
    delayLabel.setText("DELAY", juce::dontSendNotification);
    delayLabel.setJustificationType(juce::Justification::centred);
    delayLabel.setColour(juce::Label::textColourId, TukyUI::Colors::blue); // Cambiar color del textoo
    delayLabel.setFont(TukyUI::Fonts::label); // Cambiar fuente del texto

    feedbackLabel.setBounds(feedbackLabelArea.withHeight(labelHeight).withY(secondArea.getBottom() - 20));
    feedbackLabel.setText("FEEDBACK", juce::dontSendNotification);
    feedbackLabel.setJustificationType(juce::Justification::centred);
    feedbackLabel.setColour(juce::Label::textColourId, TukyUI::Colors::blue); // Cambiar color del textoo
    feedbackLabel.setFont(TukyUI::Fonts::label); // Cambiar fuente del texto

    mixLabel.setBounds(mixLabelArea.withHeight(labelHeight).withY(bounds.getBottom() - 20));
    mixLabel.setText("MIX", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setColour(juce::Label::textColourId, TukyUI::Colors::blue); // Cambiar color del textoo
    mixLabel.setFont(TukyUI::Fonts::label); // Cambiar fuente del texto


    titleLabel.setBounds(header);
    titleLabel.setText("TUKTUKY", juce::dontSendNotification);
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setColour(juce::Label::textColourId, TukyUI::Colors::blue);
    titleLabel.setFont(TukyUI::Fonts::title);

}

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
        &mixLabel,
        &titleLabel
    };
}

