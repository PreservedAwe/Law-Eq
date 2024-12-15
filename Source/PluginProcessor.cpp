/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LaweqAudioProcessor::LaweqAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    setupAllParameters();
}

LaweqAudioProcessor::~LaweqAudioProcessor()
{

}

//==============================================================================
const juce::String LaweqAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LaweqAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LaweqAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LaweqAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LaweqAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LaweqAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LaweqAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LaweqAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String LaweqAudioProcessor::getProgramName (int index)
{
    return {};
}

void LaweqAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void LaweqAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    setupAllFilters(sampleRate, samplesPerBlock);
}

void LaweqAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LaweqAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void LaweqAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    updateAllFilters();

    juce::dsp::AudioBlock<float> block = juce::dsp::AudioBlock<float>(buffer);
    juce::dsp::ProcessContextReplacing<float> context = juce::dsp::ProcessContextReplacing<float>(block);

    // Process each filter in sequence
    if(parameters->getRawParameterValue("lpToggle")->load() > 0.5f)
    {
        lowPassFilter.process(context);
    }
    if (parameters->getRawParameterValue("hpToggle")->load() > 0.5f)
    {
        highPassFilter.process(context);
    }
    if (parameters->getRawParameterValue("mgToggle")->load() > 0.5f)
    {
        midRangeFilter.process(context);
    }
    buffer.applyGain(juce::Decibels::decibelsToGain(parameters->getRawParameterValue("allGain")->load()));
}

//==============================================================================
bool LaweqAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LaweqAudioProcessor::createEditor()
{
    return new LaweqAudioProcessorEditor (*this);
}

//==============================================================================
void LaweqAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream mos(destData, true);
    parameters->state.writeToStream(mos);
}

void LaweqAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if(tree.isValid() && !juce::JUCEApplication::getInstance()->isStandaloneApp())
    {
        parameters->replaceState(tree);
        updateAllFilters();
    }
}

//This sets up all parameters for the plugin
void LaweqAudioProcessor::setupAllParameters()
{
    auto createParameterLayout = []() 
    {
        juce::AudioProcessorValueTreeState::ParameterLayout layout;

        layout.add(std::make_unique<juce::AudioParameterFloat>("lowPass", // parameterID
            "lowPass", // parameter name
            juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 1.0f), // range for the parameters along with interval and skew factor
            20000.0f)); // default value
        layout.add(std::make_unique<juce::AudioParameterFloat>("highPass", // parameterID
            "highPass", // parameter name
            juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 1.0f), // range for the parameters along with interval and skew factor
            20.0f)); // default value
        layout.add(std::make_unique<juce::AudioParameterFloat>("midGain", // parameterID
            "midGain", // parameter name
            juce::NormalisableRange<float>(-24.0f, 24.0f, 0.5f, 1.0f), // range for the parameters along with interval and skew factor
            0.0f)); // default value
        layout.add(std::make_unique<juce::AudioParameterFloat>("allGain", // parameterID
            "allGain", // parameter name
            juce::NormalisableRange<float>(-24.0f, 24.0f, 0.05f, 1.0f), // range for the parameters along with interval and skew factor
            0.0f)); // default value
        layout.add(std::make_unique<juce::AudioParameterBool>("lpToggle", // parameterID
            "lpToggle",   // parameter name
            true));        // default value
        layout.add(std::make_unique<juce::AudioParameterBool>("hpToggle", // parameterID
            "hpToggle",   // parameter name
            true));        // default value
        layout.add(std::make_unique<juce::AudioParameterBool>("mgToggle", // parameterID
            "mgToggle",   // parameter name
            true));        // default value

        //------------------------------------------------------------------------------------
        //---------------------These parameters aren't editable by the user-------------------
        //------------------------------------------------------------------------------------

        layout.add(std::make_unique<juce::AudioParameterFloat>("midFreq", // parameterID
            "midFreq", // parameter name
            juce::NormalisableRange<float>(200.0f, 5000.0f, 0.5f, 1.0f), // range for the parameters along with interval and skew factor
            1000.0f)); // default value
        layout.add(std::make_unique<juce::AudioParameterFloat>("midQ", // parameterID
            "midQ", // parameter name
            juce::NormalisableRange<float>(0.1f, 10.0f, 1.0f, 1.0f), // range for the parameters along with interval and skew factor
            1.0f)); // default value

        return layout;
    };


    parameters = std::make_unique<juce::AudioProcessorValueTreeState>(*this, nullptr, "Parameters", createParameterLayout());
}

//This sets up all filters for the plugin
void LaweqAudioProcessor::setupAllFilters(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, parameters->getRawParameterValue("lowPass")->load());
    *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, parameters->getRawParameterValue("highPass")->load());
    *midRangeFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, parameters->getRawParameterValue("midFreq")->load(),
        parameters->getRawParameterValue("midQ")->load(),
        juce::Decibels::decibelsToGain(parameters->getRawParameterValue("midGain")->load())
        );

    lowPassFilter.prepare(spec);
    highPassFilter.prepare(spec);
    midRangeFilter.prepare(spec);
}

//This updates all filters for the plugin
void LaweqAudioProcessor::updateAllFilters()
{
    *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), parameters->getRawParameterValue("lowPass")->load());
    *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), parameters->getRawParameterValue("highPass")->load());
    *midRangeFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), parameters->getRawParameterValue("midFreq")->load(),
        parameters->getRawParameterValue("midQ")->load(),
        juce::Decibels::decibelsToGain(parameters->getRawParameterValue("midGain")->load())
    );
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LaweqAudioProcessor();
}
