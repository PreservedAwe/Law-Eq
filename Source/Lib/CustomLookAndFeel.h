#pragma once
#include <JuceHeader.h>


struct CustomLookAndFeel : juce::LookAndFeel_V4
{
    juce::Image pluginWindowPng, sliderRotaryCirclePng, sliderButtonPng, sliderVerticalStickPng;

    CustomLookAndFeel()
    {
        sliderRotaryCirclePng = cropImage(juce::ImageCache::getFromMemory(BinaryData::sliderRotaryCircle_png, BinaryData::sliderRotaryCircle_pngSize));
        sliderVerticalStickPng = cropImage(juce::ImageCache::getFromMemory(BinaryData::sliderVerticalStick_png, BinaryData::sliderVerticalStick_pngSize));
        pluginWindowPng = cropImage(juce::ImageCache::getFromMemory(BinaryData::pluginWindow_png, BinaryData::pluginWindow_pngSize));
    }

    static juce::Image cropImage(const juce::Image& image)
    {
        juce::RectangleList<int> mask;
        image.createSolidAreaMask(mask, 0.01f);
        if(!mask.isEmpty())
        {
            return image.getClippedImage(mask.getBounds());
        }
        else
        {
            return image;
        }
    };

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider&) override 
    {
        auto bounds = juce::Rectangle<float>(x, y, width, height);

        if (sliderRotaryCirclePng.isValid())
        {
            g.drawImage(sliderRotaryCirclePng, bounds, juce::RectanglePlacement::stretchToFit);
        }

        if (5 < 1)
        {
            float angle = juce::jmap(sliderPosProportional, 0.0f, 1.0f, rotaryStartAngle, rotaryEndAngle);
            float buttonScale = 0.3f;
            float buttonWidth = width * buttonScale;
            float buttonHeight = height * buttonScale;
            auto buttonBounds = juce::Rectangle<float>(
                bounds.getCentreX() - (buttonWidth / 2),
                bounds.getCentreY() - (buttonHeight / 2),
                buttonWidth,
                buttonHeight
            );
            auto transform = juce::AffineTransform::rotation(angle, bounds.getCentreX(), bounds.getCentreY());
            
            g.saveState();
            g.addTransform(transform);
            g.drawImage(sliderRotaryCirclePng, buttonBounds, juce::RectanglePlacement::stretchToFit);
            g.restoreState();
        }
    };

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle sliderStyle, juce::Slider&) override
    {
        auto bounds = juce::Rectangle<float>(x, y, width, height);

        if (sliderVerticalStickPng.isValid())
        {
            g.drawImage(sliderVerticalStickPng, bounds, juce::RectanglePlacement::stretchToFit);
        }
    };

    void drawResizableFrame(juce::Graphics& g, int w, int h, const juce::BorderSize<int>& b) override
    {
        auto bounds = juce::Rectangle<float>(0, 0, w, h);
        if (pluginWindowPng.isValid())
        {
            g.drawImage(pluginWindowPng, bounds, juce::RectanglePlacement::stretchToFit);
        }
    };
};

