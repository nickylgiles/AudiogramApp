/*
  ==============================================================================

    NoiseSource.h
    Created: 26 Nov 2025 4:02:53pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include "SoundSource.h"
#include "../Utilities/NoiseGenerator.h"
#include "../Utilities/Envelope.h"

class NoiseSource : public SoundSource {
public:
    NoiseSource(double sampleRate, float centreFrequency, float gain, float durationSeconds, int channel);
    void process(float* outputL, float* outputR, int numSamples) override;
    bool isFinished() const override;
private:
    NoiseGenerator noiseGenerator;
    Envelope envelope;

    int channel;
    int samplesRemaining;
};