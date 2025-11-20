/*
  ==============================================================================

    HRTFManager.h
    Created: 19 Nov 2025 11:12:50pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class HRTFManager {
public:
    HRTFManager();
    void loadBinaryData();
    void setSampleRate(double newSampleRate);

    const juce::AudioBuffer<float>& getIR(float azimuth, int channel);
private:
    struct IRPair {
        juce::AudioBuffer<float> left;
        juce::AudioBuffer<float> right;
    };

    // Azimuth (degrees) -> IR Pair
    std::map<int, IRPair> irMap;
    
    double sampleRate;
    double irSampleRate = 48000.0;

    bool loadIR(const juce::String& name, juce::AudioBuffer<float>& dest);

    juce::AudioFormatManager audioFormatManager;

};