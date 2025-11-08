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
    void playSampleSpatial(const void* data, size_t size, float azimuth, float gain);

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

    double sampleRate;
    int toneChannel = 0;
    int noiseChannel = 1;

    bool playing = false;
    bool noisePlaying = false;
    bool tonePlaying = false;
    bool soundFilePlaying = false;

    int remainingSamples = 0;
    int samplesToPlay = 0;

    struct SoundSource {
        std::unique_ptr<SoundFilePlayer> player;
        std::unique_ptr<Spatialiser> spatialiser;
        float azimuth = 0.0f;
        float gain = 1.0f;
        bool active = false;
    };

    std::vector<SoundSource> sources;

    void addSoundSource(const void* data, size_t size, float azimuth, float gain);

};

