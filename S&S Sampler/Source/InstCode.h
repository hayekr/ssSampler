/*
  ==============================================================================

    InstCode.h
    Created: 5 Aug 2024 3:33:24pm
    Author:  Nicholas Hayek

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "Sampler.h"

class InstCode : public Sampler {
public:
    void setup();
    int pianoTop = 20;
    int mezzoPianoTop = 70;
    int mezzoForteTop = 115;
private:
    juce::AudioFormatManager audioFormatManager;
    bool setuped = false;
};
