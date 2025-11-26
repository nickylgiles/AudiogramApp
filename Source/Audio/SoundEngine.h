/*
  ==============================================================================

    SoundEngine.h
    Created: 28 Oct 2025 11:07:04pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include "Utilities/ToneGenerator.h"
#include "Utilities/NoiseGenerator.h"
#include "Utilities/Envelope.h"
#include "Utilities/SoundFilePlayer.h"
#include "Spatial/Spatialiser.h"
#include "Spatial/HRTFManager.h"
#include "SoundSources/SoundSource.h"
#include "SoundSources/ToneSource.h"
#include "SoundSources/NoiseSource.h"
#include "SoundSources/SpatialisedSoundFileSource.h"
#include "SoundSources/SpatialisedNoiseSource.h"


class SoundEngine {
public:
    SoundEngine();
    void playTone(float frequency, float amplitude, float duration, int channel);
    void playToneMasked(float frequency, float amplitude, float duration, int channel);

    void playSampleSpatial(const void* data, size_t size, float elevation, float azimuth, float gain);
    void playNoiseSpatial(float amplitude, float duration, float elevation, float azimuth);

    void stop();
    bool isPlaying() const;
    void setSampleRate(double newSampleRate);

    void processBlock(float* outputL, float* outputR, int numSamples);

private:
    double sampleRate;

    std::vector<std::unique_ptr<SoundSource>> sources;

    HRTFManager hrtfManager;
};

