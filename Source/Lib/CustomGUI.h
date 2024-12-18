#pragma once
#include <JuceHeader.h>

//---------------------Custom GUI Components--------------------------------------------------

struct CustomRotarySlider : juce::Slider
{
    CustomRotarySlider(CustomLookAndFeel* customLnf) : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
        juce::Slider::TextEntryBoxPosition::NoTextBox)
    {
        setLookAndFeel(customLnf);
    }

    ~CustomRotarySlider()
    {
        setLookAndFeel(nullptr);
    }

    void paint(juce::Graphics& g) override
    {
        auto startAngle = juce::degreesToRadians(180.f + 45.f);
        auto endAngle = juce::degreesToRadians(180.f - 45.f + (360.f));
        auto range = getRange();
        auto sliderProportional = juce::jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0);
        auto sliderBounds = getLocalBounds();

        getLookAndFeel().drawRotarySlider(g, sliderBounds.getX(), sliderBounds.getY(), sliderBounds.getWidth(), sliderBounds.getHeight(), sliderProportional, startAngle, endAngle, *this);
    };
};

struct CustomVerticalSlider : juce::Slider
{
    CustomVerticalSlider(CustomLookAndFeel* customLnf) : juce::Slider(juce::Slider::SliderStyle::LinearBarVertical,
        juce::Slider::TextEntryBoxPosition::NoTextBox)
    {
        setLookAndFeel(customLnf);
    }

    ~CustomVerticalSlider()
    {
        setLookAndFeel(nullptr);
    }

    void paint(juce::Graphics& g) override
    {
        auto range = getRange();
        auto sliderProportional = juce::jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0);
        auto sliderBounds = getLocalBounds();

        getLookAndFeel().drawLinearSlider(g, sliderBounds.getX(), sliderBounds.getY(), sliderBounds.getWidth(), sliderBounds.getHeight(), sliderProportional, 0.f, 1.f, juce::Slider::SliderStyle::LinearBarVertical, *this);
    };
};

struct CustomToggleButton : juce::ToggleButton
{
    CustomToggleButton(CustomLookAndFeel* customLnf) : juce::ToggleButton()
    {
        setLookAndFeel(customLnf);
    }

    ~CustomToggleButton()
    {
        setLookAndFeel(nullptr);
    }

    void paint(juce::Graphics& g) override
    {
        getLookAndFeel().drawToggleButton(g, *this, (this->getToggleState() == true), (this->getToggleState() == false));
    }
};

struct CustomLabel : juce::Label
{
    CustomLabel(CustomLookAndFeel* customLnf) : juce::Label()
    {
        setLookAndFeel(customLnf);
    }

    ~CustomLabel()
    {
        setLookAndFeel(nullptr);
    }

    void paint(juce::Graphics& g) override
    {
        getLookAndFeel().drawLabel(g, *this);
    }

    void resized() override
    {
        auto* component = getAttachedComponent();
        if (component != nullptr)
        {
            auto componentBounds = component->getBounds().reduced(10);
            setBounds(componentBounds.getX(), componentBounds.getY() - 25, componentBounds.getWidth(), 30);
        }
    }
};