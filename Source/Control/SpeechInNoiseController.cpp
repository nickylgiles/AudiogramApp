/*
  ==============================================================================

    SpeechInNoiseController.cpp
    Created: 29 Nov 2025 1:23:35am
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#include "SpeechInNoiseController.h"

SpeechInNoiseController::SpeechInNoiseController(MainComponent& mainComponentRef, SoundEngine& soundEngineRef) 
    : TestController(mainComponentRef, soundEngineRef)
{
    fm = std::make_unique<SpeechFileManager>();
}

void SpeechInNoiseController::startTest() {
    playDigit(currDigit);
    startTimer(1000);
}

void SpeechInNoiseController::stopTest() {
    soundEngine.stop();
}

void SpeechInNoiseController::buttonClicked(const juce::String& id) {
    if (id == "stopButton") {
        stopTest();
        return;
    }
}

void SpeechInNoiseController::playDigit(int digit) {
    if (digit < 0 || digit > 9) {
        DBG("Digit out of range.");
        return;
    }
    
    const auto& res = fm->getDigit(digit);
    soundEngine.playSample(res.data, res.dataSize, 1.0f);
    
}

void SpeechInNoiseController::timerCallback() {
    playDigit(++currDigit);
}
