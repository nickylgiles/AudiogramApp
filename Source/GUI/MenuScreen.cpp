/*
  ==============================================================================

    MenuScreen.cpp
    Created: 4 Nov 2025 7:25:01pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#include "MenuScreen.h"

MenuScreen::MenuScreen() {
    addAndMakeVisible(pureToneButton);
    pureToneButton.onClick = [this] {if (onPureToneClicked) onPureToneClicked(); };

    addAndMakeVisible(spatialButton);
    spatialButton.onClick = [this] {if (onSpatialClicked) onSpatialClicked(); };
}

void MenuScreen::resized() {
    auto area = getLocalBounds().reduced(40);
    auto buttonHeight = area.getHeight() / 2;

    pureToneButton.setBounds(area.removeFromTop(buttonHeight).reduced(10));
    spatialButton.setBounds(area.reduced(10));
}

void MenuScreen::paint(juce::Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

