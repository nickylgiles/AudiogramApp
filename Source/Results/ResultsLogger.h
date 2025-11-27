/*
  ==============================================================================

    ResultsLogger.h
    Created: 27 Nov 2025 4:10:24am
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include "Database.h"
#include "../SpatialTestResults.h"
#include "../PureToneTestResults.h"

class ResultsLogger {
public:
    ResultsLogger();
    ~ResultsLogger();

    bool openDatabase(const juce::File& file);

    bool logSpatialResults(const SpatialTestResults& results);
    bool logPureToneResults(const PureToneTestResults& results);

private:
    Database db;
};