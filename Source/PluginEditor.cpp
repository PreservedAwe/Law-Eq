/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LaweqAudioProcessorEditor::LaweqAudioProcessorEditor (LaweqAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    customLnf(std::make_unique<CustomLookAndFeel>()),
    highPassSlider(customLnf.get()), lowPassSlider(customLnf.get()), midGainSlider(customLnf.get()), allGainSlider(customLnf.get()),
    highPassToggle(customLnf.get()), lowPassToggle(customLnf.get()), midGainToggle(customLnf.get()),
    highPassLabel(customLnf.get()), midGainLabel(customLnf.get()), lowPassLabel(customLnf.get()), allGainLabel(customLnf.get()), midGainToggleLabel(customLnf.get()), highPassToggleLabel(customLnf.get()), lowPassToggleLabel(customLnf.get()), pluginWindowLabel(customLnf.get())
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
    auto padding = 0.05f;
    bounds = bounds.reduced(bounds.getWidth() * padding, bounds.getHeight() * padding);

    //Top and Bottom
    auto topMain = bounds.removeFromTop(bounds.getHeight() * 0.20);
    auto bottomMain = bounds.removeFromBottom(bounds.getHeight() * 0.25);

    //Top Left 
    auto leftTArea = topMain.removeFromLeft(topMain.getWidth() * 0.33);

    //Top Right
    auto rightTArea = topMain.removeFromRight(topMain.getWidth() * 0.5);

    //Top Middle
    auto middleTArea = topMain;

    //Bottom Left 
    auto leftBArea = bottomMain.removeFromLeft(bottomMain.getWidth() * 0.33);
    auto leftBSubArea = leftBArea.removeFromLeft(leftBArea.getWidth() * 0.33);
    auto rightBSubArea = leftBArea.removeFromRight(leftBArea.getWidth() * 0.5);
    auto middleBSubArea = leftBArea;

    //Bottom Right
    auto rightBArea = bottomMain.removeFromRight(bottomMain.getWidth() * 0.5);

    //Bottom Middle
    auto middleBArea = bottomMain;

    //Middle Area
    auto leftArea = bounds.removeFromLeft(bounds.getWidth() * 0.33);
    auto rightArea = bounds.removeFromRight(bounds.getWidth() * 0.5);
    auto middleArea = bounds;


    lowPassSlider.setBounds(rightArea);
    highPassSlider.setBounds(leftArea);
    midGainSlider.setBounds(middleArea);
    allGainSlider.setBounds(rightBArea);
    lowPassToggle.setBounds(rightBSubArea);
    midGainToggle.setBounds(middleBSubArea);
    highPassToggle.setBounds(leftBSubArea);
    pluginWindowLabel.setBounds(middleTArea);
}

void LaweqAudioProcessorEditor::getAllComponents()
{
    std::vector<CustomToggleButton*> buttons = { &lowPassToggle, &highPassToggle, &midGainToggle };

    for (auto* b : buttons)
    {
        b->setClickingTogglesState(true);
        b->onClick = [b] { b->repaint(); };
    }

    highPassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*audioProcessor.parameters, "highPass", highPassSlider);
    lowPassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*audioProcessor.parameters, "lowPass", lowPassSlider);
    midGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*audioProcessor.parameters, "midGain", midGainSlider);
    allGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*audioProcessor.parameters, "allGain", allGainSlider);
    lpToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(*audioProcessor.parameters, "lpToggle", lowPassToggle);
    hpToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(*audioProcessor.parameters, "hpToggle", highPassToggle);
    mgToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(*audioProcessor.parameters, "mgToggle", midGainToggle);

    highPassLabel.setText("High Pass", juce::sendNotification);
    highPassLabel.attachToComponent(&highPassSlider, false);
    lowPassLabel.setText("Low Pass", juce::sendNotification);
    lowPassLabel.attachToComponent(&lowPassSlider, false);
    midGainLabel.setText("Mids", juce::sendNotification);
    midGainLabel.attachToComponent(&midGainSlider, false);
    allGainLabel.setText("Gain", juce::sendNotification);
    allGainLabel.attachToComponent(&allGainSlider, false);
    highPassToggleLabel.setText("HP", juce::sendNotification);
    highPassToggleLabel.attachToComponent(&highPassToggle, false);
    lowPassToggleLabel.setText("LP", juce::sendNotification);
    lowPassToggleLabel.attachToComponent(&lowPassToggle, false);
    midGainToggleLabel.setText("M", juce::sendNotification);
    midGainToggleLabel.attachToComponent(&midGainToggle, false);
    pluginWindowLabel.setText("LAW-EQ", juce::sendNotification);

    std::vector<juce::Component*> components =
    {
        &highPassSlider,
        &lowPassSlider,
        &midGainSlider,
        &allGainSlider,
        &highPassToggle,
        &lowPassToggle,
        &midGainToggle,
        &highPassLabel,
        &midGainLabel,
        &lowPassLabel,
        &allGainLabel,
        &midGainToggleLabel,
        &lowPassToggleLabel,
        &highPassToggleLabel,
        &pluginWindowLabel,
    };

    for (auto* comp : components)
    {
        addAndMakeVisible(comp);
    }

}