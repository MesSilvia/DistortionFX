/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistorionEffectAudioProcessorEditor::DistorionEffectAudioProcessorEditor (DistorionEffectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{


    //DISTORSION KNOBS/LABELS

    //DRIVE

    addAndMakeVisible(driveKnob = new juce::Slider("Drive"));
    driveKnob->setLookAndFeel(&myLookAndFeelV1);
    driveKnob->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    driveKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);

    addAndMakeVisible(driveLabel = new juce::Label("driveLabel"));
    driveLabel->setText("DRIVE", juce::NotificationType::dontSendNotification);
    driveLabel->setJustificationType(juce::Justification::centred);
    driveLabel->setFont(juce::Font(16.0f, juce::Font::bold));


    //RANGE

    addAndMakeVisible(rangeKnob = new juce::Slider("Range"));
    rangeKnob->setLookAndFeel(&myLookAndFeelV1);
    rangeKnob->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    rangeKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);

    addAndMakeVisible(rangeLabel = new juce::Label("rangeLabel"));
    rangeLabel->setText("RANGE", juce::NotificationType::dontSendNotification);
    rangeLabel->setJustificationType(juce::Justification::centred);
    rangeLabel->setFont(juce::Font(16.0f, juce::Font::bold));

    //BLEND

    addAndMakeVisible(blendKnob = new juce::Slider("Blend"));
    blendKnob->setLookAndFeel(&myLookAndFeelV1);
    blendKnob->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    blendKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);


    addAndMakeVisible(blendLabel = new juce::Label("blendLabel"));
    blendLabel->setText("BLEND", juce::NotificationType::dontSendNotification);
    blendLabel->setJustificationType(juce::Justification::centred);
    blendLabel->setFont(juce::Font(16.0f, juce::Font::bold));

    //VOLUME 

    addAndMakeVisible(volumeKnob = new juce::Slider("Volume"));
    volumeKnob->setLookAndFeel(&myLookAndFeelV1);
    volumeKnob->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    volumeKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);

    addAndMakeVisible(volumeLabel = new juce::Label("volumeLabel"));
    volumeLabel->setText("VOLUME", juce::NotificationType::dontSendNotification);
    volumeLabel->setJustificationType(juce::Justification::centred);
    volumeLabel->setFont(juce::Font(16.0f, juce::Font::bold));


    //REVERB/DELAY KNOBS/LABELS

    addAndMakeVisible(drweKnob = new juce::Slider("DryWet"));
    drweKnob->setLookAndFeel(&myLookAndFeelV1);
    drweKnob->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    drweKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);

    addAndMakeVisible(drweLabel = new juce::Label("drweLabel"));
    drweLabel->setText("DRY/WET", juce::NotificationType::dontSendNotification);
    drweLabel->setJustificationType(juce::Justification::centred);
    drweLabel->setFont(juce::Font(16.0f, juce::Font::bold));


    addAndMakeVisible(sizeKnob = new juce::Slider("Size"));
    sizeKnob->setLookAndFeel(&myLookAndFeelV1);
    sizeKnob->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    sizeKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);


    addAndMakeVisible(sizeLabel = new juce::Label("sizeLabel"));
    sizeLabel->setText("SIZE", juce::NotificationType::dontSendNotification);
    sizeLabel->setJustificationType(juce::Justification::centred);
    sizeLabel->setFont(juce::Font(16.0f, juce::Font::bold));

    //REVERB/DELAY COMBO BOX

    addAndMakeVisible(effectType = new juce::ComboBox("effectType"));
    effectType->addItem("Reverb", 1);
    effectType->addItem("Delay", 2);
    effectType->setJustificationType(juce::Justification::centred);




    //DISTORTION COMBO BOX

    addAndMakeVisible(distortionType = new juce::ComboBox("distortionType"));
    distortionType->addItem("Soft Clip", 1);
    distortionType->addItem("Hard Clip", 2);
    distortionType->addItem("Half Wave Rectifier", 3);
    distortionType->addItem("Full Wave Rectifier", 4);
    distortionType->setJustificationType(juce::Justification::centred);


    //FILTER KNOBS/LABELS

    addAndMakeVisible(cutoffKnob = new juce::Slider("Cutoff"));
    cutoffKnob->setLookAndFeel(&myLookAndFeelV1);
    cutoffKnob->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    cutoffKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);


    addAndMakeVisible(cutoffLabel = new juce::Label("cutoffLabel"));
    cutoffLabel->setText("CUTOFF", juce::NotificationType::dontSendNotification);
    cutoffLabel->setJustificationType(juce::Justification::centred);
    cutoffLabel->setFont(juce::Font(16.0f, juce::Font::bold));


    addAndMakeVisible(resonanceKnob = new juce::Slider("Resonance"));
    resonanceKnob->setLookAndFeel(&myLookAndFeelV1);
    resonanceKnob->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    resonanceKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);


    addAndMakeVisible(resonanceLabel = new juce::Label("resonanceLabel"));
    resonanceLabel->setText("RESONANCE", juce::NotificationType::dontSendNotification);
    resonanceLabel->setJustificationType(juce::Justification::centred);
    resonanceLabel->setFont(juce::Font(16.0f, juce::Font::bold));


    addAndMakeVisible(filterType = new juce::ComboBox("filterType"));
    filterType->addItem("Low Pass", 1);
    filterType->addItem("Band Pass", 2);
    filterType->addItem("High Pass", 3);
    filterType->setJustificationType(juce::Justification::centred);




    //DRIVE, RANGE, BLEND, VOLUME ATTACHMENTS

    driveAttach = new juce::AudioProcessorValueTreeState::SliderAttachment(p.getState(), "drive", *driveKnob);
    blendAttach = new juce::AudioProcessorValueTreeState::SliderAttachment(p.getState(), "blend", *blendKnob);
    rangeAttach = new juce::AudioProcessorValueTreeState::SliderAttachment(p.getState(), "range", *rangeKnob);
    volumeAttach = new juce::AudioProcessorValueTreeState::SliderAttachment(p.getState(), "volume", *volumeKnob);


    //REVERB/DELAY ATTACHMENTS
    drweAttach = new juce::AudioProcessorValueTreeState::SliderAttachment(p.getState(), "drywet", *drweKnob);
    sizeAttach = new juce::AudioProcessorValueTreeState::SliderAttachment(p.getState(), "size", *sizeKnob);

    //REVERB/DELAY COMBO BOX ATTACHMENT
    effectAttach = new juce::AudioProcessorValueTreeState::ComboBoxAttachment(p.getState(), "effect", *effectType);


    //DISTORTION COMBO BOX ATTACHMENT
    typeAttach = new juce::AudioProcessorValueTreeState::ComboBoxAttachment(p.getState(), "type", *distortionType);

    
    //FILTER COMBO BOX ATTACHMENT
    filterAttach = new juce::AudioProcessorValueTreeState::ComboBoxAttachment(p.getState(), "filter", *filterType);


    //FILTER ATTACHMENTS
    cutoffAttach = new juce::AudioProcessorValueTreeState::SliderAttachment(p.getState(), "cutoff", *cutoffKnob);
    resonanceAttach = new juce::AudioProcessorValueTreeState::SliderAttachment(p.getState(), "resonance", *resonanceKnob);


    cutoffKnob->setSkewFactorFromMidPoint(1000.0f);

    setSize (833, 500);
}

DistorionEffectAudioProcessorEditor::~DistorionEffectAudioProcessorEditor()
{
}

//==============================================================================
void DistorionEffectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawImageAt(juce::ImageCache::getFromMemory(BinaryData::back_vst_png, BinaryData::back_vst_pngSize), 0, 0);
}

void DistorionEffectAudioProcessorEditor::resized()
{

    //DISTORSION BOUNDS

    distortionType->setBounds((416/2), 137, 400, 30);

    driveKnob->setBounds(((getWidth() / 5)* 1) - (100 / 2), (getHeight() / 2) - (116 / 2), 105, 105);
    rangeKnob->setBounds(((getWidth() / 5) * 2) - (100 / 2), (getHeight() / 2) - (116 / 2), 105, 105);
    blendKnob->setBounds(((getWidth() / 5) * 3) - (100 / 2), (getHeight() / 2) - (116 / 2), 105, 105);
    volumeKnob->setBounds(((getWidth() / 5) * 4) - (100 / 2), (getHeight() / 2) - (116 / 2), 105, 105);


    driveLabel->setBounds(((getWidth() / 5) * 1) - (100 / 2) + 10, (getHeight() / 2) + 40, 80, 50);
    rangeLabel->setBounds(((getWidth() / 5) * 2) - (100 / 2) + 10, (getHeight() / 2) + 40, 80, 50);
    blendLabel->setBounds(((getWidth() / 5) * 3) - (100 / 2) + 10, (getHeight() / 2) + 40, 80, 50);
    volumeLabel->setBounds(((getWidth() / 5) * 4) - (100 / 2) + 10, (getHeight() / 2) + 40, 80, 50);


    //REVERB/DELAY BOUNDS
    drweKnob->setBounds(((getWidth() / 6) * 1) - (100 / 2), 390, 75, 75);
    sizeKnob->setBounds(((getWidth() / 6) * 2) - (100 / 2) - 25, 390, 75, 75);

    drweLabel->setBounds(((getWidth() / 6) * 1) - (100 / 2), 435, 75, 75);
    sizeLabel->setBounds(((getWidth() / 6) * 2) - (100 / 2) - 25, 435, 75, 75);

    effectType->setBounds(((getWidth() / 6) * 1.5) - (100 / 2) - 50, 370, 155, 20);


    //FILTER BOUNDS

    cutoffKnob->setBounds(((getWidth() / 6) * 4) + 5, 390, 75, 75);
    resonanceKnob->setBounds(((getWidth() / 6) * 5) - 20, 390, 75, 75);
    filterType->setBounds(((getWidth() / 6) * 4.5) - (100 / 2) + 5, 367, 155, 20);


    cutoffLabel->setBounds(((getWidth() / 6) * 4) + 5, 435, 75, 75);
    resonanceLabel->setBounds(((getWidth() / 6) * 5) - 20, 435, 75, 75);

}
