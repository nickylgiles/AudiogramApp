/*
  ==============================================================================

    TestController.h
    Created: 30 Oct 2025 2:34:24am
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Audio/SoundEngine.h"

class MainComponent;

struct PureToneTestResults {
    std::map<float, float> left;
    std::map<float, float> right;

    std::map<float, float>& operator[](size_t index) {
        if (index == 0) return left;
        if (index == 1) return right;
        throw std::out_of_range("Index must be 0 or 1");
    }

    const std::map<float, float>& operator[](size_t index) const {
        if (index == 0) return left;
        if (index == 1) return right;
        throw std::out_of_range("Index must be 0 or 1");
    }
};

class TestController : private juce::Timer {
public:
    TestController(MainComponent& mainComponentPtr, SoundEngine& soundEnginePtr);
    void startTest();
    void buttonPress();
    void cancelTest();

    const PureToneTestResults getResults();
    
private:
    void timerCallback() override;

    SoundEngine& soundEngine;

    MainComponent& mainComponent;

    static float dbToAmplitude(float db);
    void playFirstTone();
    void playNextTone();

    void scheduleNextTone(int delayMs);

    std::vector<float> testTones = { 125.0f, 250.0f, 500.0f, 1000.0f, 2000.0f, 4000.0f, 8000.0f, 16000.0f };
   // std::vector<float> testTones = { 1000.0f };

    float dbLevelMin = -50.0f;
    float dbLevelMax = 0.0f;
    float dbIncrementAscending = 10.0f;
    float dbIncrementDescending = 5.0f;

    static constexpr bool floatsEqual(float a, float b);

    PureToneTestResults toneThresholds;


    int currentTone = 0;
    int currentEar = 0;
    float currentThreshold = -20.0f;
    bool thresholdIncreasing = true;

    

    bool currentToneDetected = false;

};

