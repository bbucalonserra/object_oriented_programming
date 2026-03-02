# Created and Modified Code

## File DJAudioPlayer.cpp
```cpp
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
        
        // Exception Handling: Visual feedback for unsupported file formats.
        juce::AlertWindow::showMessageBoxAsync (
            juce::AlertWindow::WarningIcon,
            "Format Error",
            "The selected file is not supported.",
            "Ok"
        );
    }
}
```


## File DeckGUI.cpp
```cpp

```

## File PlaylistComponent.h
```cpp

```

## File PlaylistComponent.cpp
```cpp

```