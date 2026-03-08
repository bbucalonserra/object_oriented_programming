#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


class WaveformDisplay    : public Component, 
                           public ChangeListener
{
public:
    /** Constructor: sets up the audio thumbnailer and registers the change listener.
     * @param formatManagerToUse The manager that handles audio format reading.
     * @param cacheToUse The cache used to store and quickly retrieve thumbnails.
     */
    WaveformDisplay( AudioFormatManager &   formatManagerToUse,
                    AudioThumbnailCache &   cacheToUse );

    /** Destructor: cleans up the thumbnail resources when the display is destroyed. */
    ~WaveformDisplay();

    /** Paints the visual representation of the audio waveform.
     * @param g The graphics context used for drawing the waveform and playhead.
     */
    void paint (Graphics& g) override;

    /** Handles the responsive layout of the waveform component. */
    void resized() override;

    /** Receives updates from the thumbnailer to redraw when the audio finishes loading.
     * @param source The broadcaster that sent the change notification.
     */
    void changeListenerCallback (ChangeBroadcaster *source) override;

    /** Loads a specific audio file to be drawn as a waveform.
     * @param audioURL The URL of the audio file to process.
     */
    void loadURL(URL audioURL);

    /** set the relative position of the playhead*/
    void setPositionRelative(double pos);

    /** Changes the colour of the waveform lines for visual customisation.
     * @param newColour The new colour to be applied to the waveform.
     */
    void setWaveformColour(juce::Colour newColour);

private:
    // The core object that handles the audio visual data
    AudioThumbnail audioThumb;
    
    // Flag to check if we actually have a file to draw
    bool fileLoaded; 
    
    // Current position of the playhead (0.0 to 1.0)
    double position;

    // The primary colour used to draw the waveform lines
    juce::Colour waveformColour = juce::Colours::orange;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};