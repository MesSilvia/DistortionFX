/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "myLookAndFeel.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DistorionEffectAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DistorionEffectAudioProcessorEditor (DistorionEffectAudioProcessor&);
    ~DistorionEffectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    //DISTORSION
    
    juce::ScopedPointer<juce::Slider> driveKnob;
    juce::ScopedPointer<juce::Slider> rangeKnob;
    juce::ScopedPointer<juce::Slider> blendKnob;
    juce::ScopedPointer<juce::Slider> volumeKnob;

    juce::ScopedPointer<juce::ComboBox> distortionType;



    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttach;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> blendAttach;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> rangeAttach;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttach;

    juce::ScopedPointer<juce::AudioProcessorValueTreeState::ComboBoxAttachment> typeAttach;

    juce::ScopedPointer<juce::Label> driveLabel;
    juce::ScopedPointer<juce::Label> rangeLabel;
    juce::ScopedPointer<juce::Label> blendLabel;
    juce::ScopedPointer<juce::Label> volumeLabel;

    

    //REVERB / DELAY

    juce::ScopedPointer<juce::Slider> drweKnob;
    juce::ScopedPointer<juce::Slider> sizeKnob;

    juce::ScopedPointer<juce::ComboBox> effectType;
    

    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> sizeAttach;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> drweAttach;

    juce::ScopedPointer<juce::AudioProcessorValueTreeState::ComboBoxAttachment> effectAttach;


    juce::ScopedPointer<juce::Label> sizeLabel;
    juce::ScopedPointer<juce::Label> drweLabel;


    //FILTER

    juce::ScopedPointer<juce::Slider> cutoffKnob;
    juce::ScopedPointer<juce::Slider> resonanceKnob;
    juce::ScopedPointer<juce::ComboBox> filterType;

 
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterAttach;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttach;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> resonanceAttach;

    juce::ScopedPointer<juce::Label> cutoffLabel;
    juce::ScopedPointer<juce::Label> resonanceLabel;


    //LOOK AND FEEL

    myLookAndFeelV1 myLookAndFeelV1;


    DistorionEffectAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistorionEffectAudioProcessorEditor)
};
