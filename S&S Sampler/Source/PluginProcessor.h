/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "InstCode.h"

//==============================================================================
/**
*/
class SSSamplerAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SSSamplerAudioProcessor();
    ~SSSamplerAudioProcessor() override;

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
    
    void updateADSR();
    void updateBuckets();
    juce::ADSR::Parameters& getADSRParams() { return ADSRparams; }
    
    void setPianoTop(int newValue) {pianoTop = newValue; }
    void setMezzoPianoTop(int newValue) {mezzoPianoTop = newValue; }
    void setMezzoForteTop(int newValue) {mezzoForteTop = newValue; }
    
    int getPianoTop() {return pianoTop ; }
    int getMezzoPianoTop() {return mezzoPianoTop ; }
    int getMezzoForteTop() {return mezzoForteTop ; }
    
    void encryptWavs();
    

private:
    //==============================================================================
    InstCode synth;
    juce::ADSR::Parameters ADSRparams = juce::ADSR::Parameters(0, .2, .8,.25);
    
    int pianoTop = synth.pianoTop;
    int mezzoPianoTop = synth.mezzoPianoTop;
    int mezzoForteTop = synth.mezzoForteTop;
    
    juce::Reverb rev = juce::Reverb();
    juce::Reverb::Parameters revParams = juce::Reverb::Parameters();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SSSamplerAudioProcessor);
};

