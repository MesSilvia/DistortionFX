/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistorionEffectAudioProcessor::DistorionEffectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
    //STATE INITIALIZATION
    state = new juce::AudioProcessorValueTreeState(*this, nullptr);


    //DISTORSION

    state->createAndAddParameter("drive", "Drive", "Drive", juce::NormalisableRange<float>(0.001f, 1.0f, 0.0001), 0.5, nullptr, nullptr);
    state->createAndAddParameter("range", "Range", "Range", juce::NormalisableRange<float>(1.0f, 3000.0f, 0.001), 1500.0, nullptr, nullptr);
    state->createAndAddParameter("blend", "Blend", "Blend", juce::NormalisableRange<float>(0.01f, 1.0f, 0.0001), 0.5, nullptr, nullptr);
    state->createAndAddParameter("volume", "Volume", "Volume", juce::NormalisableRange<float>(0.0f, 3.0f, 0.0001), 1.5, nullptr, nullptr);
    state->createAndAddParameter("type", "distortionType", "distortionType", juce::NormalisableRange<float>(1.0f, 4.0f, 1.0), 1.0, nullptr, nullptr);

    //REVERB/DELAY
    state->createAndAddParameter("drywet", "DryWet", "DryWet", juce::NormalisableRange<float>(0.001f, 1.0f, 0.0001), 0.5, nullptr, nullptr);
    state->createAndAddParameter("size", "Size", "Size", juce::NormalisableRange<float>(0.001f, 1.0f, 0.0001), 0.5, nullptr, nullptr);
    state->createAndAddParameter("effect", "Effect", "Effect", juce::NormalisableRange<float>(1.f, 2.f, 1.0), 1.0, nullptr, nullptr);

    //FILTER
    state->createAndAddParameter("cutoff", "Cutoff", "Cutoff", juce::NormalisableRange<float>(20.f, 20000.0f, 1.0), 10000.0, nullptr, nullptr);
    state->createAndAddParameter("resonance", "Resonance", "Resonance", juce::NormalisableRange<float>(1.f, 10.0f, 1.0), 3.0, nullptr, nullptr);
    state->createAndAddParameter("filter", "Filter", "Filter", juce::NormalisableRange<float>(1.f, 3.f, 1.0), 1.0, nullptr, nullptr);



    //VALUE TREES
    state->state = juce::ValueTree("filter");
    state->state = juce::ValueTree("cutoff");
    state->state = juce::ValueTree("resonance");


    state->state = juce::ValueTree("drive");
    state->state = juce::ValueTree("range");
    state->state = juce::ValueTree("blend");
    state->state = juce::ValueTree("volume");
    state->state = juce::ValueTree("type");


    state->state = juce::ValueTree("drywet");
    state->state = juce::ValueTree("size");
    state->state = juce::ValueTree("effect");



    //REVERB/DELAY INITIALIZATION

    float drywet = *state->getRawParameterValue("drywet");
    float size = *state->getRawParameterValue("size");

    reverb.setParameters({ size, 0.5f, drywet, 1 - drywet, 0.1f, 0.0f });



}

DistorionEffectAudioProcessor::~DistorionEffectAudioProcessor()
{
}

//==============================================================================
const juce::String DistorionEffectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistorionEffectAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool DistorionEffectAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool DistorionEffectAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double DistorionEffectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistorionEffectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DistorionEffectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistorionEffectAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String DistorionEffectAudioProcessor::getProgramName(int index)
{
    return {};
}

void DistorionEffectAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void DistorionEffectAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    //PREPARING THE FILTER 
    lastSampleRate = sampleRate;
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = lastSampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();

    stateVariableFilter.reset();
    updateFilter();
    stateVariableFilter.prepare(spec);

    //SET THE REVERB SAMPLE RATE
    reverb.setSampleRate(sampleRate);


    //PREPARE DELAY LINE

    dbuf.setSize(getTotalNumOutputChannels(), 100000);
    dbuf.clear();
    dr = 1;
    dw = 0;
}

void DistorionEffectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistorionFxAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
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

void DistorionEffectAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    
    //PARAMETERS RETRIVAL

    //DISTORSION 
    float drive = *state->getRawParameterValue("drive");
    float range = *state->getRawParameterValue("range");
    float blend = *state->getRawParameterValue("blend");
    float volume = *state->getRawParameterValue("volume");
    float distortionType = *state->getRawParameterValue("type");
    float smoothGain = 0.5f;


    //REVERB / DELAY
    float drywet = *state->getRawParameterValue("drywet");
    float size = *state->getRawParameterValue("size");
    float effect = *state->getRawParameterValue("effect");


    //SETTING DELAY PARAMETERS

    int numSamples = buffer.getNumSamples();
    float wet_now = drywet;
    float dry_now = (1.0f - drywet);
    int ds_now = (size * 50000);

    const float* channelInData = buffer.getReadPointer(0);
    float* channelOutDataL = buffer.getWritePointer(0);



    //FILTERING SECTION

    juce::dsp::AudioBlock<float> block(buffer);


    updateFilter();
    stateVariableFilter.process(juce::dsp::ProcessContextReplacing<float>(block));


    

    //DISTORSION SECTION

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);


        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {


            float cleanSig = *channelData;

            *channelData *= drive * range;
            

            if (distortionType == 1.0) {

                //SOFT CLIPPING

                if (*channelData > 0) {
                    *channelData = (((1.0f - expf(-(*channelData))) + (cleanSig * (1.f / blend))) / 2) * volume;
                }
                else {
                    *channelData = (((-1.0f + expf(*channelData)) + (cleanSig * (1.f / blend))) / 2)* volume;

                }
            }


            if (distortionType == 2.0) {

                //HARD CLIPPING 

                float threshold = 1.0f; 
                if (*channelData > threshold) *channelData = (((threshold * blend) + (cleanSig * (1.f / blend))) / 2)* volume;
                else if (*channelData < -threshold) *channelData = -(((threshold * blend) + (cleanSig * (1.f / blend))) / 2) * volume;;
                
            }


            if (distortionType == 3.0) {

                //HALF WAVE RECTIFIER

                if (*channelData <= 0) { 

                    *channelData = 0;
                }
                else { 
                    *channelData = (((*channelData * blend) + (cleanSig * (1.f / blend))) / 2) * volume * smoothGain; }


            }


            if (distortionType == 4.0) {

                //FULL WAVE RECTIFIER

                *channelData = (((fabsf(*channelData) * blend) + (cleanSig * (1.f / blend))) / 2) * volume * smoothGain;

            }


            channelData++;


        }


        if (effect == 1) {

            //REVERB PROCESSING

            reverb.setParameters({ size, 0.5f, drywet, 1 - drywet, 0.1f, 0.001f });
            reverb.processMono(buffer.getWritePointer(0), buffer.getNumSamples());

        }

        if (effect == 2) {

            //DELAY LINE PROCESSING

            for (int i = 0; i < numSamples; ++i) {


                dbuf.setSample(0, dw, channelInData[i]);

                channelOutDataL[i] = dry_now * channelInData[i] + wet_now * dbuf.getSample(0, dr);
                dw = (dw + 1) % ds_now;
                dr = (dr + 1) % ds_now;
            }

        }


    }
}

//==============================================================================
bool DistorionEffectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DistorionEffectAudioProcessor::createEditor()
{
    return new DistorionEffectAudioProcessorEditor(*this);

}





juce::AudioProcessorValueTreeState& DistorionEffectAudioProcessor::getState() {


    return *state;

}

//==============================================================================
void DistorionEffectAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, false);
    state->state.writeToStream(stream);



}

void DistorionEffectAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);

    if (tree.isValid()) {

        state->state = tree;

    }

}





//FILTER UPDATE FUNCTIONS

void DistorionEffectAudioProcessor::updateFilter() 
{
    //FILTER
    float cutoff = *state->getRawParameterValue("cutoff");
    float resonance = *state->getRawParameterValue("resonance");
    float filterType = *state->getRawParameterValue("filter");

    if (filterType == 1) {

        stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff, resonance);
    
    }

    if (filterType == 2) {

        stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff, resonance);

    }

    if (filterType == 3) {

        stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::highPass;
        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff, resonance);

    }



}





//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistorionEffectAudioProcessor();
}
