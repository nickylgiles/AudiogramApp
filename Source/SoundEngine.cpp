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
    noiseGenerator.setSampleRate(sampleRate);
    soundFilePlayer.setSampleRate(sampleRate);
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

    samplesToPlay = duration * sampleRate;
    remainingSamples = samplesToPlay;

    envelope.start(samplesToPlay);
}

void SoundEngine::playToneMasked(float frequency, float amplitude, float duration, int channel) {
    playTone(frequency, amplitude, duration, channel);
    noiseChannel = (channel == 0) ? 1 : 0;
    noisePlaying = true;
    noiseGenerator.setAmplitude(amplitude);
    noiseGenerator.setFrequency(frequency, 0.5f);
}

void SoundEngine::playSample(const void* data, size_t size) {
    bool fileLoaded = soundFilePlayer.loadBinaryData(data, size);
    if (fileLoaded) {
        soundFilePlayer.startPlaying();
        soundFilePlaying = true;
        samplesToPlay = soundFilePlayer.getLength();
        remainingSamples = samplesToPlay;
        playing = true;
    }
    else {
        soundFilePlaying = false;
        return;
    }
    sampleAzimuth = 0.0f;
}

void SoundEngine::playSampleSpatial(const void* data, size_t size, float azimuth) {
    playSample(data, size);
    sampleAzimuth = azimuth;
}

void SoundEngine::stop()
{
    playing = false;
    noisePlaying = false;
    tonePlaying = false;
    soundFilePlaying = false;
}

bool SoundEngine::isPlaying() const
{
    return playing;
}

void SoundEngine::setSampleRate(double newSampleRate) {
    sampleRate = newSampleRate;
    toneGenerator.setSampleRate(newSampleRate);
    noiseGenerator.setSampleRate(newSampleRate);

    envelope.setFallTime(newSampleRate * 0.1);
    envelope.setRiseTime(newSampleRate * 0.1);
}

std::array<float, 2> SoundEngine::nextSample() {

    std::array<float, 2> sample = {0.0f, 0.0f};

    if (!playing) {
        return sample;
    }

    float envelopeAmplitude = envelope.nextSample();

    if(tonePlaying)
        sample[toneChannel] = toneGenerator.nextSample() * envelopeAmplitude;

    if(noisePlaying)
        sample[noiseChannel] = noiseGenerator.nextSample() * envelopeAmplitude;

    if (soundFilePlaying) {
        sample[0] = soundFilePlayer.nextSample();
        sample[1] = sample[0];
    }

    remainingSamples--;
    if (remainingSamples <= 0) {
        stop();
    }

    return sample;
}

void SoundEngine::processBlock(float* outputL, float* outputR, int numSamples) {
    if (!playing) {
        return;
    }

    if (tonePlaying) {
        float* toneBuffer = (toneChannel == 0 ? outputL : outputR);
        float* noiseBuffer = (toneChannel == 0 ? outputR : outputL);

        for (int i = 0; i < numSamples; ++i) {
            float env = envelope.nextSample();
            toneBuffer[i] = toneGenerator.nextSample() * env;
            noiseBuffer[i] = noiseGenerator.nextSample() * env;
            remainingSamples--;
            if (remainingSamples <= 0) {
                stop();
            }
        }
    }

    if (soundFilePlaying) {
        for (int i = 0; i < numSamples; ++i) {
            outputL[i] = soundFilePlayer.nextSample();
            remainingSamples--;
            if (remainingSamples <= 0) {
                stop();
            }
        }
        spatialiser.processBlock(outputL, outputL, outputR, numSamples);
    }

}

