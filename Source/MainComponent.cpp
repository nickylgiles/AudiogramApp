#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{

    soundEngine = std::make_unique<SoundEngine>();

    testController = std::make_unique<TestController>(*this, *soundEngine);

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
void MainComponent::testEnd() {
    testStarted = false;
    showPureToneResultsScreen();
}
void MainComponent::showMenuScreen() {
    currentScreen.reset(new MenuScreen());
    addAndMakeVisible(currentScreen.get());

    auto screen = dynamic_cast<MenuScreen*>(currentScreen.get());
    screen->onPureToneClicked = [this] {showPureToneTestScreen();};
    screen->onSpatialClicked = [this] {showSpatialTestScreen();};

    resized();
}

void MainComponent::showPureToneTestScreen() {
    testStarted = true;
    testController->startTest();
    currentScreen.reset(new PureToneTestScreen());
    addAndMakeVisible(currentScreen.get());

    auto screen = dynamic_cast<PureToneTestScreen*>(currentScreen.get());
    screen->onHearClicked = [this] {
        testController->buttonPress();
        };
    screen->onStopClicked = [this] {
        testStarted = false;
        testController->cancelTest();
        showMenuScreen();
        };

    resized();
}

void MainComponent::showSpatialTestScreen()
{
}

void MainComponent::showPureToneResultsScreen() {
    currentScreen.reset(new PureToneResultsScreen());
    addAndMakeVisible(currentScreen.get());

    PureToneTestResults results = testController->getResults();
    auto screen = dynamic_cast<PureToneResultsScreen*>(currentScreen.get());

    screen->setResults(results);
    screen->onExportClicked = [this] {
        // code to export results
        };
    screen->onMenuClicked = [this] {
        showMenuScreen();
        };

    resized();
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    if (currentScreen)
        currentScreen->setBounds(getLocalBounds());
}
