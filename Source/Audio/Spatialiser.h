/*
  ==============================================================================

    Spatialiser.h
    Created: 1 Nov 2025 7:00:41pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include "HRTFManager.h"
#include <JuceHeader.h>

class Spatialiser {
public:
    Spatialiser(HRTFManager& hrtfManagerRef);
    void setSampleRate(double newSampleRate);
    void setAzimuth(float azimuthDegrees);
    void reset();

    void setFFTBlockSize(int newBlockSize);

    void processBlock(const float* input, float* outputL, float* outputR, int numSamples);
private:
    float azimuth;
    double sampleRate;
    HRTFManager& hrtfManager;

    /*
    juce::dsp::Convolution convolutionL;
    juce::dsp::Convolution convolutionR;
    */
    juce::dsp::FIR::Filter<float> firLeft;
    juce::dsp::FIR::Filter<float> firRight;

    bool irLoaded = false;
};