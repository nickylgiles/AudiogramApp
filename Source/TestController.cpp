/*
  ==============================================================================

    TestController.cpp
    Created: 30 Oct 2025 2:34:24am
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#include "TestController.h"

TestController::TestController(SoundEngine* soundEnginePointer) {

    soundEngine = soundEnginePointer;

    for (auto tone : testTones) {
        toneThresholds[tone] = dbLevelMax;
    }
}

void TestController::startTest() {
    currentTone = 0;
    currentEar = 0;
    currentThreshold = 0.0f;
    thresholdIncreasing = true;

    currentToneDetected = false;

    playNextTone();
   
}

/*static*/ float TestController::dbToAmplitude(float db) {
    float amplitude = std::pow(10.0, db / 20.0);
    return amplitude;
}

void TestController::playNextTone() {
    if (currentTone < testTones.size()) {

        soundEngine->playToneMasked(testTones[currentTone], dbToAmplitude(currentThreshold), 1.0f, currentEar);
        currentTone++;

        juce::Timer::callAfterDelay(2000, [this] {
            playNextTone();
            });
    }
}