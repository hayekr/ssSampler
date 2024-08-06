/*
  ==============================================================================

    SamplerSoundLayer.cpp
    Created: 5 Aug 2024 3:33:47pm
    Author:  Nicholas Hayek

  ==============================================================================
*/

#include "SamplerSoundLayer.h"


SamplerSoundLayer::SamplerSoundLayer (const juce::String& soundName,
                                      juce::AudioFormatReader& source,
                                      const juce::BigInteger& notes,
                                      const int midiNoteForNormalPitch,
                                      juce::Range<float> velocityRange,
                                      const double attackTimeSecs,
                                      const double releaseTimeSecs,
                                      const double maxSampleLengthSeconds,
                                      const int velocityType)
    :   SamplerSound(
            soundName,
            source,
            notes,
            midiNoteForNormalPitch,
            attackTimeSecs,
            releaseTimeSecs,
            maxSampleLengthSeconds
        ),
        velocity( velocityRange ),
        velocityType( velocityType )
{
}

SamplerSoundLayer::~SamplerSoundLayer()
{
}

bool SamplerSoundLayer::appliesTo( int midiNoteNumber, float velocity)
{
    bool appliesToMidiNote = appliesToNote( midiNoteNumber );
    bool isInVelocityRange = this->velocity.contains( velocity );
    return appliesToMidiNote && isInVelocityRange;
}

void SamplerSoundLayer::setVelocityRange(juce::Range<float> velocityRange){
    this->velocity = velocityRange;
}


//1 = Piano
//2 = MezzoPiano
//3 = MezzoForte
//4 = Forte
