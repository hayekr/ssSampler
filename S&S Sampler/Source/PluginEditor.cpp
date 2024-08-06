/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SSSamplerAudioProcessorEditor::SSSamplerAudioProcessorEditor (SSSamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    int textWidth = 30;
    int textHeight = 15;
    
    attackSlider.setRange(0.0f, 1.0f, 0.01f);
    attackSliderLabel.setText("Attack", juce::NotificationType::dontSendNotification);
    constructKnobLabels(&attackSlider, &attackSliderLabel, textWidth, textHeight, .037f, -.028f, audioProcessor.getADSRParams().attack);
    
    //DECAY
    decaySlider.setRange(0.0f, 1.0f, 0.01f);
    decaySliderLabel.setText("Decay", juce::NotificationType::dontSendNotification);
    constructKnobLabels(&decaySlider, &decaySliderLabel, textWidth, textHeight, .09f, -.028f, audioProcessor.getADSRParams().decay);

    
    //SUSTAIN
    sustainSlider.setRange(0.0f, 1.0f, 0.01f);
    sustainSliderLabel.setText("Sustain", juce::NotificationType::dontSendNotification);
    constructKnobLabels(&sustainSlider, &sustainSliderLabel, textWidth, textHeight, .142f, -.028f, audioProcessor.getADSRParams().sustain);
    
    //RELEASE
    releaseSlider.setRange(0.0f, 1.0f, 0.01f);
    releaseSliderLabel.setText("Release", juce::NotificationType::dontSendNotification);
    constructKnobLabels(&releaseSlider, &releaseSliderLabel, textWidth, textHeight, .194f, -.028f, audioProcessor.getADSRParams().release);

    //VELOCITY PROCESSING
    velocityBuckets.setSliderStyle(juce::Slider::SliderStyle::ThreeValueHorizontal);
    velocityBuckets.setRange(0, 127, 1);
    
    velocityBuckets.setMaxValue(audioProcessor.getMezzoForteTop(), juce::NotificationType::dontSendNotification, false);
    velocityBuckets.setValue(audioProcessor.getMezzoPianoTop());
    velocityBuckets.setMinValue(audioProcessor.getPianoTop(), juce::NotificationType::dontSendNotification, false);
    
    velocityBuckets.onValueChange = [this]{velocitySliderChanged();};

    addAndMakeVisible(velocityBuckets);
    velocityBuckets.addListener(this);
    
    //VELOCITY LOOK
    velocityBuckets.setColour(juce::Slider::thumbColourId, juce::Colours::black);
    velocityBuckets.setColour(juce::Slider::trackColourId, accentDark);
    velocityBuckets.setColour(juce::Slider::backgroundColourId, accentDark);
    velocitySliderLabel.setFont(juce::FontOptions ("Gill Sans", 15.0f, juce::Font::plain));
    velocityBuckets.setTextBoxStyle(juce::Slider::TextBoxBelow, true,0, 0);
        
    velocitySliderLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(velocitySliderLabel);

    
    setLookAndFeel(&myLook);
    setSize (400, 350);
}

SSSamplerAudioProcessorEditor::~SSSamplerAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void SSSamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (accent);
    g.setColour (juce::Colours::black);
    g.setFont (juce::FontOptions ("Gill Sans", 22.0f, juce::Font::plain));
    g.drawText ("Steinway & Sons Model B", 100, 10, 200, 40, juce::Justification::centred, true);
    g.setFont (juce::FontOptions (12.0f));
    g.drawText ("Piano Provided by Matthew Figgie", 20, 50, 400, 40, juce::Justification::left, true);
    g.drawText ("Recorded using Neumann TLM 102s and Shure KSM 137s", 20, 65, 400, 40, juce::Justification::left, true);
    int xPos = 115;
    g.drawLine(0, xPos, 400, xPos);
    g.drawLine(0, xPos+3, 400, xPos+3);
}

void SSSamplerAudioProcessorEditor::resized()
{
    float relWidth = .2f;
    float relHeight = .4f;

    attackSlider.setBoundsRelative(0.1f, 0.4f, relWidth, relHeight);
    decaySlider.setBoundsRelative(0.3f, 0.4f, relWidth, relHeight);
    sustainSlider.setBoundsRelative(0.5f, 0.4f, relWidth, relHeight);
    releaseSlider.setBoundsRelative(0.7f, 0.4f, relWidth, relHeight);
    
    velocityBuckets.setBoundsRelative(.1f, .75f, .8f, .2f);
    velocitySliderLabel.setBoundsRelative(.15, .9, .7, .03);
}

void SSSamplerAudioProcessorEditor::constructKnobLabels(juce::Slider * slider, juce::Label *label, int textWidth, int textHeight, float xShift, float yShift, float defValue){
    slider->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider->setTextBoxStyle(juce::Slider::TextBoxBelow, true, textWidth, textHeight);
    slider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::black);
    slider->setColour(juce::Slider::rotarySliderOutlineColourId, accent);
    slider->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    slider->setValue(defValue);
    
    label->setColour(juce::Label::textColourId, juce::Colours::black);
    label->setBoundsRelative(slider->getX()+xShift, slider->getY()+yShift, .2f, .4f);
    label->setFont(juce::FontOptions ("Gill Sans", 18.0f, juce::Font::plain));

    slider->addListener(this);
    addAndMakeVisible(label);
    addAndMakeVisible(slider);
}

void SSSamplerAudioProcessorEditor::sliderValueChanged(juce::Slider *slider){
    audioProcessor.getADSRParams().attack = attackSlider.getValue();
    audioProcessor.getADSRParams().decay = decaySlider.getValue();
    audioProcessor.getADSRParams().sustain = sustainSlider.getValue();
    audioProcessor.getADSRParams().release = releaseSlider.getValue();
    audioProcessor.updateADSR();
}

void SSSamplerAudioProcessorEditor::velocitySliderChanged(){
    audioProcessor.setPianoTop(rangeMin.getValue());
    audioProcessor.setMezzoPianoTop(velocityBuckets.getValue());
    audioProcessor.setMezzoForteTop(rangeMax.getValue());
    
    DBG("Piano top @ "<< audioProcessor.getPianoTop());
    DBG("mezzoPiano top @ "<< audioProcessor.getMezzoPianoTop());
    DBG("mezzoForte top @ "<< audioProcessor.getMezzoForteTop());
    
    juce::String s = "0 < Piano " + juce::String::fromUTF8(u8"\u2264 ") + juce::String(audioProcessor.getPianoTop()) + " < M. Piano " + juce::String::fromUTF8(u8"\u2264 ") + juce::String(audioProcessor.getMezzoPianoTop()) + " < M. Forte " + juce::String::fromUTF8(u8"\u2264 ") + juce::String(audioProcessor.getMezzoForteTop()) + " < Forte "+ juce::String::fromUTF8(u8"\u2264 ")+"127";
    velocitySliderLabel.setText(s, juce::NotificationType::dontSendNotification);
    
    audioProcessor.updateBuckets();

}

