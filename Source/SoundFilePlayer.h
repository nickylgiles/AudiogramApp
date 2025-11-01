/*
  ==============================================================================

    SoundFilePlayer.h
    Created: 1 Nov 2025 9:05:31am
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SoundFilePlayer {
public:
    SoundFilePlayer();
    bool loadFile(const juce::File& file);
    bool loadBinaryData(const void* data, size_t size);
    void setSampleRate(double newSampleRate);
    void reset();
    float nextSample();
    bool fileIsLoaded() const;
    bool soundIsPlaying() const;
    void startPlaying();
    int getLength();

private:
    juce::AudioBuffer<float> buffer;
    juce::AudioFormatManager audioFormatManager;
    int currentSample;
    int totalSamples;
    double sampleRate;
    bool fileLoaded;
    bool soundPlaying;
};