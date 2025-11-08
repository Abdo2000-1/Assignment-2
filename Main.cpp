#include <JuceHeader.h>
#include "MainComponent.h"
using namespace std;//Al Ahly Audio Player Version 2.0 //////

class SimpleAudioPlayer : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "Al Ahly Audio Player"; }
    const juce::String getApplicationVersion() override { return "2.0"; }

    void initialise(const juce::String&) override
    {
        
        juce::PropertiesFile::Options options;
        options.applicationName = getApplicationName();
        options.filenameSuffix = ".properties";
        options.folderName = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
            .getChildFile(getApplicationName()).getFullPathName();

       
        juce::File(options.folderName).createDirectory();


        propsFile = std::make_unique<juce::PropertiesFile>(options);

       
        mainWindow = std::make_unique<MainWindow>(getApplicationName(), *propsFile);
        
    }

    void shutdown() override
    {
       
        if (mainWindow != nullptr)
        {
            if (auto* mainComp = mainWindow->getMainComponent())
            {
                mainComp->saveSessionState(*propsFile);
            }
        }

      
        propsFile->saveIfNeeded();
        

        mainWindow = nullptr;
        propsFile = nullptr; 
    }

private:
    class MainWindow : public juce::DocumentWindow
    {
    public:
        
        MainWindow(juce::String name, juce::PropertiesFile& props) 
            : DocumentWindow(name,
                juce::Colour::fromRGB(30, 35, 50),
                DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);

            
            mainComponent = new MainComponent();
            setContentOwned(mainComponent, true);

            
            mainComponent->loadSessionState(props);
            

            setResizable(true, true);
            setResizeLimits(500, 700, 1200, 2000);
            centreWithSize(500, 400);

            setVisible(true);
        }

        void closeButtonPressed() override
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }

        MainComponent* getMainComponent() { return mainComponent; }
        

    private:
       
        MainComponent* mainComponent; 
     
    };

    std::unique_ptr<MainWindow> mainWindow;

    
    std::unique_ptr<juce::PropertiesFile> propsFile;
    
};

START_JUCE_APPLICATION(SimpleAudioPlayer)