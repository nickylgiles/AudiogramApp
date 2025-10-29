/*
  ==============================================================================

    SoundEngine.cpp
    Created: 28 Oct 2025 11:07:04pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#include "SoundEngine.h"

SoundEngine::SoundEngine() {
    toneGenerator.setSampleRate(sampleRate);
    toneChannel = 0;
    noiseChannel = 0;
}

void SoundEngine::playTone(float frequency, float amplitude, float duration, int channel) {

    tonePlaying = true;
    playing = true;

    toneGenerator.reset();

    toneGenerator.setAmplitude(amplitude);
    toneGenerator.setFrequency(frequency);
    toneChannel = channel;

    remainingSamples = duration * sampleRate;
}

void SoundEngine::playToneMasked(float frequency, float amplitude, float duration, int channel) {
    playTone(frequency, amplitude, duration, channel);
    noiseChannel = (channel == 0) ? 1 : 0;
    noisePlaying = true;
    noiseGenerator.setAmplitude(amplitude);
    noiseGenerator.setFrequency(frequency, 0.5f);
}

void SoundEngine::stop()
{
    playing = false;
    noisePlaying = false;
    tonePlaying = false;
}

bool SoundEngine::isPlaying()
{
    return playing;
}

void SoundEngine::setSampleRate(double newSampleRate) {
    sampleRate = newSampleRate;
    toneGenerator.setSampleRate(newSampleRate);
    noiseGenerator.setSampleRate(newSampleRate);
}

std::array<float, 2> SoundEngine::nextSample() {
    std::array<float, 2> sample = {0.0f, 0.0f};

    if (!playing) {
        return sample;
    }

    if(tonePlaying)
        sample[toneChannel] = toneGenerator.nextSample();
    if(noisePlaying)
        sample[noiseChannel] = noiseGenerator.nextSample();
    remainingSamples--;
    if (remainingSamples <= 0) {
        stop();
    }

    return sample;
}
