/*
  ==============================================================================

    SoundFilePlayer.cpp
    Created: 1 Nov 2025 9:05:31am
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#include "SoundFilePlayer.h"

SoundFilePlayer::SoundFilePlayer() {
    currentSample = 0;
    totalSamples = 0;
    sampleRate = 44100.0;
    fileLoaded = false;
    fileSampleRate = 44100.0;
    playbackIncrement = 1.0;
    soundPlaying = false;
    audioFormatManager.registerBasicFormats();
}

bool SoundFilePlayer::loadFile(const juce::File& file) {
    std::unique_ptr<juce::AudioFormatReader> reader(audioFormatManager.createReaderFor(file));

    if (reader.get() == nullptr) {
        fileLoaded = false;
        DBG("Failed to load file");
        return false;
    }

    totalSamples = reader->lengthInSamples;
    buffer.setSize((int)reader->numChannels, totalSamples);
    reader->read(&buffer, 0, totalSamples, 0, true, true);

    currentSample = 0;
    fileLoaded = true;
    soundPlaying = false;
    DBG("File loaded.  Length = " << totalSamples << " samples");

    fileSampleRate = reader->sampleRate;
    playbackIncrement = fileSampleRate / sampleRate;

    return true;
}

bool SoundFilePlayer::loadBinaryData(const void* data, size_t size) {
    auto inputStream = std::make_unique<juce::MemoryInputStream>(data, size, false);

    std::unique_ptr<juce::AudioFormatReader> reader(
        audioFormatManager.createReaderFor(std::move(inputStream)));

    if (reader == nullptr) {
        DBG("Failed to load binary resource");
        return false;
    }
    
    totalSamples = reader->lengthInSamples;
    buffer.setSize((int)reader->numChannels, totalSamples);
    reader->read(&buffer, 0, totalSamples, 0, true, true);
    currentSample = 0.0;
    fileLoaded = true;
    soundPlaying = false;
    DBG("Binary resource loaded. Length = " << totalSamples << " samples");


    fileSampleRate = reader->sampleRate;
    playbackIncrement = fileSampleRate / sampleRate;

    return true;

}

void SoundFilePlayer::setSampleRate(double newSampleRate) {
    sampleRate = newSampleRate;
    if (fileLoaded)
        playbackIncrement = fileSampleRate / sampleRate;
}

void SoundFilePlayer::reset() {
    currentSample = 0.0;
    soundPlaying = false;
}

float SoundFilePlayer::nextSample() {
    if (!fileLoaded || totalSamples == 0 || !soundPlaying)
        return 0.0f;

    if (currentSample >= totalSamples - 1) {
        reset();
        return 0.0f;
    }

    int index = static_cast<int>(currentSample);
    float frac = static_cast<float>(currentSample - index);

    float s1 = buffer.getSample(0, index);
    float s2 = buffer.getSample(0, index + 1);
    float sample = s1 + frac * (s2 - s1);

    currentSample += playbackIncrement;

    return sample;
}

bool SoundFilePlayer::fileIsLoaded() const {
    return fileLoaded;
}

bool SoundFilePlayer::soundIsPlaying() const {
    return soundPlaying;
}

void SoundFilePlayer::startPlaying() {
    if (!fileLoaded) return;
    soundPlaying = true;
    currentSample = 0;
}

int SoundFilePlayer::getLength()
{
    if (!fileLoaded)
        return 0;
    return totalSamples;
}

bool SoundFilePlayer::isFinished() const {
    return fileLoaded && !soundPlaying;
}

int SoundFilePlayer::samplesRemaining() {
    return totalSamples - currentSample;
}

