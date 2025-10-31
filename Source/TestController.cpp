/*
  ==============================================================================

    TestController.cpp
    Created: 30 Oct 2025 2:34:24am
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#include "TestController.h"
#include "MainComponent.h"

TestController::TestController(MainComponent* mainComponentPtr, SoundEngine* soundEnginePtr) {

    mainComponent = mainComponentPtr;
    soundEngine = soundEnginePtr;

    for (auto tone : testTones) {
        toneThresholds[0][tone] = dbLevelMax;
        toneThresholds[1][tone] = dbLevelMax;
    }
}

void TestController::startTest() {
    currentTone = 0;
    currentEar = 0;
    currentThreshold = dbLevelMin;
    thresholdIncreasing = true;

    currentToneDetected = false;

    playFirstTone();
   
}

void TestController::buttonPress() {
    currentToneDetected = true;
}

void TestController::cancelTest() {
    stopTimer();
    soundEngine->stop();
}

/*static*/ float TestController::dbToAmplitude(float db) {
    float amplitude = std::pow(10.0, db / 20.0);
    return amplitude;
}

void TestController::playNextTone() {
    if (currentToneDetected) {
        toneThresholds[currentEar][testTones[currentTone]] = currentThreshold;
        currentThreshold = dbLevelMin;
        currentTone++;
    }
    else {
        if (floatsEqual(currentThreshold, dbLevelMax) || currentThreshold > dbLevelMax) {
            currentThreshold = dbLevelMin;
            currentTone++;
        }
        else {
            currentThreshold += dbIncrementAscending;
        }
    }

    if (currentTone < testTones.size()) {
        soundEngine->playToneMasked(testTones[currentTone], dbToAmplitude(currentThreshold), 1.0f, currentEar);

        currentToneDetected = false;

        scheduleNextTone(2000);
    }
    else {
        if (currentEar < 1) {
            // Test other ear
            currentEar = 1;
            currentThreshold = dbLevelMin;
            thresholdIncreasing = true;
            currentTone = 0;
            playFirstTone();
        }
        else {
            // Test finished
            DBG("Test finished.");
            mainComponent->testEnd();
        }
    }
}

void TestController::playFirstTone() {

    soundEngine->playToneMasked(testTones[currentTone], dbToAmplitude(currentThreshold), 1.0f, currentEar);

    currentToneDetected = false;

    scheduleNextTone(2000);
}

/*static*/ constexpr bool TestController::floatsEqual(float a, float b) {
    return (a - b < 0.01f && a - b > -0.01f);
}

std::array<std::map<float, float>, 2> const TestController::getResults() {
    return toneThresholds;
}

void TestController::scheduleNextTone(int delayMs) {
    stopTimer();
    startTimer(delayMs);
}

void TestController::timerCallback() {
    stopTimer();
    playNextTone();
}