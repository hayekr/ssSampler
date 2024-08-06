/*
  ==============================================================================

    SamplerSoundLayer.h
    Created: 5 Aug 2024 3:33:47pm
    Author:  Nicholas Hayek

  ==============================================================================
*/


#ifndef SAMPLERSOUNDLAYER_H_INCLUDED
#define SAMPLERSOUNDLAYER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class SamplerSoundLayer : public juce::SamplerSound
{
public:
    SamplerSoundLayer (
        const juce::String& name,
        juce::AudioFormatReader& source,
        const juce::BigInteger& midiNotes,
        int midiNoteForNormalPitch,
        juce::Range<float> velocityRange,
        double attackTimeSecs,
        double releaseTimeSecs,
        double maxSampleLengthSeconds,
        int velocityType
    );
    ~SamplerSoundLayer();
    bool appliesTo( int midiNoteNumber, float velocity);
    void setVelocityRange(juce::Range<float> velocityRange);
    int getVelocityType() {return velocityType; }
    
private:
    juce::Range<float> velocity;
    int velocityType;
};

#endif  // SAMPLERSOUNDLAYER_H_INCLUDED

//1 = Piano
//2 = MezzoPiano
//3 = MezzoForte
//4 = Forte
