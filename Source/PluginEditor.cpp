/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LaweqAudioProcessorEditor::LaweqAudioProcessorEditor (LaweqAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), customLnf(std::make_unique<CustomLookAndFeel>()),  highPassSlider(customLnf.get()), lowPassSlider(customLnf.get()), midGainSlider(customLnf.get()), allGainSlider(customLnf.get())
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    //setSize(400, 300);
    setSize(700, 400);
    setLookAndFeel(customLnf.get());

    //Setup Each GUI object
    getAllComponents();
        
}

LaweqAudioProcessorEditor::~LaweqAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void LaweqAudioProcessorEditor::paint (juce::Graphics& g)
{
    getLookAndFeel().drawResizableFrame(g, getWidth(), getHeight(), juce::BorderSize<int>());
}

void LaweqAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds();
    auto leftArea = bounds.removeFromLeft(bounds.getWidth() * 0.33);
    auto rightArea = bounds.removeFromRight(bounds.getWidth() * 0.5);
    auto middleArea = bounds;
    lowPassSlider.setBounds(rightArea);
    highPassSlider.setBounds(leftArea);
    midGainSlider.setBounds(middleArea);
}

void LaweqAudioProcessorEditor::getAllComponents()
{
    std::vector<juce::Component*> components = 
    {
        &highPassSlider,
        &lowPassSlider,
        &midGainSlider,
        &highPassLabel,
        &midGainLabel,
        &lowPassLabel
    };

    for (auto* comp : components)
    {
        addAndMakeVisible(comp);
    }

    highPassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*audioProcessor.parameters, "highPass", highPassSlider);
    lowPassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*audioProcessor.parameters, "lowPass", lowPassSlider);
    midGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*audioProcessor.parameters, "midGain", midGainSlider);
}