/*
  ==============================================================================

    TestController.cpp
    Created: 30 Oct 2025 2:34:24am
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#include "TestController.h"

TestController::TestController() {

    for (auto tone : testTones) {
        toneThresholds[tone] = dbLevelMax;
    }
}


