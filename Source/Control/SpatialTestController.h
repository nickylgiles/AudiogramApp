/*
  ==============================================================================

    SpatialTestController.h
    Created: 7 Nov 2025 4:34:29pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include "TestController.h"

class SpatialTestController : public TestController, private juce::Timer {
public:
	SpatialTestController(MainComponent& mainComponentRef, SoundEngine& soundEngineRef);
	void startTest() override;
	void stopTest() override;

	// void buttonClicked(const juce::String& id) override;

	// const SpatialTestResults getResults();
private:
	void timerCallback() override;
};