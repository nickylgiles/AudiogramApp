/*
  ==============================================================================

    SpatialTestController.cpp
    Created: 7 Nov 2025 4:34:29pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#include "SpatialTestController.h"
#include <BinaryData.h>

SpatialTestController::SpatialTestController(MainComponent& mainComponentRef, SoundEngine& soundEngineRef)
    	: TestController(mainComponentRef, soundEngineRef) {
    
}

void SpatialTestController::startTest() {
    soundEngine.playSampleSpatial(BinaryData::snd_wav, BinaryData::snd_wavSize, -85.0f, -90.0f, 1.0f);
}

void SpatialTestController::stopTest() {
    soundEngine.stop();
}
void SpatialTestController::timerCallback() {
    return;
}