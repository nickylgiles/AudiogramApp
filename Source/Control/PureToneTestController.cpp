/*
  ==============================================================================

    PureToneTestController.cpp
    Created: 7 Nov 2025 4:33:08pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#include "PureToneTestController.h"
#include "../MainComponent.h"

PureToneTestController::PureToneTestController(MainComponent& mainComponentRef, SoundEngine& soundEngineRef)
    : TestController(mainComponentRef, soundEngineRef) {
    for (auto tone : testTones) {
        toneThresholds[0][tone] = dbLevelMax;
        toneThresholds[1][tone] = dbLevelMax;
    }
}

void PureToneTestController::startTest() {
    currentTone = 0;
    currentEar = 0;
    currentThreshold = dbLevelMin;
    thresholdIncreasing = true;

    currentToneDetected = false;

    playFirstTone();

}

void PureToneTestController::buttonPress() {
    currentToneDetected = true;
}

void PureToneTestController::stopTest() {
    stopTimer();
    soundEngine.stop();
}

/*static*/ float PureToneTestController::dbToAmplitude(float db) {
    float amplitude = std::pow(10.0, db / 20.0);
    return amplitude;
}

void PureToneTestController::playNextTone() {
    if (currentToneDetected) {
        toneThresholds[currentEar][testTones[currentTone]] = currentThreshold;
        if (thresholdIncreasing) {
            if (currentThreshold > dbLevelMin) {
                currentThreshold -= dbIncrementDescending;
                thresholdIncreasing = false;
            }
            else {
                currentThreshold = dbLevelMin;
                currentTone++;
                thresholdIncreasing = true;
            }
        }
        else {
            if (currentThreshold <= dbLevelMin || floatsEqual(currentThreshold, dbLevelMin)) {
                currentThreshold = dbLevelMin;
                currentTone++;
                thresholdIncreasing = true;
            }
            else {
                currentThreshold -= dbIncrementDescending;
            }
        }

    }
    else {
        if (floatsEqual(currentThreshold, dbLevelMax) || currentThreshold > dbLevelMax) {
            currentThreshold = dbLevelMin;
            currentTone++;
        }
        else {
            if (thresholdIncreasing) {
                currentThreshold += dbIncrementAscending;
            }
            else {
                currentThreshold = dbLevelMin;
                currentTone++;
                thresholdIncreasing = true;
            }
        }
    }

    if (currentTone < testTones.size()) {
        soundEngine.playToneMasked(testTones[currentTone], dbToAmplitude(currentThreshold), 1.0f, currentEar);

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
            mainComponent.testEnd();
        }
    }
}

void PureToneTestController::playFirstTone() {

    soundEngine.playToneMasked(testTones[currentTone], dbToAmplitude(currentThreshold), 1.0f, currentEar);

    currentToneDetected = false;

    scheduleNextTone(2000);
}

/*static*/ constexpr bool PureToneTestController::floatsEqual(float a, float b) {
    return (a - b < 0.01f && a - b > -0.01f);
}

PureToneTestResults const PureToneTestController::getResults() {
    return toneThresholds;
}

void PureToneTestController::scheduleNextTone(int delayMs) {
    stopTimer();
    startTimer(delayMs);
}

void PureToneTestController::timerCallback() {
    stopTimer();
    playNextTone();
}