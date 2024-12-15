#pragma once
#include <JuceHeader.h>


struct CustomLookAndFeel : juce::LookAndFeel_V4
{
    juce::Image pluginWindowPng, sliderRotaryCirclePng, sliderButtonPng, sliderVerticalStickPng, sliderVerticalThumbPng, sliderRotaryThumbPng, toggleButtonOffPng, toggleButtonOnPng;

    CustomLookAndFeel()
    {
        sliderRotaryCirclePng = cropImage(juce::ImageCache::getFromMemory(BinaryData::sliderRotaryCircle_png, BinaryData::sliderRotaryCircle_pngSize));
        sliderVerticalStickPng = cropImage(juce::ImageCache::getFromMemory(BinaryData::sliderVerticalStick_png, BinaryData::sliderVerticalStick_pngSize));
        pluginWindowPng = cropImage(juce::ImageCache::getFromMemory(BinaryData::pluginWindow_png, BinaryData::pluginWindow_pngSize));
        sliderVerticalThumbPng = cropImage(juce::ImageCache::getFromMemory(BinaryData::sliderVerticalThumb_png, BinaryData::sliderVerticalThumb_pngSize));
        sliderRotaryThumbPng = cropImage(juce::ImageCache::getFromMemory(BinaryData::sliderRotaryThumb_png, BinaryData::sliderRotaryThumb_pngSize));
        toggleButtonOffPng = cropImage(juce::ImageCache::getFromMemory(BinaryData::toggleButtonOff_png, BinaryData::toggleButtonOff_pngSize));
        toggleButtonOnPng = cropImage(juce::ImageCache::getFromMemory(BinaryData::toggleButtonOn_png, BinaryData::toggleButtonOn_pngSize));
    }

    static juce::Image cropImage(const juce::Image& image)
    {
        juce::RectangleList<int> mask;
        image.createSolidAreaMask(mask, 0.1f);
        if(!mask.isEmpty())
        {
            return image.getClippedImage(mask.getBounds());
        }
        else
        {
            return image;
        }
    };

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override 
    {
        auto bounds = juce::Rectangle<float>(x, y, width, height);

        if (sliderRotaryCirclePng.isValid())
        {
            g.drawImage(sliderRotaryCirclePng, bounds, juce::RectanglePlacement::stretchToFit);
        }

        if (sliderRotaryThumbPng.isValid())
        {
            auto center = bounds.getCentre();
            juce::Rectangle<float> r;
            r.setLeft(center.getX() - 10);
            r.setRight(center.getX() + 10);
            r.setTop(center.getY() - bounds.getHeight() / 4.5);
            r.setBottom(center.getY());

            auto sliderAngRad = juce::jmap(sliderPosProportional, 0.f, 1.f, rotaryStartAngle, rotaryEndAngle);
            g.saveState();
            g.addTransform(juce::AffineTransform().rotated(sliderAngRad, center.getX(), center.getY()));
            g.drawImage(sliderRotaryThumbPng, r, juce::RectanglePlacement::stretchToFit);
            g.restoreState();
        }

        auto value = slider.getValue();
        auto text = juce::String::formatted("%.1f", value);
        g.setColour(juce::Colours::white);
        g.setFont(15.0f);
        g.drawFittedText(text, bounds.toNearestInt(), juce::Justification::centred, 1);
    };

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle sliderStyle, juce::Slider& slider) override
    {
        auto bounds = juce::Rectangle<float>(x, y, width, height);

        if (sliderVerticalStickPng.isValid())
        {
            g.drawImage(sliderVerticalStickPng, bounds, juce::RectanglePlacement::stretchToFit);
        }

        if (sliderVerticalThumbPng.isValid())
        {
            auto center = bounds.getCentre();
            juce::Rectangle<float> r;
            r.setWidth(bounds.getWidth() * 0.8f);
            r.setHeight(bounds.getHeight() * 0.1f);
            r.setY(juce::jmap(sliderPosProportional, 0.f, 1.f, bounds.getBottom() - r.getHeight(), bounds.getY()));
            r.setX(bounds.getCentreX() - r.getWidth() / 2.0f);

            g.drawImage(sliderVerticalThumbPng, r, juce::RectanglePlacement::stretchToFit);
        }

        auto value = slider.getValue();
        auto text = juce::String::formatted("%.1f", value);
        g.setColour(juce::Colours::white);
        g.setFont(15.0f);
        g.drawFittedText(text, bounds.toNearestInt(), juce::Justification::centred, 1);
    };

    void drawResizableFrame(juce::Graphics& g, int w, int h, const juce::BorderSize<int>& b) override
    {
        auto bounds = juce::Rectangle<float>(0, 0, w, h);
        if (pluginWindowPng.isValid())
        {
            g.drawImage(pluginWindowPng, bounds, juce::RectanglePlacement::stretchToFit);
        }
    };

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat();
        if (toggleButtonOnPng.isValid() && (shouldDrawButtonAsHighlighted == true))
        {
            g.drawImage(toggleButtonOnPng, bounds, juce::RectanglePlacement::stretchToFit);
        }
        else if (toggleButtonOffPng.isValid() && (shouldDrawButtonAsDown == true))
        {
            g.drawImage(toggleButtonOffPng, bounds, juce::RectanglePlacement::stretchToFit);
        }
    };
};



