/*
  ==============================================================================

    SoundEngine.h
    Created: 28 Oct 2025 11:07:04pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include "ToneGenerator.h"
#include "NoiseGenerator.h"
#include "Envelope.h"
#include "SoundFilePlayer.h"
#include "Spatialiser.h"


class SoundEngine {
public:
    SoundEngine();
    void playTone(float frequency, float amplitude, float duration, int channel);
    void playToneMasked(float frequency, float amplitude, float duration, int channel);

    void playSample(const void* data, size_t size);
    void playSampleSpatial(const void* data, size_t size, float azimuth);

    void stop();
    bool isPlaying() const;
    void setSampleRate(double newSampleRate);

    std::array<float, 2> nextSample();
    void processBlock(float* outputL, float* outputR, int numSamples);

private:
    ToneGenerator toneGenerator;
    NoiseGenerator noiseGenerator;
    Envelope envelope;
    SoundFilePlayer soundFilePlayer;
    Spatialiser spatialiser;

    double sampleRate = 44100.0;
    int toneChannel = 0;
    int noiseChannel = 1;

    bool playing = false;
    bool noisePlaying = false;
    bool tonePlaying = false;
    bool soundFilePlaying = false;

    int remainingSamples = 0;
    int samplesToPlay = 0;
};

