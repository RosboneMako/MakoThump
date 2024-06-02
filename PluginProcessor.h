/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class MakoBiteAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    MakoBiteAudioProcessor();
    ~MakoBiteAudioProcessor() override;

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

    //R1.00 Add Parameters so we can store/recall our setups in File/DAW.
    juce::AudioProcessorValueTreeState parameters;                           
    
    //R1.00 Our public variables.
    bool SettingsChanged = false;

    float Pedal_Gain = .0f;
    float Pedal_Thump = .0f;
    int   Pedal_Band  = 0;
    float Pedal_Band1 = 0.0f;
    float Pedal_Band2 = 0.0f;
    float Pedal_Band3 = 0.0f;
    float Pedal_Band4 = 0.0f;
    float Pedal_Band5 = 0.0f;
    int   Pedal_LP = 10000;

    float Band1_Freq = 150.0f;
    float Band2_Freq = 300.0f;
    float Band3_Freq = 750.0f;
    float Band4_Freq = 1500.0f;
    float Band5_Freq = 3000.0f;
    float Band1_Q = .707f;
    float Band2_Q = 1.414f;
    float Band3_Q = 1.414f;
    float Band4_Q = 1.414f;
    float Band5_Q = 1.414f;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MakoBiteAudioProcessor)

    int makoGetParmValue_int(juce::String Pstring);
    float makoGetParmValue_float(juce::String Pstring);
   
    //R1.00 Some Constants 
    const float pi = 3.14159265f;
    const float pi2 = 6.2831853f;
    const float sqrt2 = 1.4142135f;
    float SampleRate = 48000.0f;

    //R1.00 OUR FILTER VARIABLES
    struct tp_coeffs {
        float a0;
        float a1;
        float a2;
        float b1;
        float b2;
        float c0;
        float d0;
    };

    struct tp_filter {
        float a0;
        float a1;
        float a2;
        float b1;
        float b2;
        float c0;
        float d0;
        float xn0[2];
        float xn1[2];
        float xn2[2];
        float yn1[2];
        float yn2[2];
        float offset[2];
    };

    //R1.00 FILTERS
    float Filter_Calc_BiQuad(float tSample, int channel, tp_filter* fn);
    void Filter_BP_Coeffs(float Gain_dB, float Fc, float Q, tp_filter* fn);
    void Filter_LP_Coeffs(float fc, tp_filter* fn);
    void Filter_HP_Coeffs(float fc, tp_filter* fn);

    //R1.00 The Mako OD pedal
    tp_filter makoF_Thump = {};
    tp_filter makoF_Band1 = {};
    tp_filter makoF_Band2 = {};
    tp_filter makoF_Band3 = {};
    tp_filter makoF_Band4 = {};
    tp_filter makoF_Band5 = {};
    tp_filter makoF_LP    = {};
    float Thump_ProcessAudio(float tSample, int channel);

    //R1.00 Handle any paramater changes.
    void Settings_Update();

};
