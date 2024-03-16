/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
MaxiHighTerAudioProcessor::MaxiHighTerAudioProcessor()
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
    isPluginOn = true;
    isEQOn = true;

    invert = false;

    isHPActive = false;
    isOnDobleHP = false;

    isLSActive = false;
    isBActive = false;
    isHSActive = false;

    isLPActive = false;
    isOnDobleLP = false;
    
    d = 1;
    sat = 0.75;
    h = 0;
    /*float x[5];
    for (int i = 0; i < 5; i++)
    {
        x[i] = nonLinearDSP((float)i/4,juce::String("hardclip"));
    }*/
    isHEActive = false;
}

MaxiHighTerAudioProcessor::~MaxiHighTerAudioProcessor()
{
}

//==============================================================================
const juce::String MaxiHighTerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MaxiHighTerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MaxiHighTerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MaxiHighTerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MaxiHighTerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MaxiHighTerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MaxiHighTerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MaxiHighTerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MaxiHighTerAudioProcessor::getProgramName (int index)
{
    return {};
}

void MaxiHighTerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MaxiHighTerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    int x = getTotalNumInputChannels();

    HP.updateNumInputChannels(x);
    HP.updateSampleRate(sampleRate);
    HP2.updateNumInputChannels(x);
    HP2.updateSampleRate(sampleRate);

    LS.updateNumInputChannels(x);
    LS.updateSampleRate(sampleRate);

    B.updateNumInputChannels(x);
    B.updateSampleRate(sampleRate);

    HS.updateNumInputChannels(x);
    HS.updateSampleRate(sampleRate);

    LP.updateNumInputChannels(x);
    LP.updateSampleRate(sampleRate);
    LP2.updateNumInputChannels(x);
    LP2.updateSampleRate(sampleRate);

    harmonicHP.updateNumInputChannels(x);
    harmonicHP.updateSampleRate(sampleRate);
}

void MaxiHighTerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MaxiHighTerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void MaxiHighTerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            float* out = buffer.getWritePointer(channel);
            if(isPluginOn)
            {
                if(!invert)
                {
                    if (isEQOn)
                    {
                        out[sample] = HP.filter(out[sample], channel, isHPActive);
                        if (isOnDobleHP)
                            out[sample] = HP2.filter(out[sample], channel, isHPActive);

                        out[sample] = LS.filter(out[sample], channel, isLSActive);
                        out[sample] = B.filter(out[sample], channel, isBActive);
                        out[sample] = HS.filter(out[sample], channel, isHSActive);

                        out[sample] = LP.filter(out[sample], channel, isLPActive);
                        if (isOnDobleLP)
                            out[sample] = LP2.filter(out[sample], channel, isLPActive);

                    }
                    if (isHEActive)
                    {
                        float processLine = harmonicHP.filter(out[sample], channel, true) * d;
                        processLine = nonLinearDSP((float)processLine, juce::String("sigmoid"), sat);
                        out[sample] = h * processLine + (h - 1) * out[sample];
                    }
                }
                else
                {
                    if (isHEActive)
                    {
                        float processLine = harmonicHP.filter(out[sample], channel, true) * d;
                        processLine = nonLinearDSP((float)processLine, juce::String("sigmoid"), sat);
                        out[sample] = h * processLine + (h - 1) * out[sample];
                    }
                    if (isEQOn)
                    {
                        out[sample] = HP.filter(out[sample], channel, isHPActive);
                        if (isOnDobleHP)
                            out[sample] = HP2.filter(out[sample], channel, isHPActive);

                        out[sample] = LS.filter(out[sample], channel, isLSActive);
                        out[sample] = B.filter(out[sample], channel, isBActive);
                        out[sample] = HS.filter(out[sample], channel, isHSActive);

                        out[sample] = LP.filter(out[sample], channel, isLPActive);
                        if (isOnDobleLP)
                            out[sample] = LP2.filter(out[sample], channel, isLPActive);

                    }
                }
            }
                            
        }
    }
}

//==============================================================================
bool MaxiHighTerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MaxiHighTerAudioProcessor::createEditor()
{
    return new MaxiHighTerAudioProcessorEditor (*this);
}

//==============================================================================
void MaxiHighTerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MaxiHighTerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

float MaxiHighTerAudioProcessor::nonLinearDSP(float in, juce::String distorsionType, float k)
{
    float out;
    if (distorsionType == "hardclip")
    {
        if (abs(in) > 1)
        { 
            out =  sign(in);
        }
        else
        {
            out = in;
        }
    }
    else if(distorsionType == "arraya")
    {
        out = 3 * in / 2 * (1 - pow(in, 2) / 3);
    }
    else if (distorsionType == "sigmoid")
    {
        out = -1 + 2 / (1 + pow(e, -k * in));
    }
    else if (distorsionType == "sigmoid2")
    {
        out = (pow(e, in) - 1) * (e + 1) / ((pow(e, in) + 1) * (e - 1));
    }
    else if (distorsionType == "hyperbolic")
    {
        out = tanh(k * in) / tan(k);
    }
    else if(distorsionType == "arctangent")
    {
        out = atan(k * in) / atan(k);
    }
    else if (distorsionType == "fuzz")
    {
        out = sign(in) * (1 - pow(e, -abs(k * in))) / (1 - pow(e, -k));
    }
    else if (distorsionType == "fuzz2")
    {
        out = sign(-in) * (1 - pow(e, abs(in))) / (e - 1);
    }
    else if (distorsionType == "exp")
    {
        out = (e - pow(e, 1 - in)) / (e - 1);
    }
    else if (distorsionType == "square")
    {
        out = sign(in) * pow(in, 2);
    }
   
    return out;
}

float MaxiHighTerAudioProcessor::sign(float number)
{
    return number>0?1:-1;
}

//template <typename T> int sgn(T val) {
//    return (T(0) < val) - (val < T(0));
//}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MaxiHighTerAudioProcessor();
}
