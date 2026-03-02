#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include <string>

// Forward declaration: to compiler "Know" that the class exists.
class DeckGUI;

// Structure to store data from each music.
struct Track {
    juce::String title;
    juce::URL url;
    juce::String format;
    double lengthInSecs;
};

/**
     * Structure to represent a single audio track in the library.
     * It stores the file name (title) and its memory location (URL).
     * It has inheritance from juce Component, TableListBoxModel and Button::Listener.
 */
class PlaylistComponent : public juce::Component,
                          public juce::TableListBoxModel,
                          public juce::Button::Listener
{
public:
    PlaylistComponent(DeckGUI* deck1, DeckGUI* deck2);
    ~PlaylistComponent();

    /** Paints the background of the table rows. */
    void paint (juce::Graphics&) override;

     /** Draws the text/data into each cell of the library table. */
    void resized() override;

    /** Returns the total number of tracks currently in the library. */
    int getNumRows() override;

    /** Draws the background for each row in the table. */
    void paintRowBackground (juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
    
    /** Draws the specific data (text) into each cell of the table. */
    void paintCell (juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    /** Insert the interactive components (buttons) in the cells. */
    juce::Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) override;
    
    /** Handles button clicks, specifically the 'Import' action for R2A. */
    void buttonClicked(juce::Button* button) override;

    /** Stores the actual state from vector of tracks in disk. */
    void saveLibrary();

    /** Reads the disk and build the vector of tracks before initialization. */
    void loadLibrary();

private:
    juce::TableListBox tableComponent;
    juce::TextButton importButton{ "IMPORT TO LIBRARY" };
    
    // Vector that stores multiple Track objects for the library.
    std::vector<Track> tracks;

    std::unique_ptr<juce::FileChooser> fChooser;

    // Pointers to decks.
    DeckGUI* deck1;
    DeckGUI* deck2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};