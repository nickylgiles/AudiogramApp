/*
  ==============================================================================

    SoundEngine.cpp
    Created: 28 Oct 2025 11:07:04pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#include "SoundEngine.h"

SoundEngine::SoundEngine() {
    hrtfManager.setSampleRate(sampleRate);
    hrtfManager.loadBinaryData();
}

void SoundEngine::playTone(float frequency, float amplitude, float duration, int channel) {
    sources.push_back(std::make_unique<ToneSource>(sampleRate, frequency, amplitude, duration, channel));
}

void SoundEngine::playToneMasked(float frequency, float amplitude, float duration, int channel) {
    sources.push_back(std::make_unique<ToneSource>(sampleRate, frequency, amplitude, duration, channel));
    int noiseChannel = (channel == 0 ? 1 : 0);
    sources.push_back(std::make_unique<NoiseSource>(sampleRate, frequency, amplitude, duration, noiseChannel));
}

void SoundEngine::playSampleSpatial(const void* data, size_t size, float elevation, float azimuth, float gain) {
    sources.push_back(std::make_unique<SpatialisedSoundFileSource>(
        sampleRate, data, size, hrtfManager, elevation, azimuth, gain));

}

void SoundEngine::playNoiseSpatial(float amplitude, float duration, float elevation, float azimuth) {
    sources.push_back(std::make_unique<SpatialisedNoiseSource>(
        sampleRate, amplitude, duration, hrtfManager, elevation, azimuth
    ));

}

void SoundEngine::stop() {
    sources.clear();
}

bool SoundEngine::isPlaying() const {
    return sources.size() > 0;
}

void SoundEngine::setSampleRate(double newSampleRate) {
    sampleRate = newSampleRate;
    hrtfManager.setSampleRate(newSampleRate);
    stop();
}


void SoundEngine::processBlock(float* outputL, float* outputR, int numSamples) {
 
    for (int i = 0; i < numSamples; ++i) {
        outputL[i] = 0.0f;
        outputR[i] = 0.0f;
    }
    
    juce::AudioBuffer<float> leftBuf(1, numSamples);
    juce::AudioBuffer<float> rightBuf(1, numSamples);

    float* tempL = leftBuf.getWritePointer(0);
    float* tempR = rightBuf.getWritePointer(0);

    for (auto it = sources.begin(); it != sources.end(); ) {
        (*it)->process(tempL, tempR, numSamples);

        for (int i = 0; i < numSamples; ++i) {
            outputL[i] += tempL[i];
            outputR[i] += tempR[i];
        }

        if ((*it)->isFinished()) {
            it = sources.erase(it);
            continue;
        }

        ++it;
    }


}

