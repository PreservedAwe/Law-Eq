/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "./Lib/CustomLookAndFeel.h"
#include "./Lib/CustomGUI.h"

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

    std::unique_ptr<CustomLookAndFeel> customLnf;
    CustomRotarySlider highPassSlider, lowPassSlider, allGainSlider;
    CustomVerticalSlider midGainSlider;
    CustomToggleButton midGainToggle, highPassToggle, lowPassToggle;
    CustomLabel highPassLabel, midGainLabel, lowPassLabel, allGainLabel, midGainToggleLabel, highPassToggleLabel, lowPassToggleLabel, pluginWindowLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowPassAttachment, highPassAttachment, midGainAttachment, allGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lpToggleAttachment, hpToggleAttachment, mgToggleAttachment;
    void getAllComponents();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LaweqAudioProcessorEditor)
};
