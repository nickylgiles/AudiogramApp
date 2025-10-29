/*
  ==============================================================================

    NoiseGenerator.h
    Created: 29 Oct 2025 3:55:39pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class NoiseGenerator {
public:
    NoiseGenerator();
    float nextSample();
    void setFrequency(float frequency, float bandwidthOctaves);
    void setAmplitude(float newAmplitude);
    void setSampleRate(double newSampleRate);

private:
    float centreFrequency;
    float amplitude;
    double sampleRate;
    juce::dsp::IIR::Filter<float> bandPassFilter;
    juce::Random random;
};