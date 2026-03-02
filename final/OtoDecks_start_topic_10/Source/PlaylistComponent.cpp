#include "PlaylistComponent.h"
#include "DeckGUI.h"

PlaylistComponent::PlaylistComponent(DeckGUI* _deck1, DeckGUI* _deck2): deck1(_deck1), deck2(_deck2)
{
    // Configuration of columns from table.
    tableComponent.getHeader().addColumn("Track Title", 1, 300);
    tableComponent.getHeader().addColumn("Format", 2, 100);
    tableComponent.getHeader().addColumn("Duration", 3, 100);
    tableComponent.getHeader().addColumn("Load D1", 4, 80);
    tableComponent.getHeader().addColumn("Load D2", 5, 80);
    tableComponent.getHeader().addColumn("Remove", 6, 80);

    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(importButton);

    importButton.addListener(this);

    // Read saves file when opening the app.
    loadLibrary();
}

PlaylistComponent::~PlaylistComponent() {}

void PlaylistComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void PlaylistComponent::resized()
{
    // Button on the top (10%) and table below (90%). 
    importButton.setBounds(0, 0, getWidth(), getHeight() / 10);
    tableComponent.setBounds(0, getHeight() / 10, getWidth(), (getHeight() / 10) * 9);
}

// Returns the size of the vector (how many rows the table should draw).
int PlaylistComponent::getNumRows()
{
    return static_cast<int>(tracks.size());
}

// Draw the background of the lines (azul if selected, gray if not).
void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected) g.fillAll(juce::Colours::lightblue);
    else g.fillAll(juce::Colours::darkgrey);
}

// Draw the text in each cell based on the columnId.
void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    if (rowNumber < getNumRows())
    {
        g.setColour(juce::Colours::white);
        
        if (columnId == 1) // Title.
            g.drawText(tracks[rowNumber].title, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        
        if (columnId == 2) // Format.
            g.drawText(tracks[rowNumber].format, 2, 0, width - 4, height, juce::Justification::centredLeft, true);

        if (columnId == 3) // Duration.
            g.drawText(juce::String( (int)tracks[rowNumber].lengthInSecs ) + "s", 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button == &importButton)
    {
        // Define to open files and allow multiple files.
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles | 
                                juce::FileBrowserComponent::canSelectMultipleItems;

        // Create the selector (using std::make_unique to manage memory).
        fChooser = std::make_unique<juce::FileChooser>("Select files to add to library...",
                                                       juce::File{},
                                                       "*.mp3;*.wav;*.aif");

        // Assync form (modern JUCE).                                                       
        fChooser->launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
        {
            auto results = chooser.getResults(); // Pega a lista de arquivos selecionados

            for (const auto& file : results)
            {
                Track newTrack;
                newTrack.title = file.getFileName();
                newTrack.url = juce::URL{file};
                newTrack.format = file.getFileExtension();

                juce::AudioFormatManager formatManager;
                formatManager.registerBasicFormats();
                std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
                
                if (reader != nullptr) {
                    newTrack.lengthInSecs = reader->lengthInSamples / reader->sampleRate;
                } else {
                    newTrack.lengthInSecs = 0;
                }

                tracks.push_back(newTrack);
            }

            // Tells table to update the list in the screen.
            tableComponent.updateContent();

            saveLibrary();
        });
    }
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate)
{

    if (columnId == 4 || columnId == 5 || columnId == 6)
    {
        auto* btn = static_cast<juce::TextButton*>(existingComponentToUpdate);

        if (btn == nullptr)
        {
            // Defines the name for the button (columns 6).
            juce::String btnName = (columnId == 4) ? "Deck 1" : (columnId == 5 ? "Deck 2" : "Remove");
            btn = new juce::TextButton(btnName);
        }

        // Stores the actual line in the ID of the button.
        btn->setComponentID(juce::String(rowNumber));

        // Defines the action of the click in memory.
                btn->onClick = [this, btn, columnId]()
                {
                    int row = btn->getComponentID().getIntValue();
                    
                    if (columnId == 4) {
                        // Send URL from music to deck 1.
                        deck1->loadFile(tracks[row].url);
                    } else if (columnId == 5) {
                        // Send URL from music to deck 2.
                        deck2->loadFile(tracks[row].url);

                    } else if (columnId == 6) {
                        
                        juce::MessageManager::callAsync([this, row]() 
                        {
                            if (row < tracks.size()) 
                            {
                                tracks.erase(tracks.begin() + row);
                                tableComponent.updateContent();
                                saveLibrary();
                            }
                        });
                        
                    }
                };

        return btn;
    }

    return nullptr;
}

void PlaylistComponent::saveLibrary()
{
    // Creates (or locates) a file  "OtoDecksLibrary.txt" in the documents directory from SO.
    juce::File libraryFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("OtoDecksLibrary.txt");
    
    // Opens the output flow of data.
    juce::FileOutputStream output(libraryFile);
    if (!output.openedOk()) return;
    
    output.setPosition(0);
    output.truncate();
    
    // Iterates from the vector and stores the path of each music.
    for (const auto& track : tracks)
    {
        // Collects the absolut path from windows / mac and adds a break line.
        juce::String filePath = track.url.getLocalFile().getFullPathName();
        output.writeText(filePath + "\n", false, false, nullptr);
    }
}

void PlaylistComponent::loadLibrary()
{
    // Points to the same file of persistance.
    juce::File libraryFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("OtoDecksLibrary.txt");
    
    if (libraryFile.existsAsFile())
    {
        // Read all ines from file once.
        juce::StringArray lines;
        libraryFile.readLines(lines);
        
        // Build the objects Track and alocated in memory.
        for (juce::String line : lines)
        {
            if (line.isNotEmpty())
            {
                juce::File file{line};
                
                if (file.exists()) {
                    Track newTrack;
                    newTrack.title = file.getFileName();
                    newTrack.url = juce::URL{file};
                    newTrack.format = file.getFileExtension();
                    
                    juce::AudioFormatManager formatManager;
                    formatManager.registerBasicFormats();
                    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
                    
                    if (reader != nullptr) {
                        newTrack.lengthInSecs = reader->lengthInSamples / reader->sampleRate;
                    } else {
                        newTrack.lengthInSecs = 0;
                    }
                    
                    tracks.push_back(newTrack);
              }
            }
        }
        // Updates table with new loaded data. 
        tableComponent.updateContent();
    }
}