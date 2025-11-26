/*
  ==============================================================================

    SpatialisedNoiseSource.h
    Created: 26 Nov 2025 4:03:16pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include "SoundSource.h"
#include "NoiseSource.h"
#include "../Spatial/Spatialiser.h"
#include "../Spatial/HRTFManager.h"

class SpatialisedNoiseSource : public SoundSource {
public:
    SpatialisedNoiseSource(double sampleRate, float gain, float durationSeconds, HRTFManager& hrtfManager, float elevation, float azimuth);
    void process(float* outputL, float* outputR, int numSamples) override;
    bool isFinished() const override;
private:
    NoiseGenerator noiseGenerator;
    Envelope envelope;
    Spatialiser spatialiser;
    juce::AudioBuffer<float> tempBuffer;
    int samplesRemaining;
};