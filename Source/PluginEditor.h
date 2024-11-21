/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


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

    juce::Slider lowCutSlider;
    juce::Slider midChangeSlider;
    juce::Slider highCutSlider;
    juce::Label lowCutLabel;
    juce::Label midChangeLabel;
    juce::Label highCutLabel;
    void setupLowSlider(juce::Slider& slider, juce::Label& label);
    void setupHighSlider(juce::Slider& slider, juce::Label& label);
    void setupMidSlider(juce::Slider& slider, juce::Label& label);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LaweqAudioProcessorEditor)
};
