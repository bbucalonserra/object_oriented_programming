#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(AudioFormatManager &   formatManagerToUse,
                                 AudioThumbnailCache &  cacheToUse) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse), 
                                 fileLoaded(false), 
                                 position(0)
                          
{
  // Registering the display to listen for changes in the audio thumbnail
  audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (waveformColour);
    
    if(fileLoaded)
    {
      // Drawing the actual waveform using the thumbnail data
      audioThumb.drawChannel(g, 
        getLocalBounds(), 
        0, 
        audioThumb.getTotalLength(), 
        0, 
        1.0f
      );

      // Drawing the playhead indicator over the waveform
      g.setColour(Colours::lightgreen);
      g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
    }
    else 
    {
      g.setFont (20.0f);
      g.drawText ("File not loaded...", getLocalBounds(),
                  Justification::centred, true);   // draw some placeholder text

    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(URL audioURL)
{
  // Clearing the previous thumbnail and loading the new audio source
  audioThumb.clear();
  fileLoaded  = audioThumb.setSource(new URLInputSource(audioURL));
  
  if (fileLoaded)
  {
    repaint();
  }
  else {
    std::cout << "Not loaded." << std::endl;
  }

}

void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    // Redraw the component whenever the thumbnailer updates
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
  // Only repaint if the position has actually changed to save resources
  if (pos != position)
  {
    position = pos;
    repaint();
  }
}

void WaveformDisplay::setWaveformColour(juce::Colour newColour)
{
    // Allowing the deck to customise its waveform colour
    waveformColour = newColour;
    repaint();
}