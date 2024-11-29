#pragma once
#include <JuceHeader.h>

class LmhFilter
{
    public:
        void processBlock(juce::AudioBuffer<float>& buffer, float samplingRate);
    private:
        std::vector<float> dnBuffer;
};

