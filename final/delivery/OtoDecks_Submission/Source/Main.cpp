#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

class OtoDecksApplication  : public JUCEApplication
{
public:

    OtoDecksApplication() {}
    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }


    void initialise (const String& /*commandLine*/) override
    {
        // This method is where you should put your application's initialisation code.
        // Creating the main window and setting the app name
        mainWindow.reset (new MainWindow (getApplicationName()));
    }

    void shutdown() override
    {
    
        // Cleaning up the main window to close the app properly
        mainWindow = nullptr; // (deletes our window)
    }


    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& /*commandLine*/) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }


    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    Desktop::getInstance().getDefaultLookAndFeel()
                                                                          .findColour (ResizableWindow::backgroundColourId),
                                                    DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            
            // Adding our MainComponent to the window
            setContentOwned (new MainComponent(), true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            // Setting the window to be resizable and centring it on the screen
            setResizable (true, true);
            centreWithSize (getWidth(), getHeight());
           #endif

            // Making the window visible to the user
            setVisible (true);
        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }


    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    // Smart pointer to manage the main window's lifetime
    std::unique_ptr<MainWindow> mainWindow;
};

// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (OtoDecksApplication)