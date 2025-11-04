/*
  ==============================================================================

    PureToneResultsScreen.cpp
    Created: 4 Nov 2025 7:24:41pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#include "PureToneResultsScreen.h"

PureToneResultsScreen::PureToneResultsScreen() {
    addAndMakeVisible(exportButton);
    exportButton.onClick = [this] {if (onExportClicked) onExportClicked(); };

    addAndMakeVisible(menuButton);
    menuButton.onClick = [this] {if (onMenuClicked) onMenuClicked(); };
}

void PureToneResultsScreen::setResults(const PureToneTestResults& newResults)
{
    results = newResults;
}

void PureToneResultsScreen::resized() {
    auto area = getLocalBounds().reduced(20);
    auto buttonHeight = area.getHeight() / 2;

    exportButton.setBounds(area.removeFromTop(buttonHeight).reduced(10));
    menuButton.setBounds(area.reduced(10));
}

void PureToneResultsScreen::paint(juce::Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::black);
    int y = 20;
    int lineHeight = 20;
    juce::String ears[2] = { "Left", "Right" };

    for (int i = 0; i < 2; ++i) {
        g.drawText(ears[i], 40, y, 300, lineHeight, juce::Justification::left);
        y += lineHeight;
        for (const auto& pair : results[i]) {
            float freq = pair.first;
            float threshold = pair.second;

            juce::String text = juce::String(freq) + " Hz: " + juce::String(threshold, 1) + " dB";
            g.drawText(text, 40, y, 300, lineHeight, juce::Justification::left);
            y += lineHeight;
        }
    }
}
