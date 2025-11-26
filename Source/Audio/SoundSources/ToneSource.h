/*
  ==============================================================================

    ToneSource.h
    Created: 26 Nov 2025 4:02:29pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include "SoundSource.h"
#include "../Utilities/ToneGenerator.h"
#include "../Utilities/Envelope.h"

class ToneSource : public SoundSource {
public:
    ToneSource(double sampleRate, float frequency, float gain, float durationSeconds, int channel);
    void process(float* outputL, float* outputR, int numSamples) override;
    bool isFinished() const override;
private:
    ToneGenerator toneGenerator;
    Envelope envelope;

    int channel;
    int samplesRemaining;
};