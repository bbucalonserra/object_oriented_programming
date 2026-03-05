/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
           ) : player(_player), 
               waveformDisplay(formatManagerToUse, cacheToUse)
{

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
       
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
    eqLabel.setFont(juce::Font(12.0f, juce::Font::bold));

    // Clear Button.
    addAndMakeVisible(clearCuesButton);
    clearCuesButton.addListener(this);

    // Prepare 8 buttons Hot Cue
    for (int i = 0; i < 8; ++i) {
        cueButtons[i].setButtonText(juce::String(i + 1)); 
        addAndMakeVisible(cueButtons[i]);
        cueButtons[i].addListener(this);
        
        // Define initial time as -1.0 (empty button).
        cuePositions[i] = -1.0; 
    }


    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 100.0);
    posSlider.setRange(0.0, 1.0);


    /* Set:
        - start volume in 0.50 (preventing user to not know if the program is properly working).
        - Speed in 1.0, in order to start in regular speed.
        - Start in 0 seconds.
    */
    volSlider.setValue(0.5);
    speedSlider.setValue(1.0);
    posSlider.setValue(0.0);

    startTimer(500);

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


}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
   g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 14.0; 
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
    int eqLabelH = (int)(rowH * 0.5); // Small height for the title
    
    // EQ Title
    eqLabel.setBounds(0, eqLabelY, getWidth(), eqLabelH);

    // EQ Knobs (The 3 sliders)
    int eqKnobsY = eqLabelY + eqLabelH;
    int eqKnobsHeight = (int)(rowH * 2.5); // Adjusted to fit the title above
    int knobWidth = getWidth() / 3;

    eqLowSlider.setBounds(juce::Rectangle<int>(0, eqKnobsY, knobWidth, eqKnobsHeight).reduced(padding));
    eqMidSlider.setBounds(juce::Rectangle<int>(knobWidth, eqKnobsY, knobWidth, eqKnobsHeight).reduced(padding));
    eqHighSlider.setBounds(juce::Rectangle<int>(knobWidth * 2, eqKnobsY, knobWidth, eqKnobsHeight).reduced(padding));

    // Load Button
    loadButton.setBounds(juce::Rectangle<int>(0, getHeight() - (int)rowH, getWidth(), (int)rowH).reduced(padding));
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
    if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
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
        for (int i = 0; i < 8; ++i)
        {
            cuePositions[i] = -1.0;
            
            // Reset to default look
            cueButtons[i].removeColour(juce::TextButton::buttonColourId);
            cueButtons[i].removeColour(juce::TextButton::textColourOffId);
            
            saveCues();
        }
    }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
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

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray& files, int /*x*/, int /*y*/)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

void DeckGUI::timerCallback()
{
    //std::cout << "DeckGUI::timerCallback" << std::endl;
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());
}

void DeckGUI::loadFile(juce::URL audioURL)
{
    // Sends the file to the audio engine.
    player->loadURL(audioURL);
    
    // Send the file to draw the wave chart in the screen.
    waveformDisplay.loadURL(audioURL);

    loadCues(audioURL);

    loadEQ(audioURL);
}
    

void DeckGUI::saveCues()
{
    if (currentURL.isEmpty()) return;

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

void DeckGUI::loadCues(juce::URL trackURL)
{
    currentURL = trackURL;

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
                cueButtons[i].setButtonText(juce::String(i + 1) + " (ON)");
            }
        }
    }
}

void DeckGUI::saveEQ()
{
    if (currentURL.isEmpty()) return;

    juce::String fileName = currentURL.getFileName() + ".eq";
    juce::File eqFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile(fileName);

    juce::FileOutputStream output(eqFile);
    if (!output.openedOk()) return;
    
    output.setPosition(0);
    output.truncate();

    // Sava Low, Mid, High
    output.writeText(juce::String(eqLowSlider.getValue()) + "\n", false, false, nullptr);
    output.writeText(juce::String(eqMidSlider.getValue()) + "\n", false, false, nullptr);
    output.writeText(juce::String(eqHighSlider.getValue()) + "\n", false, false, nullptr);
}

void DeckGUI::loadEQ(juce::URL trackURL)
{
    currentURL = trackURL;

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
    
    eqLowSlider.setValue(1.0, juce::sendNotification);
    eqMidSlider.setValue(1.0, juce::sendNotification);
    eqHighSlider.setValue(1.0, juce::sendNotification);
}

void DeckGUI::setMainColour(juce::Colour c) 
{ 
    waveformDisplay.waveformColour = c; 
    waveformDisplay.repaint();
}