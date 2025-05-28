/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
using namespace juce;

template <typename T>
int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
//==============================================================================
/**
*/
class RetroXAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    RetroXAudioProcessor();


    ~RetroXAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
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
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    void updateHighCutFilter();
    void updateLowCutFilter();
    float gain = 0.0f;
    float noiseAmount = 0.0f;
    float loFiAmount = 0.0f;
    float tremoloAmount = 0.0f;
    float chorusAmount = 0.0f;
    float highCut = 20000.0f;
    float lowCut = 20.0f;

    int noiseType = 0;
    int loFiType = 0;
    int tremoloType = 0;
    int chorusType = 0;
    
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState parameters {*this, nullptr, "Parameters", createParameterLayout()};
    float lastSampleRate = 48000;

    juce::AudioFormatManager audioFormatManager;
    juce::AudioFormatReader* reader1 = nullptr;
    juce::AudioFormatReader* reader2 = nullptr;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource1;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource2;
    std::unique_ptr<juce::AudioFormatReaderSource>* readerSource;
    juce::AudioTransportSource transportSource;
    juce::AudioBuffer<float> vinylBuffer;
    

   
private:
    
    //create a dsp::ProcessorDuplicator for each effect
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highCutFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lowCutFilter;
    juce::Random random;
    
    juce::dsp::Chorus<float> chorus;
    float f0 = 4752.456;
    float f1 = 4030.961;
    float f2 = 2784.711;
    float f3 = 1538.461;
    float f4 = 357.681;
    float f5 = 70;
    float f6 = 30;
    //----------------------
    //calculate coefficients
    double pi = juce::MathConstants<double>::pi;
    float sr = getSampleRate();
    float k0 = exp(-2 * pi * f0 / sr);
    float k1 = exp(-2 * pi * f1 / sr);
    float k2 = exp(-2 * pi * f2 / sr);
    float k3 = exp(-2 * pi * f3 / sr);
    float k4 = exp(-2 * pi * f4 / sr);
    float k5 = exp(-2 * pi * f5 / sr);
    float k6 = exp(-2 * pi * f6 / sr);
    
    //poles
    float b0, b1, b2, b3, b4, b5, b6;
    float whiteNoise;
    enum TransportState
    {
        Stop,
        Play
    };
    TransportState state;
    
    void Playing();
    void Stopped();

    float phase = 0.0f;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RetroXAudioProcessor)
};

