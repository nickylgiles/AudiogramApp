/*
  ==============================================================================

    ToneGenerator.h
    Created: 29 Oct 2025 3:46:17pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ToneGenerator {
public:
    ToneGenerator();
    void setFrequency(float newFrequency);
    void setAmplitude(float newAmplitude);
    float nextSample();
    void setSampleRate(double newSampleRate);
    void reset();
private:
    float frequency;
    float amplitude;
    double currentPhase;
    double sampleRate;
};