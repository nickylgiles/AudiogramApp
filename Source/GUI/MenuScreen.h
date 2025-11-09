/*
  ==============================================================================

    MenuScreen.h
    Created: 4 Nov 2025 7:25:01pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MenuScreen : public juce::Component {
public:
    MenuScreen();
    ~MenuScreen() override = default;


    // Callbacks set in MainComponent
    std::function<void()> onPureToneClicked;
    std::function<void()> onSpatialClicked;

    void resized() override;
    void paint(juce::Graphics& g) override;

private:
    juce::TextButton pureToneButton{ "Pure Tone Test" };
    juce::TextButton spatialButton{ "Spatialization Test" };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuScreen)
};
