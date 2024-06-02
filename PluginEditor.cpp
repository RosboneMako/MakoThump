/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MakoBiteAudioProcessorEditor::MakoBiteAudioProcessorEditor (MakoBiteAudioProcessor& p)
    : AudioProcessorEditor (&p) , audioProcessor (p)    
{    
    //addMouseListener(this, true);

    //R1.00 Create SLIDER ATTACHMENTS so our parameter vars get adjusted automatically for Get/Set states.
    ParAtt_Gain  = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "gain", jsP1_Gain);
    ParAtt_Thump = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "thump", jsP1_Thump);
    ParAtt_Band  = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "band",  jsP1_Band);
    ParAtt_Band1 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "band1", jsP1_Band1);
    ParAtt_Band2 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "band2", jsP1_Band2);
    ParAtt_Band3 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "band3", jsP1_Band3);
    ParAtt_Band4 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "band4", jsP1_Band4);
    ParAtt_Band5 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "band5", jsP1_Band5);
    ParAtt_LP    = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "lp",    jsP1_LP);
    
    //R1.00 Custom Mako image. Delete this line if you dont have the image.
    imgBack = juce::ImageCache::getFromMemory(BinaryData::makothump_back_png, BinaryData::makothump_back_pngSize);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
     
    //R1.00 Set the window size.
    setSize (480, 260);

    //****************************************************************************************
   //R1.00 Add GUI CONTROLS
   //****************************************************************************************
    GUI_Init_Large_Slider(&jsP1_Gain);
    jsP1_Gain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_Gain.setRange(-1.0, 1.0, .01);
    jsP1_Gain.setValue(audioProcessor.Pedal_Gain);
    jsP1_Gain.addListener(this);
    addAndMakeVisible(jsP1_Gain);

    GUI_Init_Large_Slider(&jsP1_Thump);
    jsP1_Thump.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_Thump.setRange(0.0, 1.0, .01);
    jsP1_Thump.setValue(audioProcessor.Pedal_Thump);
    jsP1_Thump.addListener(this);
    addAndMakeVisible(jsP1_Thump);

    GUI_Init_Large_Slider(&jsP1_Band);
    jsP1_Band.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_Band.setRange(0, 10, 1);
    jsP1_Band.setValue(audioProcessor.Pedal_Band);
    jsP1_Band.addListener(this);
    addAndMakeVisible(jsP1_Band);

    GUI_Init_Large_Slider(&jsP1_Band1);
    jsP1_Band1.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_Band1.setRange(-12.0, 12.0, .01);
    jsP1_Band1.setValue(audioProcessor.Pedal_Band1);
    jsP1_Band1.addListener(this);
    addAndMakeVisible(jsP1_Band1);

    GUI_Init_Large_Slider(&jsP1_Band2);
    jsP1_Band2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_Band2.setRange(-12.0, 12.0, .01);
    jsP1_Band2.setValue(audioProcessor.Pedal_Band2);
    jsP1_Band2.addListener(this);
    addAndMakeVisible(jsP1_Band2);

    GUI_Init_Large_Slider(&jsP1_Band3);
    jsP1_Band3.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_Band3.setRange(-12.0, 12.0, .01);
    jsP1_Band3.setValue(audioProcessor.Pedal_Band3);
    jsP1_Band3.addListener(this);
    addAndMakeVisible(jsP1_Band3);

    GUI_Init_Large_Slider(&jsP1_Band4);
    jsP1_Band4.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_Band4.setRange(-12.0, 12.0, .01);
    jsP1_Band4.setValue(audioProcessor.Pedal_Band4);
    jsP1_Band4.addListener(this);
    addAndMakeVisible(jsP1_Band4);

    GUI_Init_Large_Slider(&jsP1_Band5);
    jsP1_Band5.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_Band5.setRange(-12.0, 12.0, .01);
    jsP1_Band5.setValue(audioProcessor.Pedal_Band5);
    jsP1_Band5.addListener(this);
    addAndMakeVisible(jsP1_Band5);

    GUI_Init_Large_Slider(&jsP1_LP);
    jsP1_LP.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_LP.setTextValueSuffix(" Hz");
    jsP1_LP.setRange(2000, 10000, 100);
    jsP1_LP.setValue(audioProcessor.Pedal_LP);
    jsP1_LP.addListener(this);
    addAndMakeVisible(jsP1_LP);

    //R1.00 Update our filter text strings.
    Band_SetFilterValues(false);
}

MakoBiteAudioProcessorEditor::~MakoBiteAudioProcessorEditor()
{
}


//==============================================================================
void MakoBiteAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    //g.setColour(juce::Colour(0xFF202020));
    //g.fillRect(0, 0, 580, 400);
    
    //R1.01 Draw our background image. Comment this out if missing the PNG file.
    g.drawImageAt(imgBack, 0, 0);

    //1.01 Draw our control HEADERS.
    g.setColour(juce::Colours::white);
    g.setFont(12.0f);
    g.drawFittedText("Gain",       0, 5, 80, 15, juce::Justification::centred, 1);
    g.drawFittedText("Thump",    100, 5, 80, 15, juce::Justification::centred, 1);
    g.drawFittedText("Low Pass", 200, 5, 80, 15, juce::Justification::centred, 1);
    g.drawFittedText("EQ Band",  300, 5, 80, 15, juce::Justification::centred, 1);

    g.drawFittedText(UIS_Band1 + " Hz", 0, 125, 80, 15, juce::Justification::centred, 1);
    g.drawFittedText(UIS_Band2 + " Hz", 100, 125, 80, 15, juce::Justification::centred, 1);
    g.drawFittedText(UIS_Band3 + " Hz", 200, 125, 80, 15, juce::Justification::centred, 1);
    g.drawFittedText(UIS_Band4 + " Hz", 300, 125, 80, 15, juce::Justification::centred, 1);
    g.drawFittedText(UIS_Band5 + " Hz", 400, 125, 80, 15, juce::Justification::centred, 1);

    //R1.01 Draw Version number.
    g.setColour(juce::Colour(0xFFA0A0A0));
    g.setFont(10.0f);
    g.drawFittedText("Ver:1.01", 395, 75, 80, 15, juce::Justification::centred, 1);
    g.setFont(12.0f);
    g.setColour(juce::Colours::white);
}

void MakoBiteAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    //R1.01 Layout our slider controls.
    jsP1_Gain.setBounds (  0, 15, 80, 100);
    jsP1_Thump.setBounds(100, 15, 80, 100);
    jsP1_LP.setBounds   (200, 15, 80, 100);
    jsP1_Band.setBounds (300, 15, 80, 100);
    jsP1_Band1.setBounds(  0, 140, 80, 100);
    jsP1_Band2.setBounds(100, 140, 80, 100);
    jsP1_Band3.setBounds(200, 140, 80, 100);
    jsP1_Band4.setBounds(300, 140, 80, 100);
    jsP1_Band5.setBounds(400, 140, 80, 100);
}

void MakoBiteAudioProcessorEditor::GUI_Init_Large_Slider(juce::Slider* slider)
{
    //R1.01 Function to make it easier to define our look. 
    //R1.01 JUCE pros will use the LookAndFeel class.
    slider->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider->setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xFFA0A0A0));
    slider->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::textBoxHighlightColourId, juce::Colour(0xFF600000));
    slider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xFF5085E8));
    slider->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::thumbColourId, juce::Colour(colorButton));
}

void MakoBiteAudioProcessorEditor::GUI_Init_Small_Slider(juce::Slider* slider)
{
    slider->setSliderStyle(juce::Slider::LinearHorizontal);
    slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    slider->setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xFFA0A0A0));
    slider->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xFF202020));
    slider->setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::textBoxHighlightColourId, juce::Colour(0xFF600000));
    slider->setColour(juce::Slider::trackColourId, juce::Colour(0xFFC00000));
    slider->setColour(juce::Slider::backgroundColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::thumbColourId, juce::Colour(colorButton));
}

void MakoBiteAudioProcessorEditor::Band_SetFilterValues(bool ForcePaint)
{
  //R1.01 Define the user selected EQ mode.
  switch (audioProcessor.Pedal_Band)
  {
    default:
    {
        audioProcessor.Band1_Freq = 150.0f;
        audioProcessor.Band2_Freq = 300.0f;
        audioProcessor.Band3_Freq = 750.0f;
        audioProcessor.Band4_Freq = 1500.0f;
        audioProcessor.Band5_Freq = 3000.0f;
        audioProcessor.Band1_Q = .707f;
        audioProcessor.Band2_Q = 1.414f;
        audioProcessor.Band3_Q = 1.414f;
        audioProcessor.Band4_Q = 1.414f;
        audioProcessor.Band5_Q = 1.414f;
        break;
    }
    case 1:
    {
        audioProcessor.Band1_Freq = 150.0f;
        audioProcessor.Band2_Freq = 450.0f;
        audioProcessor.Band3_Freq = 900.0f;
        audioProcessor.Band4_Freq = 1800.0f;
        audioProcessor.Band5_Freq = 3500.0f;
        audioProcessor.Band1_Q = .707f;
        audioProcessor.Band2_Q = 1.414f;
        audioProcessor.Band3_Q = 1.414f;
        audioProcessor.Band4_Q = 1.414f;
        audioProcessor.Band5_Q = 1.414f;
        break;
    }
    case 2:
    {
        audioProcessor.Band1_Freq = 80.0f;
        audioProcessor.Band2_Freq = 220.0f;
        audioProcessor.Band3_Freq = 750.0f;
        audioProcessor.Band4_Freq = 2200.0f;
        audioProcessor.Band5_Freq = 6000.0f;
        audioProcessor.Band1_Q = .707f;
        audioProcessor.Band2_Q = 1.414f;
        audioProcessor.Band3_Q = 1.414f;
        audioProcessor.Band4_Q = 1.414f;
        audioProcessor.Band5_Q = 1.414f;
        break;
    }
    case 3:
    {
        audioProcessor.Band1_Freq = 80.0f;
        audioProcessor.Band2_Freq = 350.0f;
        audioProcessor.Band3_Freq = 900.0f;
        audioProcessor.Band4_Freq = 1500.0f;
        audioProcessor.Band5_Freq = 3000.0f;
        audioProcessor.Band1_Q = .707f;
        audioProcessor.Band2_Q = 1.414f;
        audioProcessor.Band3_Q = 1.414f;
        audioProcessor.Band4_Q = 1.414f;
        audioProcessor.Band5_Q = 1.414f;
        break;
    }
    case 4:
    {
        audioProcessor.Band1_Freq = 100.0f;
        audioProcessor.Band2_Freq = 400.0f;
        audioProcessor.Band3_Freq = 800.0f;
        audioProcessor.Band4_Freq = 1600.0f;
        audioProcessor.Band5_Freq = 3200.0f;
        audioProcessor.Band1_Q = .707f;
        audioProcessor.Band2_Q = 1.414f;
        audioProcessor.Band3_Q = 1.414f;
        audioProcessor.Band4_Q = 1.414f;
        audioProcessor.Band5_Q = 1.414f;
        break;
    }
    case 5:
    {
        audioProcessor.Band1_Freq = 120.0f;
        audioProcessor.Band2_Freq = 330.0f;
        audioProcessor.Band3_Freq = 660.0f;
        audioProcessor.Band4_Freq = 1320.0f;
        audioProcessor.Band5_Freq = 2500.0f;
        audioProcessor.Band1_Q = .707f;
        audioProcessor.Band2_Q = 1.414f;
        audioProcessor.Band3_Q = .707f;
        audioProcessor.Band4_Q = 1.414f;
        audioProcessor.Band5_Q = .707f;
        break;
    }
    case 6:
    {
        audioProcessor.Band1_Freq = 150.0f;
        audioProcessor.Band2_Freq = 500.0f;
        audioProcessor.Band3_Freq = 900.0f;
        audioProcessor.Band4_Freq = 1800.0f;
        audioProcessor.Band5_Freq = 5000.0f;
        audioProcessor.Band1_Q = 1.414f;
        audioProcessor.Band2_Q = .707f;
        audioProcessor.Band3_Q = 1.414f;
        audioProcessor.Band4_Q = 1.414f;
        audioProcessor.Band5_Q = .707f;
        break;
    }
    case 7:
    {
        audioProcessor.Band1_Freq = 80.0f;
        audioProcessor.Band2_Freq = 300.0f;
        audioProcessor.Band3_Freq = 650.0f;
        audioProcessor.Band4_Freq = 1500.0f;
        audioProcessor.Band5_Freq = 5000.0f;
        audioProcessor.Band1_Q = 1.414f;
        audioProcessor.Band2_Q = .707f;
        audioProcessor.Band3_Q = 2.00f;
        audioProcessor.Band4_Q = 1.414f;
        audioProcessor.Band5_Q = .707f;
        break;
    }
    case 8:
    {
        audioProcessor.Band1_Freq = 100.0f;
        audioProcessor.Band2_Freq = 400.0f;
        audioProcessor.Band3_Freq = 800.0f;
        audioProcessor.Band4_Freq = 1500.0f;
        audioProcessor.Band5_Freq = 5000.0f;
        audioProcessor.Band1_Q =  .707f;
        audioProcessor.Band2_Q =  .707f;
        audioProcessor.Band3_Q = 1.414f;
        audioProcessor.Band4_Q = 2.00f;
        audioProcessor.Band5_Q =  .35f;
        break;
    }
    case 9:
    {
        audioProcessor.Band1_Freq = 80.0f;
        audioProcessor.Band2_Freq = 500.0f;
        audioProcessor.Band3_Freq = 1000.0f;
        audioProcessor.Band4_Freq = 2000.0f;
        audioProcessor.Band5_Freq = 5000.0f;
        audioProcessor.Band1_Q =  .707f;
        audioProcessor.Band2_Q = 1.414f;
        audioProcessor.Band3_Q =  .707f;
        audioProcessor.Band4_Q =  .707f;
        audioProcessor.Band5_Q =  .350f;
        break;
    }
    case 10:
    {
        audioProcessor.Band1_Freq = 80.0f;
        audioProcessor.Band2_Freq = 250.0f;
        audioProcessor.Band3_Freq = 750.0f;
        audioProcessor.Band4_Freq = 1800.0f;
        audioProcessor.Band5_Freq = 5000.0f;
        audioProcessor.Band1_Q = 2.000f;
        audioProcessor.Band2_Q = .707f;
        audioProcessor.Band3_Q = 2.00f;
        audioProcessor.Band4_Q = 1.414f;
        audioProcessor.Band5_Q = .350f;
        break;
    }

  }

  //R1.00 Create formatted strings for our UI.
  UIS_Band1 = std::to_string(int(audioProcessor.Band1_Freq));
  UIS_Band2 = std::to_string(int(audioProcessor.Band2_Freq));
  UIS_Band3 = std::to_string(int(audioProcessor.Band3_Freq));
  UIS_Band4 = std::to_string(int(audioProcessor.Band4_Freq));
  UIS_Band5 = std::to_string(int(audioProcessor.Band5_Freq));

  //R1.01 We changed some stuff so refresh the screen/UI.
  if (ForcePaint) repaint();


}

void MakoBiteAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{    
    //R1.00 Vol / Gain
    if (slider == &jsP1_Gain) audioProcessor.Pedal_Gain = float(jsP1_Gain.getValue());
    
    //R1.00 Thump
    if (slider == &jsP1_Thump)
    {
        audioProcessor.Pedal_Thump = float(jsP1_Thump.getValue());
        audioProcessor.SettingsChanged = true;  //R1.00 We need to update settings in processor.
    }

    //R1.00 LP - Low Pass
    if (slider == &jsP1_LP)
    {
        audioProcessor.Pedal_LP = float(jsP1_LP.getValue());
        audioProcessor.SettingsChanged = true;  //R1.00 We need to update settings in processor.
    }

    //R1.00 Band
    if (slider == &jsP1_Band)
    {
        audioProcessor.Pedal_Band = int(jsP1_Band.getValue());
        Band_SetFilterValues(true);
        audioProcessor.SettingsChanged = true;  //R1.00 We need to update settings in processor.
    }

    //R1.00 Band 1
    if (slider == &jsP1_Band1)
    {
        audioProcessor.Pedal_Band1 = float(jsP1_Band1.getValue());
        audioProcessor.SettingsChanged = true;  //R1.00 We need to update settings in processor.
    }
    
    //R1.00 Band 2
    if (slider == &jsP1_Band2)
    {
        audioProcessor.Pedal_Band2 = float(jsP1_Band2.getValue());
        audioProcessor.SettingsChanged = true;  //R1.00 We need to update settings in processor.
    }

    //R1.00 Band 3
    if (slider == &jsP1_Band3)
    {
        audioProcessor.Pedal_Band3 = float(jsP1_Band3.getValue());
        audioProcessor.SettingsChanged = true;  //R1.00 We need to update settings in processor.
    }

    //R1.00 Band 4
    if (slider == &jsP1_Band4)
    {
        audioProcessor.Pedal_Band4 = float(jsP1_Band4.getValue());
        audioProcessor.SettingsChanged = true;  //R1.00 We need to update settings in processor.
    }

    //R1.00 Band 5
    if (slider == &jsP1_Band5)
    {
        audioProcessor.Pedal_Band5 = float(jsP1_Band5.getValue());
        audioProcessor.SettingsChanged = true;  //R1.00 We need to update settings in processor.
    }

}
