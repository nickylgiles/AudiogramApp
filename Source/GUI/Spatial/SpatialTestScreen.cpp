/*
  ==============================================================================

    SpatialTestScreen.cpp
    Created: 26 Nov 2025 11:40:40pm
    Author:  nicky_hgjk9m6

  ==============================================================================
*/

#include "SpatialTestScreen.h"

SpatialTestScreen::SpatialTestScreen() {
    addAndMakeVisible(leftButton);
    leftButton.onClick = [this] {if (onLeftClicked) onLeftClicked(); };

    addAndMakeVisible(rightButton);
    rightButton.onClick = [this] {if (onRightClicked) onRightClicked(); };

    addAndMakeVisible(stopButton);
    stopButton.onClick = [this] {if (onStopClicked) onStopClicked(); };
}

void SpatialTestScreen::resized() {
    auto area = getLocalBounds().reduced(40);
    auto buttonHeight = area.getHeight() / 2;

    auto topArea = area.removeFromTop(buttonHeight).reduced(10);
    leftButton.setBounds(topArea.removeFromLeft(topArea.getWidth() / 2).reduced(10));
    rightButton.setBounds(topArea.reduced(10));
    stopButton.setBounds(area.reduced(10));
}

void SpatialTestScreen::paint(juce::Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

