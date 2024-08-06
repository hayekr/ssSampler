/*
  ==============================================================================

    Sampler.cpp
    Created: 5 Aug 2024 3:33:32pm
    Author:  Nicholas Hayek

  ==============================================================================
*/

#include "Sampler.h"
#include "SamplerSoundLayer.h"
extern bool ISDOWN;
bool ISDOWN=false;


void Sampler::noteOn (const int midiChannel,
                          const int midiNoteNumber,
                          const float velocity)
{
    DBG("Note on @ " << velocity*127);
    const juce::ScopedLock sl (lock);

    for (int i = sounds.size(); --i >= 0;)
    {
        juce::SynthesiserSound * const soundSource = sounds.getUnchecked(i).get();
        SamplerSoundLayer* const sound = static_cast<SamplerSoundLayer* const> ( soundSource );

        if ( sound->appliesTo ( midiNoteNumber, velocity)
             && sound->appliesToChannel ( midiChannel ) )
        {
            // If hitting a note that's still ringing, stop it first (it could be
            // still playing because of the sustain or sostenuto pedal).
            for ( int j = voices.size(); --j >= 0; )
            {
                juce::SynthesiserVoice* const voice = voices.getUnchecked (j);
                if ( voice->getCurrentlyPlayingNote() == midiNoteNumber
                     && voice->isPlayingChannel ( midiChannel ) )
                    stopVoice ( voice, velocity, true );
            }
            
            startVoice ( findFreeVoice ( sound, midiChannel, midiNoteNumber, isNoteStealingEnabled() ),
                        sound, midiChannel, midiNoteNumber, .8f );
        }
    }
}
