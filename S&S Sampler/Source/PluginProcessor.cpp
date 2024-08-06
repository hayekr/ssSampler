/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SamplerSoundLayer.h"

//==============================================================================
SSSamplerAudioProcessor::SSSamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       //.withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

SSSamplerAudioProcessor::~SSSamplerAudioProcessor()
{
}

//==============================================================================
const juce::String SSSamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SSSamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SSSamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SSSamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SSSamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SSSamplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SSSamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SSSamplerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SSSamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void SSSamplerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SSSamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    synth.setup();
    revParams.wetLevel = 0.1f;
    revParams.roomSize = 0.3f;
    revParams.dryLevel = 0.4f;
    rev.setParameters(revParams);
    updateADSR();
    
}

void SSSamplerAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SSSamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SSSamplerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    rev.processStereo(buffer.getWritePointer(0),buffer.getWritePointer(1), buffer.getNumSamples());
}

//==============================================================================
bool SSSamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SSSamplerAudioProcessor::createEditor()
{
    return new SSSamplerAudioProcessorEditor (*this);
}

//==============================================================================
void SSSamplerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void SSSamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

void SSSamplerAudioProcessor::updateADSR(){
    for (int i=0; i<synth.getNumSounds(); i++){
        if (auto sound = dynamic_cast<juce::SamplerSound*>(synth.getSound(i).get())){
            sound->setEnvelopeParameters(ADSRparams);
        }
    }
}

void SSSamplerAudioProcessor::updateBuckets(){
    for (int i=0; i<synth.getNumSounds(); i++){
        if (auto sound = dynamic_cast<SamplerSoundLayer*>(synth.getSound(i).get())){
            if (sound->getVelocityType()==1){
                juce::Range<float> newPiano;
                newPiano.setStart(1.f/127*0);
                newPiano.setEnd(1.f/127*pianoTop);
                sound->setVelocityRange(newPiano);
                //DBG("Set piano velocity top to" << pianoTop);
            }
            if (sound->getVelocityType()==2){
                juce::Range<float> newPiano;
                newPiano.setStart(1.f/127*(pianoTop+1));
                newPiano.setEnd(1.f/127*mezzoPianoTop);
                sound->setVelocityRange(newPiano);
                //DBG("Set mezzoPiano velocity top to" << mezzoPianoTop);
            }
            if (sound->getVelocityType()==3){
                juce::Range<float> newPiano;
                newPiano.setStart(1.f/127*(mezzoPianoTop+1));
                newPiano.setEnd(1.f/127*mezzoForteTop);
                sound->setVelocityRange(newPiano);
                //DBG("Set mezzoForte velocity top to" << mezzoForteTop);
            }
            if (sound->getVelocityType()==4){
                juce::Range<float> newPiano;
                newPiano.setStart(1.f/127*(mezzoForteTop+1));
                newPiano.setEnd(1.f/127*129);
                sound->setVelocityRange(newPiano);
            }
        }
    }
}

void SSSamplerAudioProcessor::encryptWavs(){
}


//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SSSamplerAudioProcessor();
}

