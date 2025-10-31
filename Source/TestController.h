/*
  ==============================================================================

    TestController.h
    Created: 30 Oct 2025 2:34:24am
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SoundEngine.h"

class TestController {
public:
    TestController(SoundEngine* soundEnginePointer);
    void startTest();
    void buttonPress();

    const std::array<std::map<float, float>, 2> const getResults();
    
private:
    SoundEngine* soundEngine;

    static float dbToAmplitude(float db);
    void playFirstTone();
    void playNextTone();

    std::vector<float> testTones = { 125.0f, 250.0f, 500.0f, 1000.0f, 2000.0f, 4000.0f, 8000.0f, 16000.0f };

    float dbLevelMin = -20.0f;
    float dbLevelMax = 0.0f;
    float dbIncrementAscending = 5.0f;
    float dbIncrementDescending = 2.5f;

    static constexpr bool floatsEqual(float a, float b);

    std::array<std::map<float, float>, 2> toneThresholds;



    int currentTone = 0;
    int currentEar = 0;
    float currentThreshold = -20.0f;
    bool thresholdIncreasing = true;

    

    bool currentToneDetected = false;

};