#pragma once

#include <JuceHeader.h>
#include "TestController.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    void paintOverChildren(juce::Graphics& g) override;

private:
    //==============================================================================
    // Your private member variables go here...
    juce::TextButton startButton{ "Start Test" };
    juce::TextButton hearToneButton{ "Tone Heard" };

    std::unique_ptr<TestController> testController;
    std::unique_ptr<SoundEngine> soundEngine;

    bool testStarted = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
