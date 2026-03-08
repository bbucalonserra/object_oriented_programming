#include "MainComponent.h"

MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1400, 900);

    // Colors from Decks.
    deckGUI1.setMainColour(juce::Colours::darkmagenta);
    deckGUI2.setMainColour(juce::Colours::cyan);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }  

    // Making our GUI components visible on the screen
    addAndMakeVisible(deckGUI1); 
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);

    // Getting the format manager ready to handle common audio files
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // Getting both players ready for the audio stream
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    // Setting up the mixer and adding our decks as inputs
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

 }
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Fetching the combined audio from the mixer to play it out
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // Component is opaque, so we must completely fill the background with a solid colour.
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    // Divides screen.
    auto deckHeight = getHeight() * 0.6;

    // Decks upper part.
    deckGUI1.setBounds(0, 0, getWidth()/2, deckHeight);
    deckGUI2.setBounds(getWidth()/2, 0, getWidth()/2, deckHeight);

    // Playlist below decks.
   playlistComponent.setBounds(0, deckHeight, getWidth(), getHeight() - deckHeight);
}