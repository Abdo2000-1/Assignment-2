#include <JuceHeader.h>
#include "MainComponent.h"
using namespace std;///////////////////////Last Edit \\\\\\\\\\\\\\\\\\\

class SimpleAudioPlayer : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "Al Ahly Audio Player"; }
    const juce::String getApplicationVersion() override { return "2.0"; }

    void initialise(const juce::String&) override
    {
        mainWindow = std::make_unique<MainWindow>(getApplicationName());
        
        MainComponent* mainComp = mainWindow->getMainComponent();

        auto* session = mainComp->getSessionManager();

        juce::uint64 uid1 = mainComp->getPlayerAudio1()->getFile()->getFileIdentifier();
        juce::String fileID1 = std::to_string(uid1);

        juce::uint64 uid2 = mainComp->getPlayerAudio2()->getFile()->getFileIdentifier();
        juce::String fileID2 = std::to_string(uid2);

        session->saveSession(fileID1, mainComp->getPlayerAudio1()->getCurrentPosition());
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
            : DocumentWindow(name,
                juce::Colour::fromRGB(30, 35, 50), 
                DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            mainComponent = new MainComponent();
            setContentOwned(mainComponent, true);
           
            setResizable(true, true);
            setResizeLimits(500, 700, 1200, 2000); 
            centreWithSize(500, 400); 
          

            setVisible(true);
        }

        void closeButtonPressed() override
        {
            
            auto* session = mainComponent->getSessionManager();


            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }

        MainComponent* getMainComponent() const
        {
            return mainComponent;
        }
    private:
        MainComponent* mainComponent = nullptr;
    };

    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(SimpleAudioPlayer)