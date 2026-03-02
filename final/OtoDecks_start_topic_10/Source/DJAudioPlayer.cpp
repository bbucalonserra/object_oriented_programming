/*
==============================================================================

DJAudioPlayer.cpp
Created: 13 Mar 2020 4:22:22pm
Author:  matthew

==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager) 
: formatManager(_formatManager)
{

}
DJAudioPlayer::~DJAudioPlayer()
{

}

void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate) 
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);

}
void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

/*
@brief Load an audio resource from a URL (e.g. file:///C:/Musics/track01.mp3) and starts reproducing.

* This method is responsible for creating an audio format reader for the provided resource,
* that manages a dynamic memory allocation via pointers (std::unique_ptr),
* and performing a transition of the audio stream to the transport source.

@param audioURL The URL object pointing to the local or remote audio file.

* @note If the file format is unsupported or the resource is inaccessible,
* the function fails silently through a null pointer check, 
* ensuring application stability.
*/
void DJAudioPlayer::loadURL(URL audioURL)
{
    // createReaderFor: reserve bytes in the Heap. Returns the position of the first byte where is allocated in the memory (uses "new"), therefore creates in heap.
    // createInputStream: If local file, opens in HD/SSD and prepare the OS to start reading. The bool is to show or not the useProgressDialog.
    // Store the RAM address in reader.
    // The method returns a object juce::AudioFormatReader*, isntead of writting it, Wrote auto* + name. 
    // auto* reader has 8 bytes because it's only the address.
    // Variable reader is in stack, the object file is in heap.
    // NOTE: it's not the entire file, in the heap there's just a small space to read the file little by little

    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));

    // If the variable reader points to somewhere, therefore there's a file.
    if (reader != nullptr) {       
        // Create a variable called newSource.
        // unique_ptr creates a smart pointer (creates the address and when it's not used anymore, deletes it). 
        // It will store a AudioFormatReaderSource.
        // new AudioFormatReaderSource reserves a space in the heap, received the address from reader.
        // This variable is created to play, stop, pause, etc.
        std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, true)); 
        
        // transportSource is the objected created. The setSource will set from where the
        // file comes from. Gets the newSource, starts from 0, don't use thread manager (nullptr).
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);    
        
        // Reset.
        readerSource.reset (newSource.release());          
    } else {
        juce::AlertWindow::showMessageBoxAsync (
            juce::AlertWindow::WarningIcon,
            "Format Error",
            "The selected file is not supported.",
            "Ok"
        );
    }
}

void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else {
        transportSource.setGain(gain);
    }
   
}
void DJAudioPlayer::setSpeed(double ratio)
{
  if (ratio < 0 || ratio > 100.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
     if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}


void DJAudioPlayer::start()
{
    transportSource.start();
}
void DJAudioPlayer::stop()
{
  transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}