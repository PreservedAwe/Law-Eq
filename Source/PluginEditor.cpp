/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LaweqAudioProcessorEditor::LaweqAudioProcessorEditor (LaweqAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);

    //Setup Each GUI object
    setupLowSlider(lowCutSlider, lowCutLabel);
    setupMidSlider(midChangeSlider, midChangeLabel);
    setupHighSlider(highCutSlider, highCutLabel);
        
}

LaweqAudioProcessorEditor::~LaweqAudioProcessorEditor()
{
}

//==============================================================================
void LaweqAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Law Eq!", getLocalBounds(), juce::Justification::centred, 1);
}

void LaweqAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    float availableWidth = getWidth(); // Subtracting 20px for left/right padding
    float availableHeight = getHeight() - 60; // Subtracting 60px for top/bottom padding
    float sliderWidth = 40;
    float yPosition = 40;
    float gapBetweenSliders = 20;
    int numOfSliders = 3;

    float totalSliderWidth = (sliderWidth * numOfSliders) + (gapBetweenSliders * (numOfSliders - 1));
    float leftOffset = availableWidth / numOfSliders;

    lowCutSlider.setBounds(leftOffset, yPosition, sliderWidth, availableHeight);
    highCutSlider.setBounds((leftOffset + (sliderWidth * 2) + (gapBetweenSliders * 2)), yPosition, sliderWidth, availableHeight);
    midChangeSlider.setBounds((leftOffset + sliderWidth + gapBetweenSliders), yPosition, sliderWidth, availableHeight);
}

void LaweqAudioProcessorEditor::setupLowSlider(juce::Slider& slider, juce::Label& label)
{
    slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    slider.setRange(0.0, 127.0, 1.0);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 90, 0);
    slider.setTextValueSuffix("Hz");
    slider.setValue(90.0);
    addAndMakeVisible(&slider);
    label.setText("Low Cut", juce::dontSendNotification);
    label.setFont(juce::Font(14.0f));
    label.attachToComponent(&slider, false);
    addAndMakeVisible(&label);
}

void LaweqAudioProcessorEditor::setupHighSlider(juce::Slider& slider, juce::Label& label)
{
    slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    slider.setRange(0.0, 127.0, 1.0);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 90, 0);
    slider.setTextValueSuffix("Hz");
    slider.setValue(30.0);
    addAndMakeVisible(&slider);
    label.setText("High Cut", juce::dontSendNotification);
    label.attachToComponent(&slider, false);
    addAndMakeVisible(&label);
}

void LaweqAudioProcessorEditor::setupMidSlider(juce::Slider& slider, juce::Label& label)
{
    slider.setSliderStyle(juce::Slider::LinearBarVertical);
    slider.setRange(0.0, 127.0, 1.0);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 90, 0);
    slider.setTextValueSuffix("Hz");
    slider.setValue(1.0);
    addAndMakeVisible(&slider);
    label.setText("Mids", juce::dontSendNotification);
    label.attachToComponent(&slider, false);
    addAndMakeVisible(&label);
}