/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "cmath"              //R1.00 Added library.

//==============================================================================
MakoBiteAudioProcessor::MakoBiteAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
    ),
       
    //R1.00 Added for VALUE TREE. 
    //R1.00 The DAW needs a way to pass setup info back and forth. These are defined here.
    parameters(*this, nullptr, "PARAMETERS", {
      std::make_unique<juce::AudioParameterFloat>("gain","Gain", -1.0f, 1.0f, .0f),
      std::make_unique<juce::AudioParameterFloat>("thump","Thump", 0.0f, 1.0f, 0.00f),
      std::make_unique<juce::AudioParameterInt>("band","Band", 0.0f, 10.0f, 0.0f),
      std::make_unique<juce::AudioParameterFloat>("band1","Band1", -12.0f, 12.0f, 0.0f),
      std::make_unique<juce::AudioParameterFloat>("band2","Band2", -12.0f, 12.0f, 0.0f),
      std::make_unique<juce::AudioParameterFloat>("band3","Band3", -12.0f, 12.0f, 0.0f),
      std::make_unique<juce::AudioParameterFloat>("band4","Band4", -12.0f, 12.0f, 0.0f),
      std::make_unique<juce::AudioParameterFloat>("band5","Band5", -12.0f, 12.0f, 0.0f),
      std::make_unique<juce::AudioParameterInt>("lp","LP", 2000.0f, 10000.0f, 10000.0f),
    }

    )

#endif
{     
   
}

MakoBiteAudioProcessor::~MakoBiteAudioProcessor()
{
}

//==============================================================================
const juce::String MakoBiteAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MakoBiteAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MakoBiteAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MakoBiteAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MakoBiteAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MakoBiteAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MakoBiteAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MakoBiteAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MakoBiteAudioProcessor::getProgramName (int index)
{
    return {};
}

void MakoBiteAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MakoBiteAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    //R1.00 Get our Sample Rate for filter calculations.
    SampleRate = MakoBiteAudioProcessor::getSampleRate();
    if (SampleRate < 21000) SampleRate = 48000;
    if (192000 < SampleRate) SampleRate = 48000;

    //R1.00 Calc our filters before we try to process data.
    Settings_Update();
}

void MakoBiteAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MakoBiteAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MakoBiteAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    //R1.00 Our defined variables.
    float tS;

    //R1.00 Handle any changes to our Paramters.
    //R1.00 Handle any settings changes made in Editor. 
    //R1.01 Probably do not want to calc in editor because the variables
    //R1.01 may be in use here as you are changing them.
    if (0 < SettingsChanged) Settings_Update();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
        for (int samp = 0; samp < buffer.getNumSamples(); samp++)
        {
            
            //R1.01 All of OUR work starts here.
            tS = buffer.getSample(channel, samp);  //R1.00 Get the current sample and put it in tS. 
            tS = Thump_ProcessAudio(tS, channel);  //R1.00 Apply our OD to the sample. 
            channelData[samp] = tS;                //R1.00 Write our modified sample back into the sample buffer.

        }
    }
}

//==============================================================================
bool MakoBiteAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MakoBiteAudioProcessor::createEditor()
{
    return new MakoBiteAudioProcessorEditor (*this);
}


//==============================================================================
void MakoBiteAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    //R1.00 Save our parameters to file/DAW.
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);           
   
}

void MakoBiteAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
        
    //R1.00 Read our parameters from file/DAW.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
    
    //R1.01 Dont know why these dont propagate after load, so force them to.
    //R1.01 This Code is probably loaded before the EDITOR exists? So we
    //R1.01 need to put this data into our Processor variables, which
    //R1.01 the editor variables pull from.
    Pedal_Gain = makoGetParmValue_float("gain");
    Pedal_Thump = makoGetParmValue_float("thump");
    Pedal_Band = makoGetParmValue_int("band");
    Pedal_Band1 = makoGetParmValue_float("band1");
    Pedal_Band2 = makoGetParmValue_float("band2");
    Pedal_Band3 = makoGetParmValue_float("band3");
    Pedal_Band4 = makoGetParmValue_float("band4");
    Pedal_Band5 = makoGetParmValue_float("band5");
    Pedal_LP = makoGetParmValue_int("lp");
}


int MakoBiteAudioProcessor::makoGetParmValue_int(juce::String Pstring)
{
    auto parm = parameters.getRawParameterValue(Pstring);
    if (parm != NULL)
        return int(parm->load());
    else
        return 0;
}

float MakoBiteAudioProcessor::makoGetParmValue_float(juce::String Pstring)
{
    auto parm = parameters.getRawParameterValue(Pstring);
    if (parm != NULL)
        return float(parm->load());
    else
        return 0.0f;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MakoBiteAudioProcessor();
}


float MakoBiteAudioProcessor::Filter_Calc_BiQuad(float tSample, int channel, tp_filter* fn)
{
    //R1.00 This function takes a sample (tSample) and applies the filter (fn) to the signal.
    //R1.00 The filters need past samples to calc from, so you need vars for each channel. We have 2 defined for Stereo (L,R).
    float tS = tSample;

    //R1.00 Apply insane math some aliens left here decades ago.
    fn->xn0[channel] = tS;
    tS = fn->a0 * fn->xn0[channel] + fn->a1 * fn->xn1[channel] + fn->a2 * fn->xn2[channel] - fn->b1 * fn->yn1[channel] - fn->b2 * fn->yn2[channel];
    fn->xn2[channel] = fn->xn1[channel]; fn->xn1[channel] = fn->xn0[channel]; fn->yn2[channel] = fn->yn1[channel]; fn->yn1[channel] = tS;

    return tS;
}

void MakoBiteAudioProcessor::Filter_BP_Coeffs(float Gain_dB, float Fc, float Q, tp_filter* fn)
{
    //R1.00 This code calculates BIQUAD filter coefficients and stores them in passed in Filter fn.
    //R1.00 The coefficients are used in Filter_Calc_BiQuad to apply the filter.
    
    //R1.00 Second order parametric/peaking boost filter with constant-Q.
    float K = pi2 * (Fc * .5f) / SampleRate;
    float K2 = K * K;
    float V0 = pow(10.0, Gain_dB / 20.0);

    float a = 1.0f + (V0 * K) / Q + K2;
    float b = 2.0f * (K2 - 1.0f);
    float g = 1.0f - (V0 * K) / Q + K2;
    float d = 1.0f - K / Q + K2;
    float dd = 1.0f / (1.0f + K / Q + K2);

    fn->a0 = a * dd;
    fn->a1 = b * dd;
    fn->a2 = g * dd;
    fn->b1 = b * dd;
    fn->b2 = d * dd;
    fn->c0 = 1.0f;
    fn->d0 = 0.0f;
}

void MakoBiteAudioProcessor::Filter_LP_Coeffs(float fc, tp_filter* fn)
{
    //R1.00 This code calculates BIQUAD filter coefficients and stores them in passed in Filter fn.
    //R1.00 The coefficients are used in Filter_Calc_BiQuad to apply the filter.
    //R2.00 Second order LOW PASS filter. 
    float c = 1.0f / (tanf(pi * fc / SampleRate));
    fn->a0 = 1.0f / (1.0f + sqrt2 * c + (c * c));
    fn->a1 = 2.0f * fn->a0;
    fn->a2 = fn->a0;
    fn->b1 = 2.0f * fn->a0 * (1.0f - (c * c));
    fn->b2 = fn->a0 * (1.0f - sqrt2 * c + (c * c));
}

void MakoBiteAudioProcessor::Filter_HP_Coeffs(float fc, tp_filter* fn)
{
    //R1.00 This code calculates BIQUAD filter coefficients and stores them in passed in Filter fn.
    //R1.00 The coefficients are used in Filter_Calc_BiQuad to apply the filter.
    //F1.00 Second order butterworth High Pass.
    float c = tanf(pi * fc / SampleRate);
    fn->a0 = 1.0f / (1.0f + sqrt2 * c + (c * c));
    fn->a1 = -2.0f * fn->a0;
    fn->a2 = fn->a0;
    fn->b1 = 2.0f * fn->a0 * ((c * c) - 1.0f);
    fn->b2 = fn->a0 * (1.0f - sqrt2 * c + (c * c));
}


float MakoBiteAudioProcessor::Thump_ProcessAudio(float tSample, int channel)
{
    //R1.00 This is our function we created to BRING THE THUMP!!!

    //R1.00 Make a copy of our incoming signal to modify. Just good practice.
    float tS = tSample;
    
    //R1.00 Calc our thumpy signal by applying  a 150 Hz Low Pass filter.
    //R1.00 A parameter could be added to change this filters freq.
    //R1.00 150Hz works well generically so we have it fixed at 150.
    float tSThump = Filter_Calc_BiQuad(tSample, channel, &makoF_Thump);
    
    //R1.00 Distort the Low Pass Signal to emulate speaker compression and cabinet thump.
    //R1.00 Pedal_Thump is both volume and distortion as we drive into compression.
    //R1.00 Could be optimised by applying gain and distortion separately.
    tSThump = tanhf(tSThump * Pedal_Thump * 5.0f);                       

    //R1.00 Add the new Thump to the original signal. 
    //R1.00 Do before any EQ to reduce phase issues from filters shifting the sample phase.
    tS = (tS + tSThump);

    //1.00 Apply the Low Pass filter to reduce Fizz in signal. Dont do LP if set to max.
    if (Pedal_LP < 10000) tS = Filter_Calc_BiQuad(tS, channel, &makoF_LP);

    //R1.00 Apply EQ. Try to not to calc, if not needed, to save CPU cycles.
    if (Pedal_Band1 != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_Band1);
    if (Pedal_Band2 != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_Band2);
    if (Pedal_Band3 != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_Band3);
    if (Pedal_Band4 != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_Band4);
    if (Pedal_Band5 != .0f) tS = Filter_Calc_BiQuad(tS, channel, &makoF_Band5);
        
    //R1.00 Volume/Gain adjust.
    tS = tS * (Pedal_Gain + 1.0f) * 2.0f;

    //R1.00 It is possible our code can drive the volume past the clipping range.
    //R1.00 Clip the signal to -1 and +1 or VST will stop working.
    //R1.00 Should set a var here to let the user know they are clipping.
    if (.999f < tS) tS = .999f;
    if (tS < -.999f) tS = -.999f;

    return tS;
}

void MakoBiteAudioProcessor::Settings_Update()
{
    //R1.00 Update our Filters. Gain, Drive, Mix do not need recalc unless you are smoothing the changes.
    Filter_LP_Coeffs(150.0f, &makoF_Thump);
    Filter_BP_Coeffs(Pedal_Band1, Band1_Freq, Band1_Q, &makoF_Band1);
    Filter_BP_Coeffs(Pedal_Band2, Band2_Freq, Band2_Q, &makoF_Band2);
    Filter_BP_Coeffs(Pedal_Band3, Band3_Freq, Band3_Q, &makoF_Band3);
    Filter_BP_Coeffs(Pedal_Band4, Band4_Freq, Band4_Q, &makoF_Band4);
    Filter_BP_Coeffs(Pedal_Band5, Band5_Freq, Band5_Q, &makoF_Band5);
    Filter_LP_Coeffs(Pedal_LP, &makoF_LP);

    SettingsChanged = false;
}

