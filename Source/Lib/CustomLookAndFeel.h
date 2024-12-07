#pragma once
#include <JuceHeader.h>


struct CustomLookAndFeel : juce::LookAndFeel_V4
{
    juce::Image sliderCirclePng, sliderButtonPng;

    CustomLookAndFeel()
    {
        //juce::File sourceFile = juce::File(__FILE__);
        //juce::File projectFolder = sourceFile.getParentDirectory().getParentDirectory().getParentDirectory();

        sliderCirclePng = cropImage(juce::ImageCache::getFromMemory(BinaryData::sliderCircle_png, BinaryData::sliderCircle_pngSize));
        sliderButtonPng = cropImage(juce::ImageCache::getFromMemory(BinaryData::sliderButton_png, BinaryData::sliderButton_pngSize));
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

        if (sliderCirclePng.isValid())
        {
            g.drawImage(sliderCirclePng, bounds, juce::RectanglePlacement::stretchToFit);
        }

        if (sliderButtonPng.isValid())
        {
            float angle = juce::jmap(sliderPosProportional, 0.0f, 1.0f, rotaryStartAngle, rotaryEndAngle);
            float buttonScale = 0.3f;
            float buttonWidth = width * buttonScale;
            float buttonHeight = height * buttonScale;
            //auto buttonBounds = bounds.reduced(bounds.getWidth() * (1 - buttonScale) / 2);
            auto buttonBounds = juce::Rectangle<float>(
                bounds.getCentreX() - (buttonWidth / 2),
                bounds.getCentreY() - (buttonHeight / 2),
                buttonWidth,
                buttonHeight
            );
            auto transform = juce::AffineTransform::rotation(angle, bounds.getCentreX(), bounds.getCentreY());
            
            g.saveState();
            g.addTransform(transform);
            g.drawImage(sliderButtonPng, buttonBounds, juce::RectanglePlacement::stretchToFit);
            g.restoreState();
        }
        //g.setColour(juce::Colours::green);
        //g.fillEllipse(buttonBounds);
    };
};

