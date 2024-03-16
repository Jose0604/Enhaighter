/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//#include "zolzer.h"

//==============================================================================
MaxiHighTerAudioProcessorEditor::MaxiHighTerAudioProcessorEditor (MaxiHighTerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    uint8_t zero = 0;
    juce::Colour black(zero, zero, zero);

    /* Knobs*/
    // -----------------------------------------------------------------------------
    juce::Image fknob = juce::ImageFileFormat::loadFrom(BinaryData::Donald_Duck_png,
        BinaryData::Donald_Duck_pngSize);
    juce::Image qknob = juce::ImageFileFormat::loadFrom(BinaryData::_7up_png,
        BinaryData::_7up_pngSize);
    juce::Image gKnob = juce::ImageFileFormat::loadFrom(BinaryData::knob_png,
        BinaryData::knob_pngSize);
    juce::Image gainKnob = juce::ImageFileFormat::loadFrom(BinaryData::Felix_Orange_png,
        BinaryData::Felix_Orange_pngSize);
    juce::Image dryWet = juce::ImageFileFormat::loadFrom(BinaryData::Royal_Flush_png, BinaryData::Royal_Flush_pngSize);
    // -----------------------------------------------------------------------------

    addAndMakeVisible(bypass);
    bypass.setButtonText("Bypass");
    bypass.setToggleState(false, juce::dontSendNotification);
    bypass.onClick = [this]
    {
        audioProcessor.isPluginOn = !bypass.getToggleState();
    };

    addAndMakeVisible(invertir);
    invertir.setButtonText("Invert");
    invertir.setToggleState(false, juce::dontSendNotification);
    invertir.onClick = [this]
    {
        audioProcessor.invert = invertir.getToggleState();
    };

    addAndMakeVisible(EQ);
    EQ.setButtonText("EQ");
    EQ.setColour(juce::ToggleButton::textColourId, juce::Colour(black));
    EQ.setToggleState(true, juce::dontSendNotification);
    EQ.onClick = [this]
    {
        audioProcessor.isEQOn = EQ.getToggleState();
    };

    const juce::String freq = juce::String("frequency");
    /* Modificaciones de interfaz gráfica de interacción de cambio de valor */
    
    /* High pass */
    // -----------------------------------------------------------------------------
    //addAndMakeVisible(fHigh);

    addAndMakeVisible(highpass);
    highpass.setButtonText("High pass");
    highpass.onClick = [this]
    {
        audioProcessor.isHPActive = highpass.getToggleState();
    };
    
    fHP.reset(new FilmStripKnob(fknob,31,false));
    addAndMakeVisible(fHP.get());
    fHP->setRange(20, 2e3, 0.1);
    fHP->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    fHP->setTextValueSuffix("Hz");
    fHP->setValue(20, juce::dontSendNotification);
    fHP->setTooltip(freq);
    fHP->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 20);
    fHP->setSkewFactorFromMidPoint(500);
    //fHP->setTooltip("frequency");
    fHP->onValueChange = [this]
    {
        audioProcessor.HP2.updateCutFrequency(fHP->getValue());
        audioProcessor.HP.updateCutFrequency(fHP->getValue());
    };
    qHP.reset(new FilmStripKnob(qknob, 31, false));
    addAndMakeVisible(qHP.get());
    qHP->setRange(1, 3, 0.1);
    qHP->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    qHP->setValue(1, juce::dontSendNotification);
    qHP->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 20);
    qHP->onValueChange = [this]
    {
        audioProcessor.HP.updateQfactor(qHP->getValue());
        audioProcessor.HP2.updateQfactor(qHP->getValue());
    };

    addAndMakeVisible(fourGradeHP);
    fourGradeHP.setButtonText("24 dB/Oct");
    fourGradeHP.setColour(juce::ToggleButton::textColourId, juce::Colour(black));
    fourGradeHP.onClick = [this]
    {
        audioProcessor.isOnDobleHP = fourGradeHP.getToggleState();
    };
    // -----------------------------------------------------------------------------

    
    /* Low Shelv */
    // -----------------------------------------------------------------------------
    addAndMakeVisible(lowshelv);
    lowshelv.setButtonText("Low Shelv");
    lowshelv.onClick = [this]
    {
        audioProcessor.isLSActive = lowshelv.getToggleState();
    };

    fLS.reset(new FilmStripKnob(fknob, 31, false));
    addAndMakeVisible(fLS.get());
    fLS->setRange(20, 1e3, 0.1);
    fLS->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    fLS->setTextValueSuffix("Hz");
    fLS->setValue(500, juce::dontSendNotification);
    fLS->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 20);
    fLS->setSkewFactorFromMidPoint(500);
    fLS->onValueChange = [this]
    {
        audioProcessor.LS.updateCutFrequency(fLS->getValue());
    };
    
    gLS.reset(new FilmStripKnob(gKnob, 100, false));
    addAndMakeVisible(gLS.get());
    gLS->setRange(-12, 12, 0.1);
    gLS->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gLS->setValue(0, juce::dontSendNotification);
    gLS->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 20);
    gLS->onValueChange = [this]
    {
        audioProcessor.LS.updateGdB(gLS->getValue());
    };
    // -----------------------------------------------------------------------------

    /* Bell */
    // -----------------------------------------------------------------------------
    addAndMakeVisible(bell);
    bell.setButtonText("Bell");
    bell.onClick = [this]
    {
        audioProcessor.isBActive = bell.getToggleState();
    };
    fB.reset(new FilmStripKnob(fknob, 31, false));
    addAndMakeVisible(fB.get());
    fB->setRange(250, 8e3, 0.1);
    fB->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    fB->setTextValueSuffix("Hz");
    fB->setValue(1e3, juce::dontSendNotification);
    fB->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 20);
    fB->setSkewFactorFromMidPoint(4e3);
    fB->onValueChange = [this]
    {
        audioProcessor.B.updateCutFrequency(fB->getValue());
    };

    qB.reset(new FilmStripKnob(qknob, 31, false));
    addAndMakeVisible(qB.get());
    qB->setRange(1, 3, 0.1);
    qB->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    qB->setValue(1, juce::dontSendNotification);
    qB->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 20);
    qB->onValueChange = [this]
    {
        audioProcessor.B.updateQfactor(qB->getValue());
    };

    gB.reset(new FilmStripKnob(gKnob, 100, false));
    addAndMakeVisible(gB.get());
    gB->setRange(-12, 12, 0.1);
    gB->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gB->setValue(0, juce::dontSendNotification);
    gB->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 20);
    gB->onValueChange = [this]
    {
        audioProcessor.B.updateGdB(gB->getValue());
    };
    // -----------------------------------------------------------------------------

    /* High Shelv */
    // -----------------------------------------------------------------------------
    addAndMakeVisible(highshelv);
    highshelv.setButtonText("High Shelv");
    highshelv.onClick = [this]
    {
        audioProcessor.isHSActive = highshelv.getToggleState();
    };

    fHS.reset(new FilmStripKnob(fknob, 31, false));
    addAndMakeVisible(fHS.get());
    fHS->setRange(1e3, 16e3, 0.1);
    fHS->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    fHS->setTextValueSuffix("Hz");
    fHS->setValue(8e3, juce::dontSendNotification);
    fHS->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 20);
    fHS->setSkewFactorFromMidPoint(8e3);
    fHS->onValueChange = [this]
    {
        audioProcessor.HS.updateCutFrequency(fHS->getValue());
    };

    gHS.reset(new FilmStripKnob(gKnob, 100, false));
    addAndMakeVisible(gHS.get());
    gHS->setRange(-12, 12, 0.1);
    gHS->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gHS->setValue(0, juce::dontSendNotification);
    gHS->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 20);
    gHS->onValueChange = [this]
    {
        audioProcessor.HS.updateGdB(gHS->getValue());
    };
    // -----------------------------------------------------------------------------

    /* Low pass */
    // -----------------------------------------------------------------------------
    addAndMakeVisible(lowpass);
    lowpass.setButtonText("Low pass");
    lowpass.onClick = [this]
    {
        audioProcessor.isLPActive = lowpass.getToggleState();
    };

    fLP.reset(new FilmStripKnob(fknob, 31, false));
    addAndMakeVisible(fLP.get());
    fLP->setRange(1e3, 20e3, 0.1);
    fLP->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    fLP->setTextValueSuffix("Hz");
    fLP->setValue(20e3, juce::dontSendNotification);
    fLP->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 20);
    fLP->setSkewFactorFromMidPoint(4e3);
    fLP->onValueChange = [this]
    {
        audioProcessor.LP2.updateCutFrequency(fLP->getValue());
        audioProcessor.LP.updateCutFrequency(fLP->getValue());
    };

    qLP.reset(new FilmStripKnob(qknob, 31, false));
    addAndMakeVisible(qLP.get());
    qLP->setRange(1, 3, 0.1);
    qLP->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    qLP->setValue(1, juce::dontSendNotification);
    qLP->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 20);
    //qLP->setSkewFactorFromMidPoint(1);
    qLP->onValueChange = [this]
    {
        audioProcessor.LP.updateQfactor(qLP->getValue());
        audioProcessor.LP2.updateQfactor(qLP->getValue());
    };

    addAndMakeVisible(fourGradeLP);
    fourGradeLP.setButtonText("24 dB/Oct");
    fourGradeLP.setColour(juce::ToggleButton::textColourId,juce::Colour(black));
    fourGradeLP.onClick = [this]
    {
        audioProcessor.isOnDobleLP = fourGradeLP.getToggleState();
    };
    // -----------------------------------------------------------------------------

    // -----------------------------------------------------------------------------
    addAndMakeVisible(harmonicExciter);
    harmonicExciter.setButtonText("Harmonic Exciter");
    harmonicExciter.setColour(juce::ToggleButton::textColourId, juce::Colour(black));
    harmonicExciter.onClick = [this]
    {
        audioProcessor.isHEActive = harmonicExciter.getToggleState();
    };

    //harmonicDrive.reset(new FilmStripKnob(gainKnob, 31, false));
    //addAndMakeVisible(harmonicDrive.get());
    //harmonicDrive->setRange(0, 2, 0.01);
    //harmonicDrive->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    //harmonicDrive->setValue(1, juce::dontSendNotification);
    //harmonicDrive->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 20);
    //harmonicDrive->setSkewFactorFromMidPoint(1.5);
    //harmonicDrive->onValueChange = [this]
    //{
    //    audioProcessor.d = harmonicDrive->getValue();
    //    //DBG(audioProcessor.d);
    //};

    eHPfc.reset(new FilmStripKnob(fknob, 31, false));
    addAndMakeVisible(eHPfc.get());
    eHPfc->setRange(250, 16e3, 0.1);
    eHPfc->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    eHPfc->setTextValueSuffix("Hz");
    eHPfc->setValue(300, juce::dontSendNotification);
    eHPfc->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 20);
    eHPfc->setSkewFactorFromMidPoint(1e3);
    eHPfc->onValueChange = [this]
    {
        audioProcessor.harmonicHP.updateCutFrequency(eHPfc->getValue());
    };


    harmonicSaturation.reset(new FilmStripKnob(gainKnob, 31, false));
    addAndMakeVisible(harmonicSaturation.get());
    harmonicSaturation->setRange(0, 10, 0.1);
    harmonicSaturation->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    harmonicSaturation->setValue(1, juce::dontSendNotification);
    harmonicSaturation->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 20);
    harmonicSaturation->setSkewFactorFromMidPoint(1);
    harmonicSaturation->onValueChange = [this]
    {
        audioProcessor.sat = harmonicSaturation->getValue();
    };

    harmonicBlend.reset(new FilmStripKnob(dryWet, 31, false));
    addAndMakeVisible(harmonicBlend.get());
    harmonicBlend->setRange(0, 1, 0.01);
    harmonicBlend->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    harmonicBlend->setValue(0, juce::dontSendNotification);
    harmonicBlend->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 20);
    harmonicBlend->setSkewFactorFromMidPoint(0.75);
    harmonicBlend->onValueChange = [this]
    {
        audioProcessor.h = harmonicBlend->getValue();
    };

    setSize (630, 400);

    /* ---------------------------- EQ ---------------------------------------------*/
    // -----------------------------------------------------------------------------
    /* High pass*/
    // -----------------------------------------------------------------------------
    qHP->setBounds(50, 90, 80, 80);
    fHP->setBounds(50, 180, 80, 80);
    // -----------------------------------------------------------------------------

    /* Low Shelv */
    // -----------------------------------------------------------------------------
    fLS->setBounds(140, 180, 80, 80);
    gLS->setBounds(140, 90, 80, 80);
    // -----------------------------------------------------------------------------

    /* Bell*/
    // -----------------------------------------------------------------------------
    qB->setBounds(230, 90, 80, 80);
    gB->setBounds(320, 90, 80, 80);
    fB->setBounds(275, 180, 80, 80);
    // -----------------------------------------------------------------------------

    /* High Shelv*/
    // -----------------------------------------------------------------------------
    gHS->setBounds(410, 90, 80, 80);
    fHS->setBounds(410, 180, 80, 80);
    // -----------------------------------------------------------------------------

    /* Low pass*/
    // -----------------------------------------------------------------------------
    qLP->setBounds(500, 90, 80, 80);
    fLP->setBounds(500, 180, 80, 80);
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    //harmonicDrive->setBounds(140, 285, 80, 80);
    eHPfc->setBounds(230-45, 285, 80, 80);
    harmonicSaturation->setBounds(320-45, 285, 80, 80);
    harmonicBlend->setBounds(410-45, 285, 80, 80);
}

MaxiHighTerAudioProcessorEditor::~MaxiHighTerAudioProcessorEditor()
{
}

//==============================================================================
void MaxiHighTerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    juce::Image background = juce::ImageFileFormat::loadFrom(BinaryData::feel_jpg, BinaryData::feel_jpgSize);
    juce::Rectangle<float>  interfaz(0, 0, getWidth(), getHeight());

    juce::Image texttitle = juce::ImageFileFormat::loadFrom(BinaryData::ENHAIGHTER_png, BinaryData::ENHAIGHTER_pngSize);
    juce::Rectangle<float> title(157, 10, 315, 50);

    
    g.drawImage(background, interfaz, juce::RectanglePlacement::doNotResize);
    g.drawImage(texttitle, title, juce::RectanglePlacement::centred);
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Made by Jose", getLocalBounds(), juce::Justification::bottomLeft, 1);
}

void MaxiHighTerAudioProcessorEditor::resized()
{

    bypass.setBounds(10, 10, 80, 30);
    invertir.setBounds(90, 10, 120, 30);
    /* ---------------------------- EQ ---------------------------------------------*/
    // -----------------------------------------------------------------------------
    /* High pass*/
    // -----------------------------------------------------------------------------
    highpass.setBounds(50, 70, 80, 30);
    fourGradeHP.setBounds(50, 260, 80, 30);
    // -----------------------------------------------------------------------------
    
    /* Low Shelv*/
    // -----------------------------------------------------------------------------
    lowshelv.setBounds(140, 70, 80, 30);
    // -----------------------------------------------------------------------------

    /* Bell */
    // -----------------------------------------------------------------------------
    bell.setBounds(285, 70, 80, 30);
    // -----------------------------------------------------------------------------

    /* High Shelv*/
    // -----------------------------------------------------------------------------
    highshelv.setBounds(410, 70, 80, 30);
    // -----------------------------------------------------------------------------

    /* Low pass */
    // -----------------------------------------------------------------------------
    lowpass.setBounds(500, 70, 80, 30);
    fourGradeLP.setBounds(500, 260, 80, 30);
    // -----------------------------------------------------------------------------


    harmonicExciter.setBounds(250, 260, 120, 40);
    EQ.setBounds(560, 10, 50, 30);
}
