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
class CustomSliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomSliderLookAndFeel(juce::Colour track, juce::Colour thumb);

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;



private:
    juce::Colour trackColour;
    juce::Colour thumbColour;
};
class RetroXAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                    
                                    public juce::ComboBox::Listener
{
public:
    RetroXAudioProcessorEditor (RetroXAudioProcessor&);
    ~RetroXAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    /*void sliderValueChanged(juce::Slider* slider) override;*/
    void comboBoxChanged(juce::ComboBox* comboBox) override;
    juce::Slider noise, loFi, tremolo, chorus, outputGain, highCut, lowCut;
    CustomSliderLookAndFeel noiseLookAndFeel, wobbleLookAndFeel, tremoloLookAndFeel, chorusLookAndFeel;
    juce::Label noiseLabel, loFiLabel, tremoloLabel, chorusLabel, outputGainLabel, highCutLowCutLabel1, highCutLowCutLabel2, retroLabel;

    //Make comboboxes for each of the 4 effects
    juce::ComboBox noiseType, loFiType, tremoloType, chorusType;

    //make sliderAttachments for each of the effects
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> loFiAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tremoloAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highCutAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowCutAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> noiseTypeAttachment, loFiTypeAttachment, tremoloTypeAttachment, chorusTypeAttachment;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RetroXAudioProcessor& audioProcessor;

    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RetroXAudioProcessorEditor)
};
