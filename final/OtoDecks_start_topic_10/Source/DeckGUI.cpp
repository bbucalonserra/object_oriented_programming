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

    // Clear Buttom
    addAndMakeVisible(clearCuesButton);
    clearCuesButton.addListener(this);

    // Prepare 8 buttons Hot Cue
    for (int i = 0; i < 8; ++i) {
        cueButtons[i].setButtonText("Cue " + juce::String(i + 1)); 
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


}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("DeckGUI", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 8; 
    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);  
    volSlider.setBounds(0, rowH * 2, getWidth(), rowH);
    speedSlider.setBounds(0, rowH * 3, getWidth(), rowH);
    posSlider.setBounds(0, rowH * 4, getWidth(), rowH);
    waveformDisplay.setBounds(0, rowH * 5, getWidth(), rowH * 2);
    loadButton.setBounds(0, rowH * 7, getWidth(), rowH);


    // Draw line from hot cues.
    int cueY = getHeight() - 30; 
    int cueWidth = getWidth() / 9; 

    for (int i = 0; i < 8; ++i) {
        cueButtons[i].setBounds(i * cueWidth, cueY, cueWidth, 30);
    }
    clearCuesButton.setBounds(8 * cueWidth, cueY, cueWidth, 30);

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
                    player->loadURL(fileURL);
                    waveformDisplay.loadURL(fileURL);
                    
                    loadCues(fileURL); 
                }
            });
        }

    for (int i = 0; i < 8; ++i)
    {
        if (button == &cueButtons[i])
        {
            if (cuePositions[i] == -1.0 || juce::ModifierKeys::getCurrentModifiers().isShiftDown())
            {
                cuePositions[i] = player->getPositionRelative();
                cueButtons[i].setButtonText("Cue " + juce::String(i + 1) + " (ON)");

                saveCues();
            }
            else
            {
                player->setPositionRelative(cuePositions[i]);
            }
        }
    }

    for (int i = 0; i < 8; ++i)
    {
        if (button == &cueButtons[i])
        {
            if (cuePositions[i] == -1.0 || juce::ModifierKeys::getCurrentModifiers().isShiftDown())
            {
                cuePositions[i] = player->getPositionRelative();
                cueButtons[i].setButtonText("Cue " + juce::String(i + 1) + " (ON)");

                saveCues();
            }
            else
            {
                player->setPositionRelative(cuePositions[i]);
            }
        }
    }

    if (button == &clearCuesButton)
    {
        for (int i = 0; i < 8; ++i)
        {
            cuePositions[i] = -1.0;
            cueButtons[i].setButtonText("Cue " + juce::String(i + 1));
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
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
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
}
    

void DeckGUI::saveCues()
{
    // Se não houver música carregada, não guarda nada
    if (currentURL.isEmpty()) return;

    // Cria um ficheiro com o nome da música + ".cues" na pasta Documentos
    juce::String fileName = currentURL.getFileName() + ".cues";
    juce::File cueFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile(fileName);

    juce::FileOutputStream output(cueFile);
    if (!output.openedOk()) return;
    
    output.setPosition(0);
    output.truncate();

    // Escreve os 8 tempos no ficheiro, um por linha
    for (int i = 0; i < 8; ++i) {
        output.writeText(juce::String(cuePositions[i]) + "\n", false, false, nullptr);
    }
}

void DeckGUI::loadCues(juce::URL trackURL)
{
    currentURL = trackURL;

    for (int i = 0; i < 8; ++i) {
        cuePositions[i] = -1.0;
        cueButtons[i].setButtonText("Cue " + juce::String(i + 1));
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
                cueButtons[i].setButtonText("Cue " + juce::String(i + 1) + " (ON)");
            }
        }
    }
}