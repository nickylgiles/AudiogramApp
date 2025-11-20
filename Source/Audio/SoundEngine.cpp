/*
  ==============================================================================

    SoundEngine.cpp
    Created: 28 Oct 2025 11:07:04pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#include "SoundEngine.h"

SoundEngine::SoundEngine() 
    : spatialiser(hrtfManager) 
{
    toneGenerator.setSampleRate(sampleRate);
    noiseGenerator.setSampleRate(sampleRate);
    soundFilePlayer.setSampleRate(sampleRate);
    toneChannel = 0;
    noiseChannel = 0;

    hrtfManager.setSampleRate(sampleRate);
    hrtfManager.loadBinaryData();
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
    spatialiser.setAzimuth(0.0f);
}

void SoundEngine::playSampleSpatial(const void* data, size_t size, float azimuth, float gain) {
    //playSample(data, size);
    //spatialiser.setAzimuth(azimuth);
    SoundSource source;
    source.player = std::make_unique<SoundFilePlayer>();
    source.player->loadBinaryData(data, size);
    source.player->setSampleRate(sampleRate);
    source.player->startPlaying();

    source.spatialiser = std::make_unique<Spatialiser>(hrtfManager);
    source.spatialiser->setSampleRate(sampleRate);
    source.spatialiser->setAzimuth(azimuth);

    source.azimuth = azimuth;
    source.gain = gain;
    source.active = true;

    sources.push_back(std::move(source));
    playing = true;
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
    soundFilePlayer.setSampleRate(newSampleRate);
    hrtfManager.setSampleRate(newSampleRate);

    envelope.setFallTime(newSampleRate * 0.1);
    envelope.setRiseTime(newSampleRate * 0.1);

    spatialiser.setSampleRate(newSampleRate);
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
        juce::AudioBuffer<float> soundBuffer(1, numSamples);
        float* soundBufferWritePointer = soundBuffer.getWritePointer(0);
        for (int i = 0; i < numSamples; ++i) {
            soundBufferWritePointer[i] = soundFilePlayer.nextSample();
            remainingSamples--;
            if (remainingSamples <= 0) {
                stop();
            }
        }
        spatialiser.processBlock(soundBuffer.getReadPointer(0), outputL, outputR, numSamples);
    }

    if (!sources.empty()) {
        juce::AudioBuffer<float> monoBuffer(1, numSamples);
        juce::AudioBuffer<float> stereoBuffer(2, numSamples);

        for (auto it = sources.begin(); it != sources.end(); ) {
            if (it->active && it->player) {
                float* monoWritePointer = monoBuffer.getWritePointer(0);
                float* stereoL = stereoBuffer.getWritePointer(0);
                float* stereoR = stereoBuffer.getWritePointer(1);

                for (int i = 0; i < numSamples; ++i)
                    monoWritePointer[i] = it->player->nextSample();

                it->spatialiser->processBlock(monoWritePointer, stereoL, stereoR, numSamples);

                for (int i = 0; i < numSamples; ++i) {
                    outputL[i] += stereoL[i] * it->gain;
                    outputR[i] += stereoR[i] * it->gain;
                }

                // Remove finished sources
                if (it->player->isFinished()) {
                    it = sources.erase(it);
                    continue;
                }
            }
            ++it;
        }
    }

}

