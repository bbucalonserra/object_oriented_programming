#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

class MainComponent   : public AudioAppComponent
{
public:
    /** Constructor: sets up the audio format manager, players, and the main layout. */
    MainComponent();

    /** Destructor: shuts down the audio system and cleans up resources. */
    ~MainComponent();

    /** Prepares the audio channels and mixer for playback.
     * @param samplesPerBlockExpected The number of samples the audio device will request.
     * @param sampleRate The output sample rate of the audio device.
     */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;

    /** Combines the audio streams from both decks and sends them to the output.
     * @param bufferToFill Information about the audio buffer to be processed.
     */
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

    /** Releases audio resources when they are no longer needed. */
    void releaseResources() override;

    /** Paints any background visuals or branding for the main application window. 
     * @param g The graphics context used for drawing.
     */
    void paint (Graphics& g) override;

    /** Handles the responsive layout of the decks and the playlist. */
    void resized() override;

private:  
    // Handles the different audio file formats
    AudioFormatManager formatManager;

    // Cache to store thumbnails so they don't have to be re-drawn constantly
    AudioThumbnailCache thumbCache{100}; 

    // Player and GUI for the first deck
    DJAudioPlayer player1{formatManager};
    DeckGUI deckGUI1{&player1, formatManager, thumbCache}; 

    // Player and GUI for the second deck
    DJAudioPlayer player2{formatManager};
    DeckGUI deckGUI2{&player2, formatManager, thumbCache}; 

    // Combines multiple audio sources into one output stream
    MixerAudioSource mixerSource; 
    
    // The component that manages our track list and loading logic
    PlaylistComponent playlistComponent{&deckGUI1, &deckGUI2};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};