/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class DistorionEffectAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DistorionEffectAudioProcessor();
    ~DistorionEffectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //FILTER UPDATE FUNCTION
    void updateFilter();



    juce::AudioProcessorValueTreeState& getState();

private:

    //STATE
    juce::ScopedPointer<juce::AudioProcessorValueTreeState> state;

    //FILTER

    juce::dsp::ProcessorDuplicator<juce::dsp::StateVariableFilter::Filter<float>, juce::dsp::StateVariableFilter::Parameters<float>> stateVariableFilter;

    float lastSampleRate;

    //REVERB

    juce::Reverb reverb;

    //DELAY LINE PARAMETERS
    juce::AudioSampleBuffer dbuf;
    int dw;
    int dr;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistorionEffectAudioProcessor)
};
