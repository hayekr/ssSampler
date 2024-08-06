/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/

class MyLookAndFeel : public juce::LookAndFeel_V4
{
public:
    juce::Label* createSliderTextBox (juce::Slider& slider) override
    {
        juce::Label* l = LookAndFeel_V4::createSliderTextBox (slider);
        l->setColour (juce::Label::textColourId, juce::Colours::black);
        l->setFont(juce::FontOptions ("Gill Sans", 12.0f, juce::Font::plain));
        l->setColour (juce::Label::outlineColourId, juce::Colours::transparentWhite );
        //l->setColour (juce::Label::outlineWhenEditingColourId, juce::Colours::transparentWhite );
        return l;
    }
    
    juce::Slider::SliderLayout getSliderLayout (juce::Slider& slider) override{
        
        juce::Slider::SliderLayout layout = LookAndFeel_V4::getSliderLayout(slider);
        auto localBounds = slider.getLocalBounds();
        if (slider.getTextBoxPosition() == juce::Slider::TextBoxBelow){
            layout.textBoxBounds.setY (localBounds.getHeight() - 35);
        }
        
        return layout;
    }
    
};



class SSSamplerAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    SSSamplerAudioProcessorEditor (SSSamplerAudioProcessor&);
    ~SSSamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    juce::Slider attackSlider, decaySlider, sustainSlider, releaseSlider;
    juce::Label attackSliderLabel, decaySliderLabel, sustainSliderLabel, releaseSliderLabel;
    juce::Label velocitySliderLabel;
    
    juce::Slider velocityBuckets;
    juce::Value rangeMin = velocityBuckets.getMinValueObject();
    juce::Value rangeMax = velocityBuckets.getMaxValueObject();
    
    void sliderValueChanged(juce::Slider* slider) override;
    void velocitySliderChanged();
    
    void constructKnobLabels(juce::Slider* slider, juce::Label* label, int textWidth, int textHeight, float xShift, float yShift, float defValue);

private:
    SSSamplerAudioProcessor& audioProcessor;
    MyLookAndFeel myLook;
    juce::Colour accent = juce::Colour::fromFloatRGBA (1, 0.918, 0.741, 1);
    juce::Colour accentDark = juce::Colour::fromFloatRGBA (0.494, 0.757, 1, 1);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SSSamplerAudioProcessorEditor);
};

