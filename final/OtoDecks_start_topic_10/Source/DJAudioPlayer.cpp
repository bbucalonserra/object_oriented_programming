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
        
        // Search for BPM.
        juce::String bpmString = reader->metadataValues.getValue("bpm", "120");
        currentBaseBpm = bpmString.getDoubleValue();

        // If anything "weird", use 120.
        if (currentBaseBpm <= 0) currentBaseBpm = 120.0;

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
  if (ratio < 0 || ratio > 2.00)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 2" << std::endl;
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

void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate) 
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    // Prepare DSP.
    currentSampleRate = sampleRate;
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.numChannels = 2; // Estéreo
    
    eqChain.prepare(spec);

    // Initialize filtees (1 = flat / no changes).
    setEqLow(1.0f);
    setEqMid(1.0f);
    setEqHigh(1.0f);
}

void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);

    juce::dsp::AudioBlock<float> block(*bufferToFill.buffer);

    auto subBlock = block.getSubsetChannelBlock(bufferToFill.startSample, (size_t)bufferToFill.numSamples);
    
    juce::dsp::ProcessContextReplacing<float> context(subBlock);
    
    eqChain.process(context);
}


void DJAudioPlayer::setEqLow(float gainLinear)
{
    auto coeffs = juce::dsp::IIR::Coefficients<float>::makeLowShelf(currentSampleRate, 200.0f, 0.707f, gainLinear);
    *eqChain.get<0>().coefficients = *coeffs;
}

void DJAudioPlayer::setEqMid(float gainLinear)
{
    auto coeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter(currentSampleRate, 5000.0f, 0.707f, gainLinear);
    *eqChain.get<1>().coefficients = *coeffs;
}

void DJAudioPlayer::setEqHigh(float gainLinear)
{
    auto coeffs = juce::dsp::IIR::Coefficients<float>::makeHighShelf(currentSampleRate, 4000.0f, 0.707f, gainLinear);
    *eqChain.get<2>().coefficients = *coeffs;
}

double DJAudioPlayer::getBpm() 
{ 
    return currentBaseBpm; 
}