/*
  ==============================================================================

    SpatialTestResults.h
    Created: 9 Nov 2025 6:10:43pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct SpeechTestResponse {
    juce::String targetWord;
    juce::String reportedWord;
    bool wordCorrect;
};

struct SpatialTestResponse {
    float referenceAzimuth;
    float targetAzimuth;
    bool spatialCorrect;
};

struct DualTaskResponse {
    SpeechTestResponse wordTestResponse;
    SpatialTestResponse spatialTestResponse;
};

struct SpatialSpeechTestResults {
    std::vector<DualTaskResponse> responses;
};

struct SpatialTestResults {
    std::vector<SpatialTestResponse> responses;
};

struct SpeechTestResults {
    std::vector<SpeechTestResponse> responses;
};