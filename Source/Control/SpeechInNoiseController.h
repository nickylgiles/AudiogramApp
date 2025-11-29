/*
  ==============================================================================

    SpeechInNoiseController.h
    Created: 29 Nov 2025 1:23:35am
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include "TestController.h"
#include "../Audio/Speech/SpeechFileManager.h"

class SpeechInNoiseController : public TestController, private juce::Timer {
public:
    SpeechInNoiseController(MainComponent& mainComponentRef, SoundEngine& soundEngineRef);
    void startTest() override;
    void stopTest() override;

    void buttonClicked(const juce::String& id) override;
private:
    void playDigit(int digit);
    
    std::unique_ptr<SpeechFileManager> fm;

    void timerCallback() override;
    int currDigit = 0;

};