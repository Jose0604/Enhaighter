/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "zolzer.h"
//==============================================================================
/**
*/
class MaxiHighTerAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MaxiHighTerAudioProcessor();
    ~MaxiHighTerAudioProcessor() override;

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

    float nonLinearDSP(float in, juce::String distorsionType= juce::String("sigmoid"),float k = 1);
    float sign(float number);

    
    
    zolzer HP = zolzer(juce::String("highpass"));
    zolzer HP2 = zolzer(juce::String("highpass"));
    bool isHPActive,isOnDobleHP;

    zolzer LS = zolzer(juce::String("lowshelv"));
    bool isLSActive;

    zolzer B = zolzer(juce::String("bell"));
    bool isBActive;

    zolzer HS = zolzer(juce::String("highshelv"));
    bool isHSActive;

    zolzer LP = zolzer(juce::String("lowpass"));
    zolzer LP2 = zolzer(juce::String("lowpass"));
    bool isLPActive,isOnDobleLP;

    zolzer harmonicHP = zolzer(juce::String("highpass"));
    bool isHEActive;
    float d, sat, h;

    bool isPluginOn;
    bool isEQOn;
    bool invert;
private:
    float e = juce::MathConstants<float>::euler;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MaxiHighTerAudioProcessor)
};
