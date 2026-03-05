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
    DeckGUI(DJAudioPlayer* player, 
           AudioFormatManager &    formatManagerToUse,
           AudioThumbnailCache &   cacheToUse );
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;

    /** Loads audio file directly from deck or playlist. */
    void loadFile(juce::URL audioURL);

    /** Sets the main theme colour for the deck's waveform. */
    void setMainColour(juce::Colour c);

private:
    /** implement Button::Listener. */
    void buttonClicked (Button *) override;

    /** implement Slider::Listener. */
    void sliderValueChanged (Slider *slider) override;

    /** Detects if a file is being dragged over the deck. */
    bool isInterestedInFileDrag (const StringArray &files) override;

    /** Handles the file dropped onto the deck. */
    void filesDropped (const StringArray &files, int x, int y) override; 

    /** Updates the waveform position based on the audio playhead. */
    void timerCallback() override; 

    // Internal persistence methods
    void saveCues();
    void loadCues(juce::URL trackURL);
    void saveEQ();
    
    juce::TextButton resetEQButton{"RESET EQ"};

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