/*
  ==============================================================================

    PureToneResultsScreen.h
    Created: 4 Nov 2025 7:24:41pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class PureToneResultsScreen : public juce::Component {
public:
    PureToneResultsScreen();
    ~PureToneResultsScreen() override = default;


    // Callbacks set in MainComponent
    std::function<void()> onExportClicked;
    std::function<void()> onMenuClicked;

    void resized() override;
    void paint(juce::Graphics& g) override;

private:
    juce::TextButton exportButton{ "Export Results" };
    juce::TextButton menuButton{ "Back to menu" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PureToneResultsScreen)
};