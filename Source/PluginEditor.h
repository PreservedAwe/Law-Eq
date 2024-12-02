/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//---------------------Custom GUI Components--------------------------------------------------
struct CustomRotarySlider : juce::Slider
{
    CustomRotarySlider() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
        juce::Slider::TextEntryBoxPosition::NoTextBox)
    {

    }
};

struct CustomVerticalSlider : juce::Slider
{
    CustomVerticalSlider() : juce::Slider(juce::Slider::SliderStyle::LinearBarVertical,
        juce::Slider::TextEntryBoxPosition::NoTextBox)
    {

    }
};

//==============================================================================
/**
*/
class LaweqAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    LaweqAudioProcessorEditor (LaweqAudioProcessor&);
    ~LaweqAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LaweqAudioProcessor& audioProcessor;

    CustomRotarySlider highPassSlider, lowPassSlider;
    CustomVerticalSlider midGainSlider;
    juce::Label highPassLabel, midGainLabel, lowPassLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowPassAttachment, highPassAttachment, midGainAttachment;

    void getAllComponents();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LaweqAudioProcessorEditor)
};
