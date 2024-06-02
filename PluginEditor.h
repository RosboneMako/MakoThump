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

//R1.00 Add SLIDER listener. BUTTON or TIMER listeners also go here if needed. Must add ValueChanged overrides!
class MakoBiteAudioProcessorEditor  : public juce::AudioProcessorEditor , public juce::Slider::Listener //, public juce::Button::Listener , public juce::Timer
{
public:
    MakoBiteAudioProcessorEditor (MakoBiteAudioProcessor&);
    ~MakoBiteAudioProcessorEditor() override;

    //R1.00 OUR override functions.
    void sliderValueChanged(juce::Slider* slider) override;


    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MakoBiteAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MakoBiteAudioProcessorEditor)

    //R1.00 Our functions to ease Slider definitions.
    void GUI_Init_Large_Slider(juce::Slider* slider);
    void GUI_Init_Small_Slider(juce::Slider* slider);

    //R1.00 PNG graphic background with Mako Logo. Comment this out if not used.
    juce::Image imgBack;

    //R1.00 Variable we use for Slider knob color. 
    juce::uint32 colorButton = 0xFF505050;

    //R1.00 Define our UI Juce Slider controls.
    juce::Slider jsP1_Gain;
    juce::Slider jsP1_Thump; 
    juce::Slider jsP1_Band;
    juce::Slider jsP1_Band1;
    juce::Slider jsP1_Band2;
    juce::Slider jsP1_Band3;
    juce::Slider jsP1_Band4;
    juce::Slider jsP1_Band5;
    juce::Slider jsP1_LP;

    void Band_SetFilterValues(bool ForcePaint);
    
    juce::String UIS_Band1 = "";
    juce::String UIS_Band2 = "";
    juce::String UIS_Band3 = "";
    juce::String UIS_Band4 = "";
    juce::String UIS_Band5 = "";
    

public:    
    //R1.00 Define our SLIDER attachment variables.
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_Gain;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_Thump;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_Band;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_Band1;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_Band2;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_Band3;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_Band4;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_Band5;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_LP;
    

};
