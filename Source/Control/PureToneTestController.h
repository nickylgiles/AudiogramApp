/*
  ==============================================================================

    PureToneTestController.h
    Created: 7 Nov 2025 4:33:08pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/
#include "TestController.h"
#pragma once

class PureToneTestController : public TestController, private juce::Timer {
public:
    PureToneTestController(MainComponent& mainComponentRef, SoundEngine& soundEngineRef);
    void startTest() override;
    void stopTest() override;

    void buttonPress();

    const PureToneTestResults getResults();

private:
    void timerCallback() override;

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