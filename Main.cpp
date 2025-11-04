#include <JuceHeader.h>
#include "MainComponent.h"
#include <fstream>
#include <iostream>
using namespace std;




class AudioPlayer : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "Audio Player"; }
    const juce::String getApplicationVersion() override { return "1.0"; }

    void initialise(const juce::String&) override
    {
        mainWindow = make_unique<MainWindow>(getApplicationName());

        // Get the session if exists
        std::ifstream sessionFile(".session");
        MainComponent* mainComp = mainWindow->getMainComponent();

        PlayerAudio& audio = mainComp->getPlayerAudio();
        juce::AudioTransportSource* player = &audio.getTransportSource();
        PlayerGUI* guiPtr = mainComp->getPlayerGUI();
        

        if (sessionFile.good()) {
            std::string fullPath;
            double currentPos = 0.0;
            std::getline(sessionFile, fullPath);  
            sessionFile >> currentPos;      

            // Load the file
            juce::File f = juce::File(fullPath);
            mainComp->setFile(f);
            audio.loadFile(f);
            player->setPosition(currentPos);
            guiPtr->setFileName(f.getFileName());
        }
    }

    void shutdown() override
    {
        mainWindow = nullptr;
    }


private:
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name)
            : DocumentWindow(name, juce::Colours::lightgrey, DocumentWindow::allButtons)
        {
            mainComponent = std::make_unique<MainComponent>();
            setContentOwned(mainComponent.get(), true);
            centreWithSize(getWidth(), getHeight());
            setVisible(true);
        }

        MainComponent* getMainComponent() { return mainComponent.get(); }

        void closeButtonPressed() override
        {
            auto* mainComp = getMainComponent();
            if (mainComp != nullptr)
            {
                PlayerAudio& audio = mainComp->getPlayerAudio();

                double currentTimeInSec = audio.getCurrentPosition();
                juce::File loadedFile = mainComp->getFile();

                std::ofstream file(".session");
                if (file.good())
                    file << loadedFile.getFullPathName() << std::endl << currentTimeInSec;
            }

            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        std::unique_ptr<MainComponent> mainComponent;
    };


    unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(AudioPlayer)
