/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "filmStrip.h"
//==============================================================================
/**
*/
class MaxiHighTerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MaxiHighTerAudioProcessorEditor (MaxiHighTerAudioProcessor&);
    ~MaxiHighTerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
   // --------------------- EQ ----------------------------------------------------
    std::unique_ptr<FilmStripKnob> fHP, fLS, fB,     fHS, fLP;
    std::unique_ptr<FilmStripKnob> qHP, gLS, gB, qB, gHS, qLP;
    
    /*juce::Label fHigh, fLowS, fBell, fHighS, fLow;
    juce::Label qHigh, gLowS, qBell,gBell, gHighS, qLow;*/

    juce::ToggleButton highpass, lowshelv, bell, highshelv, lowpass;
    juce::ToggleButton fourGradeLP, fourGradeHP;

    // --------------------Exciter--------------------------------------------------
    std::unique_ptr<FilmStripKnob> harmonicDrive, eHPfc, harmonicSaturation,
        harmonicBlend;
    juce::ToggleButton harmonicExciter;
    juce::ToggleButton bypass;
    juce::ToggleButton EQ;
    juce::ToggleButton invertir;

    MaxiHighTerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MaxiHighTerAudioProcessorEditor)
};
