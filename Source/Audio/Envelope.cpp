/*
  ==============================================================================

    Envelope.cpp
    Created: 29 Oct 2025 8:41:59pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#include "Envelope.h"

void Envelope::stop() {
    currentSample = 0;
    active = false;
}

void Envelope::setFallTime(int numSamples) {
    fallTime = numSamples;
}

void Envelope::setRiseTime(int numSamples) {
    riseTime = numSamples;
}

void Envelope::start(int numSamples) {
    duration = numSamples;
    currentSample = 0;
    active = true;
}

float Envelope::nextSample() {

    float amplitude = 0.0f;
    if (!active)
        return amplitude;

    if (currentSample < riseTime) {
        amplitude = static_cast<float>(currentSample) / static_cast<float>(riseTime);
    }
    else if (currentSample < (duration - fallTime)) {
        amplitude = 1.0f;
    }
    else {
        amplitude = 1.0f - (static_cast<float>(currentSample) - (static_cast<float>(duration) - static_cast<float>(fallTime))) / static_cast<float>(fallTime);
    }
    
    ++currentSample;
    if (currentSample >= duration) {
        stop();
    }

    return amplitude;
}