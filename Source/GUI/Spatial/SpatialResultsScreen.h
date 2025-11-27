/*
  ==============================================================================

    SpatialResultsScreen.h
    Created: 26 Nov 2025 11:40:54pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../../SpatialTestResults.h" 

class SpatialResultsScreen : public juce::Component {
public:
    SpatialResultsScreen();
    ~SpatialResultsScreen() override = default;

    void setResults(const SpatialTestResults& newResults);

    // Callbacks set in MainComponent
    std::function<void()> onExportClicked;
    std::function<void()> onMenuClicked;

    void resized() override;
    void paint(juce::Graphics& g) override;

private:
    juce::TextButton exportButton{ "Export Results" };
    juce::TextButton menuButton{ "Back to menu" };

    SpatialTestResults results;
    std::vector<float> frequencies;
    std::vector<float> dbLevels;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpatialResultsScreen)
};