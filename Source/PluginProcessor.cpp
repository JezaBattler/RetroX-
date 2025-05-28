/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
using namespace juce;

//==============================================================================
RetroXAudioProcessor::RetroXAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )

                          
                              						  
#endif
{
    
    audioFormatManager.registerBasicFormats();
    
    

    juce::File sourceFile = juce::File(__FILE__);
    juce::File sourceDirectory = sourceFile.getParentDirectory();

    // Load Vinyl1.wav
    juce::File audioFile1 = sourceDirectory.getChildFile("Vinyl1.wav");
    reader1 = audioFormatManager.createReaderFor(audioFile1);
    if (reader1 != nullptr)
    {
        readerSource1 = std::make_unique<juce::AudioFormatReaderSource>(reader1, true);
        readerSource1->setLooping(true);
    }

    // Load Vinyl2.wav
    juce::File audioFile2 = sourceDirectory.getChildFile("Vinyl2.wav");
    reader2 = audioFormatManager.createReaderFor(audioFile2);
    if (reader2 != nullptr)
    {
        readerSource2 = std::make_unique<juce::AudioFormatReaderSource>(reader2, true);
        readerSource2->setLooping(true);
    }


    

}

RetroXAudioProcessor::~RetroXAudioProcessor()
{
}

//==============================================================================


juce::AudioProcessorValueTreeState::ParameterLayout  RetroXAudioProcessor::createParameterLayout()
{
    AudioProcessorValueTreeState :: ParameterLayout layout;
	layout.add(std::make_unique<AudioParameterFloat>("gain", "Gain", NormalisableRange<float>(-48.0f, 10.0f, 0.01f,2.0f), 0.0f));
    layout.add(std::make_unique<AudioParameterFloat>("noise", "Noise", NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));
    layout.add(std::make_unique<AudioParameterFloat>("loFi", "LoFi", NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));
    layout.add(std::make_unique<AudioParameterFloat>("tremolo", "Tremolo", NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));
    layout.add(std::make_unique<AudioParameterFloat>("chorus", "Chorus", NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));
    layout.add(std::make_unique<AudioParameterFloat>("highCut", "HighCut", NormalisableRange<float>(20.0f, 20000.0f, 10.0f, 0.35f), 20000.0f));
    layout.add(std::make_unique<AudioParameterFloat>("lowCut", "LowCut", NormalisableRange<float>(20.0f, 20000.0f, 10.0f, 0.35f), 20.0f));
    layout.add(std::make_unique<AudioParameterChoice>("noiseType", "NoiseType", StringArray{ "White", "Pink"}, 0));
    layout.add(std::make_unique<AudioParameterChoice>("loFiType", "LoFiType", StringArray{"4bits", "2bits" }, 0));
    layout.add(std::make_unique<AudioParameterChoice>("tremoloType", "TremoloType", StringArray{ "Tremolo1", "Tremolo2" }, 0));
    layout.add(std::make_unique<AudioParameterChoice>("chorusType", "ChorusType", StringArray{ "Chorus1", "Chorus2", "Chorusus"}, 0));

    

    return layout;
}
const juce::String RetroXAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RetroXAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RetroXAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RetroXAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RetroXAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RetroXAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RetroXAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RetroXAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String RetroXAudioProcessor::getProgramName (int index)
{
    return {};
}

void RetroXAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void RetroXAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    lastSampleRate = sampleRate;

    dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    highCutFilter.prepare(spec);
    highCutFilter.reset();
    lowCutFilter.prepare(spec);
    lowCutFilter.reset();
    chorus.prepare(spec);
    chorus.reset();
    
    
    
}

void RetroXAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RetroXAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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
void RetroXAudioProcessor::updateHighCutFilter()
{
    highCut = *parameters.getRawParameterValue("highCut");
    *highCutFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, highCut, 4.0f);
}
//method to update the low cut filter
void RetroXAudioProcessor::updateLowCutFilter()
{
    lowCut = *parameters.getRawParameterValue("lowCut");
    *lowCutFilter.state = *dsp::IIR::Coefficients<float>::makeHighPass(lastSampleRate, lowCut, 4.0f);
}
void RetroXAudioProcessor::Playing()
{
    state = Play;
	transportSource.start();
}
void RetroXAudioProcessor::Stopped()
{
	state = Stop;
	transportSource.stop();

}

void RetroXAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //Create a floats for the tremolo
    float frequency = 0.1f;
    float frequency2 = 0.3f;
    
    float tremoloDepth1 = 0.5f; 
    float offset = 0.5f;

    float tremoloDepth2 = 0.4f; 
    float offset2 = 0.6f;
    


    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    dsp::AudioBlock<float> block(buffer);
    
    dsp::ProcessContextReplacing<float> context(block);
    
    updateHighCutFilter();
    updateLowCutFilter();
 
    //get the gain from the slider
    gain = *parameters.getRawParameterValue("gain");
    float outputGain = *parameters.getRawParameterValue("gain"); 
    gain = pow(10, outputGain / 20);
    //get the noise type form the combobox
    noiseType = *parameters.getRawParameterValue("noiseType");
    loFiType = *parameters.getRawParameterValue("loFiType");
    tremoloType = *parameters.getRawParameterValue("tremoloType");
    chorusType = *parameters.getRawParameterValue("chorusType");


    float originalSample;
    //create a temp buffer to store the noise


    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {

        /*if (noiseType == 2 && readerSource1)
        {

            readerSource1->getNextAudioBlock(info);
        }
        else if (noiseType == 3 && readerSource2)
        {

            readerSource2->getNextAudioBlock(info);
        }*/

        auto* channelData = buffer.getWritePointer (channel);
        float* data = buffer.getWritePointer(channel);
        noiseAmount = *parameters.getRawParameterValue("noise");
        loFiAmount = *parameters.getRawParameterValue("loFi");
        tremoloAmount = *parameters.getRawParameterValue("tremolo");
        chorusAmount = *parameters.getRawParameterValue("chorus");
        for (int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            /*if (noiseType == 0 || noiseType == 1)
            {
                readerSource1.reset();
                readerSource2.reset();
            }*/

            if (noiseType == 0)
			{
				whiteNoise = (random.nextFloat() * 0.25 - 0.2) * noiseAmount;
			}
			else if (noiseType == 1)
			{
                
                float white = random.nextFloat() * 0.25 - 0.2;
                //white - noise input
                b0 = 0.99886 * b0 + white * 0.0555179;
                b1 = 0.99332 * b1 + white * 0.0750759;
                b2 = 0.96900 * b2 + white * 0.1538520;
                b3 = 0.86650 * b3 + white * 0.3104856;
                b4 = 0.55000 * b4 + white * 0.5329522;
                b5 = -0.7616 * b5 - white * 0.0168980;
                float pink = b0 + b1 + b2 + b3 + b4 + b5 + b6 + white * 0.5362;
                b6 = white * 0.115926;
                whiteNoise = pink * noiseAmount / 10;
			}
            float originalSample = buffer.getSample(channel, sample) + whiteNoise;
            float drySample = buffer.getSample(channel, sample) + whiteNoise;
            
            //lofitype 0 is 8 bit
            
            if (loFiType == 0 && loFiAmount > 0)
            {
                if (originalSample < 0.00001 && originalSample > -0.00001)
                {
                    originalSample = 0.0;
                }
                else  
                {
                    
                    float bits = 4.0f;
                    float factor = pow(2, bits);
                    
                    originalSample = originalSample * factor;
                    originalSample = (floor(originalSample) / factor);
                    
                }            

				channelData[sample] = originalSample * loFiAmount + drySample * (1 - loFiAmount);   
                
			}
            else if (loFiType == 1 && loFiAmount > 0)
            {
                if (originalSample < 0.00001 && originalSample > -0.00001)
                {
                    originalSample = 0.0;
                }
                else
                {

                    float bits = 2.0f;
                    float factor = pow(2, bits);

                    originalSample = originalSample * factor;
                    originalSample = (floor(originalSample) / factor);

                }

                channelData[sample] = originalSample * loFiAmount + drySample * (1 - loFiAmount);
                
            }
            else
            {
				channelData[sample] = originalSample;
			}
            if (tremoloAmount > 0) 
            {
                if (tremoloType == 0)
                {
                    //tremolo effect
                    float modulation = offset + tremoloDepth1 * sin(2.0f * pi * phase);
                    channelData[sample] =  channelData[sample] * modulation * tremoloAmount + channelData[sample] * (1 - tremoloAmount);
                    phase += frequency * 2.0f * pi / getSampleRate();


                   
                    if (phase >= 2.0f * pi)
                    {

                        phase -= 2.0f * pi;
                    }
				}
                else if (tremoloType == 1)
                {
					//tremolo effect
                    float modulation = offset2 + tremoloDepth2 * sin(2.0f * pi * phase);
                    channelData[sample] = channelData[sample] * modulation * tremoloAmount + channelData[sample] * (1 - tremoloAmount);
                    phase += frequency2 * 2.0f * pi / getSampleRate();   

                    if (phase >= 2.0f * pi)
                    {

                        phase -= 2.0f * pi;
                    }
				}

            }
            
            channelData[sample] *= gain;
            
        }
        
        
    }
    //use the chorus effect
    if (chorusAmount > 0)
    {
        if (chorusType == 0)
        {
            chorus.setCentreDelay(7.0f);
            chorus.setFeedback(0.7f);
            chorus.setDepth(0.5f);
            chorus.setRate(1.3f);
		}
        else if (chorusType == 1)
        {
			chorus.setRate(0.5f);
			chorus.setDepth(0.7f);
			chorus.setFeedback(0.5f);
			chorus.setCentreDelay(8.0f);
		}
        else
        {
            chorus.setRate(4.3f);
			chorus.setDepth(0.1f);
			chorus.setFeedback(0.5f);
			chorus.setCentreDelay(5.0f);
        }
		chorus.setMix(chorusAmount);
		chorus.process(context);
	}
    highCutFilter.process(context);
    lowCutFilter.process(context);
    
    
}

//==============================================================================
bool RetroXAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* RetroXAudioProcessor::createEditor()
{
    return new RetroXAudioProcessorEditor (*this);
}

//==============================================================================
void RetroXAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream stream(destData, true);
    parameters.state.writeToStream(stream);
}

void RetroXAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if(tree.isValid())
	{
		parameters.state = tree;
	}
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RetroXAudioProcessor();
}
