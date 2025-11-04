#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{

    soundEngine = std::make_unique<SoundEngine>();

    testController = std::make_unique<TestController>(this, soundEngine.get());
    /*
    addAndMakeVisible(startButton);

    startButton.onClick = [this] {
        
        if (!testStarted) {
            testStarted = true;
            startButton.setButtonText("Stop Test");
            testController->startTest();
            soundEngine->playSampleSpatial(BinaryData::snd_wav, BinaryData::snd_wavSize, -90.0f);
        }
        else {
            testStarted = false;
            startButton.setButtonText("Restart Test");
            testController->cancelTest();
            soundEngine->playSampleSpatial(BinaryData::snd_wav, BinaryData::snd_wavSize, 80.0f);
        }

    };

    addAndMakeVisible(hearToneButton);

    hearToneButton.onClick = [this] {
        testController->buttonPress();
        };
    */

    showMenuScreen();
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    soundEngine->setSampleRate(sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();

    auto* leftChannel = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightChannel = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    /*
    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        auto stereoSample = soundEngine->nextSample();

        leftChannel[i] = stereoSample[0];
        rightChannel[i] = stereoSample[1];
    }
    */

    soundEngine->processBlock(leftChannel, rightChannel, bufferToFill.numSamples);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
   
}
void MainComponent::paintOverChildren(juce::Graphics& g) {
    g.setColour(juce::Colours::black);
    int y = 20;
    int lineHeight = 20;
    juce::String ears[2] = { "Left", "Right" };

    auto results = testController->getResults();
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
void MainComponent::testEnd() {
    testStarted = false;
    startButton.setButtonText("Restart Test");
}
void MainComponent::showMenuScreen() {
    currentScreen.reset(new MenuScreen());
    addAndMakeVisible(currentScreen.get());

    auto menu = dynamic_cast<MenuScreen*>(currentScreen.get());
    menu->onPureToneClicked = [this] {showPureToneTestScreen();};
    menu->onSpatialClicked = [this] {showSpatialTestScreen();};

    resized();
}

void MainComponent::showPureToneTestScreen()
{
}

void MainComponent::showSpatialTestScreen()
{
}

void MainComponent::showPureToneResultsScreen()
{
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    /*
    auto area = getLocalBounds().reduced(40);
    auto buttonHeight = area.getHeight() / 2;

    startButton.setBounds(area.removeFromTop(buttonHeight).reduced(10));
    hearToneButton.setBounds(area.reduced(10));
    */
    if (currentScreen)
        currentScreen->setBounds(getLocalBounds());
}
