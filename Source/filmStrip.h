/*
  ==============================================================================

    filmStrip.h
    Created: 27 Dec 2016 10:27:25pm
    Author:  Mateo Yepes

  ==============================================================================
*/

#ifndef FILMSTRIP_H_INCLUDED
#define FILMSTRIP_H_INCLUDED
#include <JuceHeader.h>
class FilmStripKnob : public juce::Slider
{
public:
	FilmStripKnob(juce::Image image, const int numFrames, const bool stripIsHorizontal) // Class Constructor
		: Slider(), // Initialize inherited Slider
		filmStrip(image), // Initialize Image
		numFrames_(numFrames), // Initialize number of frames for image
		isHorizontal_(stripIsHorizontal) // Initialize orientation from Knob or image
{
    setTextBoxStyle(NoTextBox, 0, 0, 0); // Slider TextBox
    if (isHorizontal_) // Calculate steps for Knob FilmStrip
    {
        frameHeight = filmStrip.getHeight();
        frameWidth = filmStrip.getWidth() / numFrames_;
    }
    else 
    { // Same for Vertical FilmStrip
        frameHeight = filmStrip.getHeight() / numFrames_;
        frameWidth = filmStrip.getWidth();
    }
}

void paint(juce::Graphics& g) // Override the paint method from the Slider Class
{   
    // Value is the actual position of the "fader" or knob
    int value = (getValue() - getMinimum()) / (getMaximum() - getMinimum()) * (numFrames_ - 1);
	
    // Draw the frame corresponding to the position given by value
    
    if (isHorizontal_) // Horizontal
    {
        g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(),
        value * frameWidth, 0, frameWidth, frameHeight);
    }
    else// Vertical
    {
        g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(),
        0, value * frameHeight, frameWidth, frameHeight);
    }
}

int getFrameWidth() const  { return frameWidth; }

int getFrameHeight() const { return frameHeight; }

private:

	juce::Image filmStrip; // Create Image Object (with Filmstrip)
	const int numFrames_; // This is for the number of frames given by the 
	const bool isHorizontal_;
	int frameWidth, frameHeight; // Size of individual frames of the filmstrip
};



#endif  // FILMSTRIP_H_INCLUDED
