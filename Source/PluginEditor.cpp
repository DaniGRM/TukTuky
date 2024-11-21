/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
void LookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPosProportional, float rotaryStartAngle,
    float rotaryEndAngle, juce::Slider& slider)
{
    using namespace juce;

    auto bounds = Rectangle<float>(x, y, width, height);

    g.setColour(Colours::whitesmoke);
    g.fillEllipse(bounds);

    g.setColour(Colour(24u, 45u, 100u));
    g.drawEllipse(bounds, 2.f);

    auto center = bounds.getCentre();
    Path p;

    Rectangle<float> r;
    r.setLeft(center.getX() - 4);
    r.setRight(center.getX() + 4);
    r.setTop(bounds.getY() + 4);
    r.setBottom(bounds.getY() + 12);

    p.addEllipse(r);

    jassert(rotaryStartAngle < rotaryEndAngle);

    auto sliderAngRad = jmap(sliderPosProportional, 0.f, 1.f, rotaryStartAngle, rotaryEndAngle);
    p.applyTransform(AffineTransform().rotated(sliderAngRad, center.getX(), center.getY()));

    g.fillPath(p);
}

void RotarySliderWithLabels::paint(juce::Graphics& g) {
    using namespace juce;
    auto startAng = degreesToRadians(180.f + 45.f);
    auto endAng = degreesToRadians(180.f - 45.f) + MathConstants<float>::twoPi;

    auto range = getRange();
    auto sliderBounds = getSliderBounds();

    getLookAndFeel().drawRotarySlider(g,
        sliderBounds.getX(),
        sliderBounds.getY(),
        sliderBounds.getWidth(),
        sliderBounds.getHeight(),
        jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0),
        startAng,
        endAng,
        *this);
}

juce::Rectangle<int> RotarySliderWithLabels::getSliderBounds() const {

    auto bounds = getLocalBounds();

    auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());

    size -= getTextHeight() * 2;

    juce::Rectangle<int> r;
    r.setSize(size, size);
    r.setCentre(bounds.getCentreX(), 0);
    r.setY(2);

    return r;
}
//==============================================================================
TukTukyAudioProcessorEditor::TukTukyAudioProcessorEditor (TukTukyAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    delaySlider(*audioProcessor.apvts.getParameter("Delay"), "s"),
    feedbackSlider(*audioProcessor.apvts.getParameter("Feedback"), ""),
    mixSlider(*audioProcessor.apvts.getParameter("Mix"), ""),
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
    g.fillAll(Colours::whitesmoke);
}

void TukTukyAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds();
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
    delayLabel.setColour(juce::Label::textColourId, juce::Colour(24u, 45u, 100u)); // Cambiar color del texto

    feedbackLabel.setBounds(feedbackLabelArea.withHeight(labelHeight).withY(secondArea.getBottom() - 20));
    feedbackLabel.setText("FEEDBACK", juce::dontSendNotification);
    feedbackLabel.setJustificationType(juce::Justification::centred);
    feedbackLabel.setColour(juce::Label::textColourId, juce::Colour(24u, 45u, 100u));

    mixLabel.setBounds(mixLabelArea.withHeight(labelHeight).withY(bounds.getBottom() - 20));
    mixLabel.setText("MIX", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setColour(juce::Label::textColourId, juce::Colour(24u, 45u, 100u));


    titleLabel.setBounds(header);
    titleLabel.setText("TukTuky", juce::dontSendNotification);
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setColour(juce::Label::textColourId, juce::Colour(24u, 45u, 100u));
    titleLabel.setFont(juce::Font(24.f, juce::Font::bold));

}

std::vector<juce::Component*> TukTukyAudioProcessorEditor::getComps()
{
    return
    {
        &delaySlider,
        &feedbackSlider,
        &mixSlider,
        &delayLabel,
        &feedbackLabel,
        &mixLabel,
        &titleLabel
    };
}

