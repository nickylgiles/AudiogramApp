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
    
}

void SpatialTestController::stopTest() {
    soundEngine.stop();
}
void SpatialTestController::timerCallback() {
    return;
}