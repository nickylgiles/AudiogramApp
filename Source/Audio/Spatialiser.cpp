/*
  ==============================================================================

    Spatialiser.cpp
    Created: 1 Nov 2025 7:00:41pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#include "Spatialiser.h"
#include <JuceHeader.h>

Spatialiser::Spatialiser(HRTFManager& hrtfManagerRef)
    : hrtfManager(hrtfManagerRef) {
}

void Spatialiser::setSampleRate(double newSampleRate) {
    sampleRate = newSampleRate;
}

void Spatialiser::setAzimuth(float newAzimuth) {
    azimuth = newAzimuth;
}

void Spatialiser::reset() {
    return;
}

void Spatialiser::setFFTBlockSize(int newBlockSize) {
    return;
}

/*
    Placeholder method - currently pans left/ right basede on azimuth
    Todo - implement partitioned convulution with HRTFs
*/
void Spatialiser::processBlock(const float* input, float* outputL, float* outputR, int numSamples) {
    float pan = (azimuth + 90.0f) / 180.0f;
    float gainLeft = std::cos(pan * juce::MathConstants<float>::halfPi);
    float gainRight = std::sin(pan * juce::MathConstants<float>::halfPi);

    for (int i = 0; i < numSamples; ++i) {
        outputL[i] = input[i] * gainLeft;
        outputR[i] = input[i] * gainRight;
    }
}

