#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget, 
                   public Timer
{
public:
    /** Constructor: sets up all the buttons, sliders, and labels, and starts the UI timer. 
     * @param player The audio player that this GUI will control.
     * @param formatManagerToUse The manager that handles different audio file formats.
     * @param cacheToUse The cache used to store and display audio thumbnails.
     */
    DeckGUI(DJAudioPlayer* player, 
           AudioFormatManager &    formatManagerToUse,
           AudioThumbnailCache &   cacheToUse );

    /** Destructor: stops the timer and tidies up when the deck is closed. */
    ~DeckGUI();

    /** Paints the background and standard visuals of the component. 
     * @param g The graphics context used for drawing.
     */
    void paint (Graphics& g) override;

    /** Handles the responsive layout and positioning of all UI elements. */
    void resized() override;

    /** Loads audio file directly from deck or playlist. 
     * @param audioURL The URL of the audio file to be loaded.
     */
    void loadFile(juce::URL audioURL);

    /** Sets the main theme colour for the deck's waveform. 
     * @param c The colour to be applied to the waveform display.
     */
    void setMainColour(juce::Colour c);

private:
    /** Logic for when any button is clicked, like playing music or managing cues. 
     * @param button A pointer to the button that was clicked.
     */
    void buttonClicked (Button * button) override;

    /** Updates the audio player and UI colours when sliders are moved. 
     * @param slider A pointer to the slider that has changed value.
     */
    void sliderValueChanged (Slider *slider) override;

    /** Tells the system we are ready to receive files dragged onto the deck. 
     * @param files A list of files being dragged over the component.
     * @return True if we are interested in the files, false otherwise.
     */
    bool isInterestedInFileDrag (const StringArray &files) override;

    /** Logic for loading a file when it's dropped onto the component. 
     * @param files The list of files that were dropped.
     * @param x The x-coordinate of the drop location.
     * @param y The y-coordinate of the drop location.
     */
    void filesDropped (const StringArray &files, int x, int y) override; 

    /** Regular callback to update the waveform playhead position. */
    void timerCallback() override; 

    /** Saves current hot cue positions to a local file. */
    void saveCues();

    /** Loads saved hot cue positions for a specific track. 
     * @param trackURL The URL of the track to load cues for.
     */
    void loadCues(juce::URL trackURL);

    /** Saves current EQ settings to a local file. */
    void saveEQ();
    
    juce::TextButton resetEQButton{"RESET EQ"};

    /** Loads saved EQ settings for a specific track. 
     * @param trackURL The URL of the track to load EQ settings for.
     */
    void loadEQ(juce::URL trackURL);

    // UI Components
    juce::FileChooser fChooser{"Select a file..."};

    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
    TextButton loadButton{"LOAD"};

    juce::TextButton cueButtons[8];
    juce::TextButton clearCuesButton{"Clear"};
    double cuePositions[8];

    juce::URL currentURL;
  
    Slider volSlider; 
    Slider speedSlider;
    Slider posSlider;

    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player;

    juce::Slider eqLowSlider;
    juce::Slider eqMidSlider;
    juce::Slider eqHighSlider;

    juce::Label volLabel{ "VOL", "VOL" };
    juce::Label speedLabel{ "SPD", "SPD" };
    juce::Label posLabel{ "POS", "POS" };
    juce::Label eqLabel{ "EQUALIZER", "EQUALIZER" };
    juce::Label lowLabel{ "LOW", "Low" }, midLabel{ "MID", "Mid" }, highLabel{ "HIGH", "High" };
    juce::Label bpmLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI);
};