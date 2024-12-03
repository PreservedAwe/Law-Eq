#pragma once
#include <JuceHeader.h>


struct CustomLookAndFeel : juce::LookAndFeel_V4
{
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider&) override 
    {
        auto bounds = juce::Rectangle<float>(x, y, width, height);

        g.setColour(juce::Colours::green);
        g.fillEllipse(bounds);
    };
};

