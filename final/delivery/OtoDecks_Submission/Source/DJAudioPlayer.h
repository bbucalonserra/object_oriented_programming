#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <juce_dsp/juce_dsp.h>

class DJAudioPlayer : public AudioSource {
  public:

    /** Constructor: initializes the player and sets up the audio format manager.
     * @param _formatManager The manager that helps the player recognize different audio files.
     */
    DJAudioPlayer(AudioFormatManager& _formatManager);

    /** Destructor: cleans up the resources when the player is no longer needed. */
    ~DJAudioPlayer();

    /** Prepares the audio source for playback.
     * @param samplesPerBlockExpected The number of samples the player should process at a time.
     * @param sampleRate The sample rate of the audio output device.
     */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;

    /** Fills the audio buffer with the next block of data to be played.
     * @param bufferToFill Information about the buffer that needs audio data.
     */
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

    /** Releases audio resources when playback is finished or stopped. */
    void releaseResources() override;

    /** Loads a specific audio file from a URL.
     * @param audioURL The location of the audio file to load.
     */
    void loadURL(URL audioURL);

    /** Adjusts the volume gain of the player.
     * @param gain The volume level (usually between 0.0 and 1.0).
     */
    void setGain(double gain);

    /** Sets the playback speed ratio.
     * @param ratio The speed multiplier (e.g., 1.0 is normal, 2.0 is double speed).
     */
    void setSpeed(double ratio);

    /** Moves the playhead to a specific time in seconds.
     * @param posInSecs The desired position in the track.
     */
    void setPosition(double posInSecs);

    /** Moves the playhead to a relative position (0.0 to 1.0).
     * @param pos The percentage of the track to jump to.
     */
    void setPositionRelative(double pos);

    /** Adjusts the low-frequency gain of the equaliser.
     * @param gainLinear The gain level for the low band.
     */
    void setEqLow(float gainLinear);

    /** Adjusts the mid-frequency gain of the equaliser.
     * @param gainLinear The gain level for the mid band.
     */
    void setEqMid(float gainLinear);

    /** Adjusts the high-frequency gain of the equaliser.
     * @param gainLinear The gain level for the high band.
     */
    void setEqHigh(float gainLinear);

    /** Starts the audio playback. */
    void start();

    /** Stops the audio playback. */
    void stop();

    /** get the relative position of the playhead */
    double getPositionRelative();

    /** Calculates the current BPM based on the track and playback speed.
     * @return The calculated beats per minute.
     */
    double getBpm();

private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource; 
    ResamplingAudioSource resampleSource{&transportSource, false, 2};
    double currentSampleRate = 44100.0;

    juce::dsp::ProcessorChain<juce::dsp::IIR::Filter<float>,
                                  juce::dsp::IIR::Filter<float>,
                                  juce::dsp::IIR::Filter<float>> eqChain;

    double currentBaseBpm = 120.0;                          
};