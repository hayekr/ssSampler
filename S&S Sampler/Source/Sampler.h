/*
  ==============================================================================

    Sampler.h
    Created: 5 Aug 2024 3:33:32pm
    Author:  Nicholas Hayek

  ==============================================================================
*/

#ifndef SAMPLER_H_INCLUDED
#define SAMPLER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class Sampler : public juce::Synthesiser
{
    
public:
    void noteOn (
        const int midiChannel,
        const int midiNoteNumber,
        const float velocity
    ) override;

};

#endif
