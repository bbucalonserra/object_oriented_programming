#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

// Constructor: sets up all the buttons, sliders, and labels, and starts the UI timer
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager &    formatManagerToUse,
                AudioThumbnailCache &    cacheToUse
           ) : player(_player), 
                waveformDisplay(formatManagerToUse, cacheToUse)
{
    // Adding the main playback and file buttons
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
       
    // Making the main deck sliders visible
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(waveformDisplay);

    // Inside DeckGUI Constructor
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posLabel);

    // Force text to the extreme left
    volLabel.setJustificationType(juce::Justification::centredLeft);
    speedLabel.setJustificationType(juce::Justification::centredLeft);
    posLabel.setJustificationType(juce::Justification::centredLeft);

    // Remove default border/internal gap
    volLabel.setBorderSize(juce::BorderSize<int>(5));
    speedLabel.setBorderSize(juce::BorderSize<int>(5));
    posLabel.setBorderSize(juce::BorderSize<int>(5));

    // Text from equalizer.
    addAndMakeVisible(eqLabel);
    eqLabel.setJustificationType(juce::Justification::centred);
    eqLabel.setFont(juce::Font(juce::FontOptions(12.0f).withStyle("Bold")));

    // Configuring the Reset EQ button
    addAndMakeVisible(resetEQButton);
    resetEQButton.addListener(this);

    // Clear Button.
    addAndMakeVisible(clearCuesButton);
    clearCuesButton.addListener(this);

    // EQ.
    addAndMakeVisible(lowLabel);
    addAndMakeVisible(midLabel);
    addAndMakeVisible(highLabel);

    lowLabel.setJustificationType(juce::Justification::centred);
    midLabel.setJustificationType(juce::Justification::centred);
    highLabel.setJustificationType(juce::Justification::centred);

    // Prepare 8 buttons Hot Cue
    for (int i = 0; i < 8; ++i) {
        cueButtons[i].setButtonText(juce::String(i + 1)); 
        addAndMakeVisible(cueButtons[i]);
        cueButtons[i].addListener(this);
        
        // Define initial time as -1.0 (empty button).
        cuePositions[i] = -1.0; 
    }

    // Setting up the listeners for all our buttons and sliders
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 2.0);
    posSlider.setRange(0.0, 1.0);


    /* Set:
        - start volume in 0.50 (preventing user to not know if the program is properly working).
        - Speed in 1.0, in order to start in regular speed.
        - Start in 0 seconds.
    */
    volSlider.setValue(0.5);
    speedSlider.setValue(1.0);
    posSlider.setValue(0.0);

    // Starting the timer so the UI keeps updating regularly
    startTimer(50);

    // Configurating sliders from EQ.
    eqLowSlider.setSliderStyle(juce::Slider::Rotary);
    eqLowSlider.setRange(0.1, 3.0);
    eqLowSlider.setValue(1.0);
    eqLowSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(eqLowSlider);
    eqLowSlider.addListener(this);

    eqMidSlider.setSliderStyle(juce::Slider::Rotary);
    eqMidSlider.setRange(0.1, 3.0);
    eqMidSlider.setValue(1.0);
    eqMidSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(eqMidSlider);
    eqMidSlider.addListener(this);

    eqHighSlider.setSliderStyle(juce::Slider::Rotary);
    eqHighSlider.setRange(0.1, 3.0);
    eqHighSlider.setValue(1.0);
    eqHighSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(eqHighSlider);
    eqHighSlider.addListener(this);

    // Label BPM
    addAndMakeVisible(bpmLabel);
    bpmLabel.setText("BPM: --", juce::dontSendNotification);
    bpmLabel.setJustificationType(juce::Justification::centredLeft);
    bpmLabel.setColour(juce::Label::textColourId, juce::Colours::orange);

    // Adjust speedSlider.
    speedSlider.setRange(0.0, 2.0); 
    speedSlider.setValue(1.0);


}

// The destructor: stops the timer and tidies up when the deck is closed
DeckGUI::~DeckGUI()
{
    stopTimer();
}

// Paints the background and standard visuals of the component
void DeckGUI::paint (Graphics& g)
{
   g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);

    g.setColour (Colours::white);
    g.setFont (14.0f);
}

// Handles the responsive layout and positioning of all UI elements
void DeckGUI::resized()
{
    // Increased divisor to 15.0 to provide more vertical granularity
    double rowH = getHeight() / 15.0; 
    int middleX = getWidth() / 2;
    int padding = 4;

    // Waveform
    waveformDisplay.setBounds(0, 0, getWidth(), (int)(rowH * 3.5));

    int startY = (int)(rowH * 3.5);
    int centralAreaH = (int)(rowH * 3.0); 

    // Left Side: Play and Stop
    int bigButtonH = centralAreaH / 2;
    playButton.setBounds(juce::Rectangle<int>(0, startY, middleX, bigButtonH).reduced(padding));
    stopButton.setBounds(juce::Rectangle<int>(0, startY + bigButtonH, middleX, bigButtonH).reduced(padding));

    // Right Side: Hot Cues
    int cueW = middleX / 3;
    int cueH = centralAreaH / 3;

    for (int i = 0; i < 3; ++i) 
        cueButtons[i].setBounds(juce::Rectangle<int>(middleX + (i * cueW), startY, cueW, cueH).reduced(padding));
    
    for (int i = 0; i < 3; ++i) 
        cueButtons[i + 3].setBounds(juce::Rectangle<int>(middleX + (i * cueW), startY + cueH, cueW, cueH).reduced(padding));
    
    cueButtons[6].setBounds(juce::Rectangle<int>(middleX, startY + (cueH * 2), cueW, cueH).reduced(padding));
    cueButtons[7].setBounds(juce::Rectangle<int>(middleX + cueW, startY + (cueH * 2), cueW, cueH).reduced(padding));
    clearCuesButton.setBounds(juce::Rectangle<int>(middleX + (cueW * 2), startY + (cueH * 2), cueW, cueH).reduced(padding));

    // Main Sliders (VOL, SPD, POS)
    int sliderY = startY + centralAreaH;
    int sliderH = (int)rowH;
    int labelW = 40; 
    int sliderW = getWidth() - labelW;

    volLabel.setBounds(0, sliderY, labelW, sliderH);
    volSlider.setBounds(labelW, sliderY, sliderW, sliderH);

    speedLabel.setBounds(0, sliderY + sliderH, labelW, sliderH);
    speedSlider.setBounds(labelW, sliderY + sliderH, sliderW, sliderH);

    posLabel.setBounds(0, sliderY + (sliderH * 2), labelW, sliderH);
    posSlider.setBounds(labelW, sliderY + (sliderH * 2), sliderW, sliderH);

    // Equalizer Section
    int eqLabelY = sliderY + (sliderH * 3);
    int eqLabelH = (int)(rowH * 0.6); 
    
    // EQ Title
    eqLabel.setBounds(0, eqLabelY, getWidth(), eqLabelH);

    // EQ Knobs (The 3 sliders)
    int eqKnobsY = eqLabelY + eqLabelH;
    // Maintained the increased knob size (2.2) for better visibility
    int eqKnobsHeight = (int)(rowH * 2.2); 
    int knobWidth = getWidth() / 3;

    eqLowSlider.setBounds(juce::Rectangle<int>(0, eqKnobsY, knobWidth, eqKnobsHeight).reduced(padding));
    eqMidSlider.setBounds(juce::Rectangle<int>(knobWidth, eqKnobsY, knobWidth, eqKnobsHeight).reduced(padding));
    eqHighSlider.setBounds(juce::Rectangle<int>(knobWidth * 2, eqKnobsY, knobWidth, eqKnobsHeight).reduced(padding));

    // EQ Labels (Low, Mid, High) placed directly below the knobs
    int eqTextY = eqKnobsY + eqKnobsHeight - 5; 
    // Height maintained to ensure "g" in "High" is not cut off
    int eqTextH = (int)(rowH * 0.6); 

    lowLabel.setBounds(0, eqTextY, knobWidth, eqTextH);
    midLabel.setBounds(knobWidth, eqTextY, knobWidth, eqTextH);
    highLabel.setBounds(knobWidth * 2, eqTextY, knobWidth, eqTextH);

    // Reset EQ Button - repositioned with safety margin to avoid overlapping LOAD button
    int resetBtnY = eqTextY + eqTextH + 5; 
    int resetBtnHeight = (int)(rowH * 0.8);
    resetEQButton.setBounds(juce::Rectangle<int>(getWidth() / 4, resetBtnY, getWidth() / 2, resetBtnHeight).reduced(padding));

    // Load Button - Anchored to the very bottom of the component
    int loadBtnHeight = (int)(rowH * 1.2);
    loadButton.setBounds(juce::Rectangle<int>(0, getHeight() - loadBtnHeight, getWidth(), loadBtnHeight).reduced(padding));

    // BPM visualization at the top right
    bpmLabel.setBounds(getWidth() - 100, 5, 90, 20);
}

// Logic for when any button is clicked, like playing music or managing cues
void DeckGUI::buttonClicked(Button* button)
{
    // Detecting which button was clicked and starting the action
    if (button == &playButton)
    {
        player->start();
    }
    if (button == &stopButton)
    {
        player->stop();
    }
    if (button == &loadButton)
    {
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
        {
            juce::File chosenFile = chooser.getResult();
            if (chosenFile.exists()){
                juce::URL fileURL = juce::URL{chosenFile};
                loadFile(fileURL); 
            }
        });
    }

    if (button == &resetEQButton)
        {
            // Reset the three sliders back to 1.0 (flat) and notify the listener
            eqLowSlider.setValue(1.0, juce::sendNotification);
            eqMidSlider.setValue(1.0, juce::sendNotification);
            eqHighSlider.setValue(1.0, juce::sendNotification);
        }

    for (int i = 0; i < 8; ++i)
    {
        if (button == &cueButtons[i])
        {
            // Set/Overwrite Cue point (Shift or Empty)
            if (cuePositions[i] == -1.0 || juce::ModifierKeys::getCurrentModifiers().isShiftDown())
            {
                cuePositions[i] = player->getPositionRelative();
                
                // When clicked.
                cueButtons[i].setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
                cueButtons[i].setColour(juce::TextButton::textColourOffId, juce::Colours::black);

                saveCues();
            }
            else
            {
                // Jump to Cue point
                player->setPositionRelative(cuePositions[i]);
            }
        }
    }

    if (button == &clearCuesButton)
    {
        // Cleaning all the hot cues we've set
        for (int i = 0; i < 8; ++i)
        {
            cuePositions[i] = -1.0;
            
            // Reset to default look
            cueButtons[i].removeColour(juce::TextButton::buttonColourId);
            cueButtons[i].removeColour(juce::TextButton::textColourOffId);
        }
        // Save Cues.
        saveCues();
    }
}

// Updates the audio player and UI colours when sliders are moved
void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
       double val = slider->getValue();
        player->setGain(val);

        // Changing the slider colour based on the volume level
        if (val > 0.8) {
            slider->setColour(juce::Slider::thumbColourId, juce::Colours::red);
            slider->setColour(juce::Slider::trackColourId, juce::Colours::red.withAlpha(0.5f));
        } else if (val > 0.5) {
            slider->setColour(juce::Slider::thumbColourId, juce::Colours::orange);
            slider->setColour(juce::Slider::trackColourId, juce::Colours::orange.withAlpha(0.5f));
        } else {
            slider->setColour(juce::Slider::thumbColourId, juce::Colours::green);
            slider->setColour(juce::Slider::trackColourId, juce::Colours::green.withAlpha(0.5f));
        }
    }

    if (slider == &speedSlider)
    {
        double speedVal = slider->getValue();
        player->setSpeed(speedVal);

        // Colours
        if (speedVal > 1.0) slider->setColour(juce::Slider::thumbColourId, juce::Colours::red);
        else if (speedVal < 1.0) slider->setColour(juce::Slider::thumbColourId, juce::Colours::orange);
        else slider->setColour(juce::Slider::thumbColourId, juce::Colours::green);

        // R5: Live BPM calculation
        double baseBpm = player->getBpm();
        double liveBpm = baseBpm * speedVal; 
        
        bpmLabel.setText("BPM: " + juce::String(liveBpm, 1), juce::dontSendNotification);
    }
    
    if (slider == &posSlider)
    {
        // Jumping to the specific track position
        player->setPositionRelative(slider->getValue());
    }

    if (slider == &eqLowSlider) {
        player->setEqLow((float)slider->getValue());
        saveEQ();
    }
    if (slider == &eqMidSlider) {
        player->setEqMid((float)slider->getValue());
        saveEQ();
    }
    if (slider == &eqHighSlider) {
        player->setEqHigh((float)slider->getValue());
        saveEQ();
    }
    
}

// Tells the system we are ready to receive files dragged onto the deck
bool DeckGUI::isInterestedInFileDrag (const StringArray& /*files*/)
{
  // We're always interested in new tracks!
  return true; 
}

// Logic for loading a file when it's dropped onto the component
void DeckGUI::filesDropped (const StringArray& files, int /*x*/, int /*y*/)
{
  // If a single file is dropped, load it into the player
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

// Regular callback to update the waveform playhead position
void DeckGUI::timerCallback()
{
    // Moving the waveform playhead based on the audio position
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());
}

// Coordinates loading a file into the player, waveform, cues, and EQ
void DeckGUI::loadFile(juce::URL audioURL)
{
    // Sends the file to the audio engine.
    player->loadURL(audioURL);
    
    // Send the file to draw the wave chart in the screen.
    waveformDisplay.loadURL(audioURL);

    loadCues(audioURL);

    loadEQ(audioURL);
}
    
// Saves current hot cue positions to a local file
void DeckGUI::saveCues()
{
    if (currentURL.isEmpty()) return;

    // Writing the cue points to a local file for the next time we load the track
    juce::String fileName = currentURL.getFileName() + ".cues";
    juce::File cueFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile(fileName);

    juce::FileOutputStream output(cueFile);
    if (!output.openedOk()) return;
    
    output.setPosition(0);
    output.truncate();

    for (int i = 0; i < 8; ++i) {
        output.writeText(juce::String(cuePositions[i]) + "\n", false, false, nullptr);
    }
}

// Loads saved hot cue positions for a specific track
void DeckGUI::loadCues(juce::URL trackURL)
{
    currentURL = trackURL;

    // Resetting the UI for a fresh start with this track
    for (int i = 0; i < 8; ++i) {
        cuePositions[i] = -1.0;
        cueButtons[i].setButtonText(juce::String(i + 1));
    }

    juce::String fileName = currentURL.getFileName() + ".cues";
    juce::File cueFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile(fileName);

    if (cueFile.existsAsFile()) {
        juce::StringArray lines;
        cueFile.readLines(lines);

        for (int i = 0; i < 8 && i < lines.size(); ++i) {
            double pos = lines[i].getDoubleValue();
            cuePositions[i] = pos;
            
            if (pos != -1.0) {
                cueButtons[i].setButtonText(juce::String(i + 1)); // Without the "ON"
                cueButtons[i].setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
                cueButtons[i].setColour(juce::TextButton::textColourOffId, juce::Colours::black);
            }
        }
    }
}

// Saves current EQ settings to a local file
void DeckGUI::saveEQ()
{
    if (currentURL.isEmpty()) return;

    // Saving the EQ settings so the track sounds the same next time
    juce::String fileName = currentURL.getFileName() + ".eq";
    juce::File eqFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile(fileName);

    juce::FileOutputStream output(eqFile);
    if (!output.openedOk()) return;
    
    output.setPosition(0);
    output.truncate();

    // Save Low, Mid, High
    output.writeText(juce::String(eqLowSlider.getValue()) + "\n", false, false, nullptr);
    output.writeText(juce::String(eqMidSlider.getValue()) + "\n", false, false, nullptr);
    output.writeText(juce::String(eqHighSlider.getValue()) + "\n", false, false, nullptr);
}

// Loads saved EQ settings for a specific track
void DeckGUI::loadEQ(juce::URL trackURL)
{
    currentURL = trackURL;

    // Searching for saved EQ settings in the user's documents
    juce::String fileName = currentURL.getFileName() + ".eq";
    juce::File eqFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile(fileName);

    if (eqFile.existsAsFile()) {
        juce::StringArray lines;
        eqFile.readLines(lines);
        
        if (lines.size() >= 3) {
            eqLowSlider.setValue(lines[0].getFloatValue(), juce::sendNotification);
            eqMidSlider.setValue(lines[1].getFloatValue(), juce::sendNotification);
            eqHighSlider.setValue(lines[2].getFloatValue(), juce::sendNotification);
            return;
        }
    }
    
    // Setting defaults if no settings are found
    eqLowSlider.setValue(1.0, juce::sendNotification);
    eqMidSlider.setValue(1.0, juce::sendNotification);
    eqHighSlider.setValue(1.0, juce::sendNotification);
}

// Customises the waveform colour for this deck
void DeckGUI::setMainColour(juce::Colour c) 
{ 
  waveformDisplay.setWaveformColour(c); 
}