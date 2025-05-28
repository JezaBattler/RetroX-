/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
using namespace juce;

//==============================================================================
RetroXAudioProcessorEditor::RetroXAudioProcessorEditor (RetroXAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    noiseLookAndFeel(Colours::darkgoldenrod, Colours::beige),
    wobbleLookAndFeel(Colours::darkgreen, Colours::yellow),
    tremoloLookAndFeel(Colours::darkred, Colours::orange),
    chorusLookAndFeel(Colours::darkblue, Colours::magenta)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 350);
    setResizable(false, false);


    noiseLabel.setText("Noise", juce::dontSendNotification);
    loFiLabel.setText("Lo-Fi", juce::dontSendNotification);
    tremoloLabel.setText("Tremolo", juce::dontSendNotification);
    chorusLabel.setText("Chorus", juce::dontSendNotification);

    outputGainLabel.setText("Gain", juce::dontSendNotification);
    highCutLowCutLabel1.setText("High Cut", juce::dontSendNotification);
    highCutLowCutLabel2.setText("Low Cut", juce::dontSendNotification);

    retroLabel.setText("RetroX by PJ Audio", juce::dontSendNotification);

    noiseLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::darkgoldenrod);
    loFiLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::darkgreen);
    tremoloLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::darkred);
    chorusLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::darkblue);

    outputGainLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::beige);
    highCutLowCutLabel1.setColour(juce::Label::ColourIds::textColourId, juce::Colours::beige);
    highCutLowCutLabel2.setColour(juce::Label::ColourIds::textColourId, juce::Colours::beige);
    retroLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::beige);

    juce::Font customFont("Arial", 16.0f, juce::Font::bold);
    noiseLabel.setFont(customFont);
    loFiLabel.setFont(customFont);
    tremoloLabel.setFont(customFont);
    chorusLabel.setFont(customFont);

    outputGainLabel.setFont(customFont);
    highCutLowCutLabel1.setFont(customFont);
    highCutLowCutLabel2.setFont(customFont);

    juce::Font customFont2 = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::Rooster_ttf, BinaryData::Rooster_ttfSize));
    customFont2.setHeight(70.0f);
    retroLabel.setFont(customFont2);

    noiseLabel.attachToComponent(&noise, false);
    loFiLabel.attachToComponent(&loFi, false);
    tremoloLabel.attachToComponent(&tremolo, false);
    chorusLabel.attachToComponent(&chorus, false);

    outputGainLabel.attachToComponent(&outputGain, false);
    highCutLowCutLabel1.attachToComponent(&highCut, false);
    highCutLowCutLabel2.attachToComponent(&lowCut, false);




    noiseLabel.setJustificationType(juce::Justification::centred);
    loFiLabel.setJustificationType(juce::Justification::centred);
    tremoloLabel.setJustificationType(juce::Justification::centred);
    chorusLabel.setJustificationType(juce::Justification::centred);

    outputGainLabel.setJustificationType(juce::Justification::centredTop);
    highCutLowCutLabel1.setJustificationType(juce::Justification::centredTop);
    retroLabel.setJustificationType(juce::Justification::centredTop);
    highCutLowCutLabel2.setJustificationType(juce::Justification::centredTop);


    //Creating the sliders





    noise.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    loFi.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    tremolo.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    chorus.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);

    outputGain.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    highCut.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    lowCut.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);



    noise.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    loFi.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    tremolo.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    chorus.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    //Add text below the slider outputGain
    outputGain.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    highCut.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 15);
    lowCut.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 15);


    outputGain.setDoubleClickReturnValue(true, 0.0f);
    outputGain.setSkewFactor(2.0f);


    highCut.setValue(20000.0f);
    lowCut.setValue(20.0f);
    outputGain.setValue(0.0f);


    noise.setRange(0.0f, 1.0f, 0.01f);
    loFi.setRange(0.0f, 1.0f, 0.01f);
    tremolo.setRange(0.0f, 1.0f, 0.01f);
    chorus.setRange(0, 1.0f, 0.01f);
    outputGain.setRange(-48.0f, 10.0f, 0.01f);

    /*highCut.setRange(20.0f, 20000.0f, 1.0f);
    highCut.setSkewFactorFromMidPoint(5000.0f);
    lowCut.setRange(20.0f, 20000.0f, 1.0f);
    lowCut.setSkewFactorFromMidPoint(5000.0f);*/
    








    noise.setLookAndFeel(&noiseLookAndFeel);
    loFi.setLookAndFeel(&wobbleLookAndFeel);
    tremolo.setLookAndFeel(&tremoloLookAndFeel);
    chorus.setLookAndFeel(&chorusLookAndFeel);


    //create comboboxes for each of the 4 effects
    addAndMakeVisible(noiseType);
    addAndMakeVisible(loFiType);
    addAndMakeVisible(tremoloType);
    addAndMakeVisible(chorusType);

    //Add items to the comboboxes
    noiseType.addItem("White", 1);
    noiseType.addItem("Pink", 2);
    

    loFiType.addItem("4bits", 1);
    loFiType.addItem("2bits", 2);

    tremoloType.addItem("Tremolo", 1);
    tremoloType.addItem("Tremolino", 2);

    chorusType.addItem("Strong", 1);
    chorusType.addItem("Futura Obscura", 2);
    chorusType.addItem("Chorusus", 3);

    //Set the default selected item
    noiseType.setSelectedId(1);
    loFiType.setSelectedId(1);
    tremoloType.setSelectedId(1);
    chorusType.setSelectedId(1);

    //ADD LISTENERS TO THE COMBOBOXES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //ComboBoxes are ugly so I am making them pretty
    noiseType.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::beige);
    noiseType.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colours::darkgoldenrod);
    noiseType.setColour(juce::ComboBox::ColourIds::arrowColourId, juce::Colours::darkgoldenrod);
    noiseType.setColour(juce::ComboBox::ColourIds::textColourId, juce::Colours::darkgoldenrod);
    noiseType.setColour(juce::ComboBox::ColourIds::buttonColourId, juce::Colours::darkgoldenrod);
    noiseType.setColour(juce::ComboBox::ColourIds::focusedOutlineColourId, juce::Colours::darkgoldenrod);

    loFiType.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::beige);
    loFiType.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colours::darkgreen);
    loFiType.setColour(juce::ComboBox::ColourIds::arrowColourId, juce::Colours::darkgreen);
    loFiType.setColour(juce::ComboBox::ColourIds::textColourId, juce::Colours::darkgreen);
    loFiType.setColour(juce::ComboBox::ColourIds::buttonColourId, juce::Colours::darkgreen);
    loFiType.setColour(juce::ComboBox::ColourIds::focusedOutlineColourId, juce::Colours::darkgreen);

    tremoloType.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::beige);
    tremoloType.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colours::darkred);
    tremoloType.setColour(juce::ComboBox::ColourIds::arrowColourId, juce::Colours::darkred);
    tremoloType.setColour(juce::ComboBox::ColourIds::textColourId, juce::Colours::darkred);
    tremoloType.setColour(juce::ComboBox::ColourIds::buttonColourId, juce::Colours::darkred);
    tremoloType.setColour(juce::ComboBox::ColourIds::focusedOutlineColourId, juce::Colours::darkred);

    chorusType.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::beige);
    chorusType.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colours::darkblue);
    chorusType.setColour(juce::ComboBox::ColourIds::arrowColourId, juce::Colours::darkblue);
    chorusType.setColour(juce::ComboBox::ColourIds::textColourId, juce::Colours::darkblue);
    chorusType.setColour(juce::ComboBox::ColourIds::buttonColourId, juce::Colours::darkblue);
    chorusType.setColour(juce::ComboBox::ColourIds::focusedOutlineColourId, juce::Colours::darkblue);

    


    /*sizeComboBox.addSeparator();
    sizeComboBox.addItem("100%", 1);
    sizeComboBox.addItem("150%", 1.5);
    sizeComboBox.addItem("200%", 2);*/


    /*sizeComboBox.onChange = [this]()
    {
        scalingFactor = static_cast<int>(sizeComboBox.getSelectedId());
        resized();
    };
    sizeComboBox.setSelectedId(1);*/

    outputGain.setColour(juce::Slider::thumbColourId, juce::Colours::black);  // Set thumb color
    outputGain.setColour(juce::Slider::trackColourId, juce::Colours::beige);  // Set track color

    highCut.setColour(juce::Slider::thumbColourId, juce::Colours::black);  // Set thumb color
    highCut.setColour(juce::Slider::trackColourId, juce::Colours::beige);  // Set track color
    lowCut.setColour(juce::Slider::thumbColourId, juce::Colours::black);  // Set thumb color
    lowCut.setColour(juce::Slider::trackColourId, juce::Colours::beige);  // Set track color

    
    



    addAndMakeVisible(noise);
    addAndMakeVisible(loFi);
    addAndMakeVisible(tremolo);
    addAndMakeVisible(chorus);

    addAndMakeVisible(outputGain);
    addAndMakeVisible(highCut);
    addAndMakeVisible(lowCut);

    addAndMakeVisible(noiseLabel);
    addAndMakeVisible(loFiLabel);
    addAndMakeVisible(tremoloLabel);
    addAndMakeVisible(chorusLabel);

    addAndMakeVisible(outputGainLabel);
    addAndMakeVisible(highCutLowCutLabel1);
    addAndMakeVisible(highCutLowCutLabel2);
    addAndMakeVisible(retroLabel);

    
    outputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "gain", outputGain);
    highCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "highCut", highCut);
    lowCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "lowCut", lowCut);
    noiseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "noise", noise);
    loFiAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "loFi", loFi);
    tremoloAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "tremolo", tremolo);
    chorusAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "chorus", chorus);
    noiseTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.parameters, "noiseType", noiseType);
    loFiTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.parameters, "loFiType", loFiType);
    tremoloTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.parameters, "tremoloType", tremoloType);
    chorusTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.parameters, "chorusType", chorusType);

    

}

void RetroXAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBox)
{
    if (comboBox == &noiseType)
    {
        audioProcessor.noiseType = noiseType.getSelectedId();
    }
    else if (comboBox == &loFiType)
    {
        audioProcessor.loFiType = loFiType.getSelectedId();
    }
    else if (comboBox == &tremoloType)
    {
        audioProcessor.tremoloType = tremoloType.getSelectedId();
    }
    else if (comboBox == &chorusType)
    {
        audioProcessor.chorusType = chorusType.getSelectedId();
    }
}

RetroXAudioProcessorEditor::~RetroXAudioProcessorEditor()
{
    noise.setLookAndFeel(nullptr);
    loFi.setLookAndFeel(nullptr);
    tremolo.setLookAndFeel(nullptr);
    chorus.setLookAndFeel(nullptr);
}

//==============================================================================
void RetroXAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    //fill the whole screen with a picture in my resources
    Image backgroundImage = ImageCache::getFromMemory(BinaryData::bckgr2_jpg, BinaryData::bckgr2_jpgSize);
    g.drawImage(backgroundImage, 0, 0, getWidth(), getHeight(), 0, 0, backgroundImage.getWidth(), backgroundImage.getHeight());
}

void RetroXAudioProcessorEditor::resized()
{
    const int border = 15;
    const int sliderWidth = 100;
    const int sliderHeight = 110;

    noise.setBounds(border, 220, sliderWidth, sliderHeight);
    loFi.setBounds(border + 110, 220, sliderWidth, sliderHeight);
    tremolo.setBounds(border + 220, 220, sliderWidth, sliderHeight);
    chorus.setBounds(border + 330, 220, sliderWidth, sliderHeight);
    outputGain.setBounds(border + 415, border * 2, sliderWidth, 350 - (2 * border));
    highCut.setBounds(border + 495, border * 2, sliderWidth, 210 - (4 * border));
    lowCut.setBounds(border + 495, 170 + (2 * border), sliderWidth, 210 - (4 * border));

   
    retroLabel.setBounds(0, 0, 400, 80);

    noiseType.setBounds(border, 120, sliderWidth - 5, 40);
    loFiType.setBounds(border + 110, 120, sliderWidth - 5, 40);
    tremoloType.setBounds(border + 220, 120, sliderWidth - 5, 40);
    chorusType.setBounds(border + 330, 120, sliderWidth - 5, 40);
}
CustomSliderLookAndFeel::CustomSliderLookAndFeel(juce::Colour track, juce::Colour thumb)
    : trackColour(track), thumbColour(thumb) {}

void CustomSliderLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float rotaryStartAngle,
    float rotaryEndAngle, juce::Slider& slider)
{

    // Draw a cool background for the rotary slider
     // Draw an outline around the slider
    g.setGradientFill(juce::ColourGradient(trackColour, x, y, thumbColour, x + width, y + height, false));
    g.fillEllipse(x, y, width, height);

    // Draw a line for the rotary position
    juce::Point<float> center(x + width * 0.5f, y + height * 0.5f);
    juce::Point<float> thumbPoint = center.getPointOnCircumference(height * 0.5f, rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle));

    //Draw the line from the center to the thumb
    g.setColour(juce::Colours::black);
    g.drawLine(std::abs(center.getX() + thumbPoint.getX()) / 2, std::abs(center.getY() + thumbPoint.getY()) / 2, thumbPoint.getX(), thumbPoint.getY(), 3.0f);
}