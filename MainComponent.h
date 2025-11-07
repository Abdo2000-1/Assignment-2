#pragma once

#include <JuceHeader.h>
#include "PlayerGUI.h"
#include "PlayerAudio.h"
#include "MarkManager.h"
#include "SessionManager.h"

class MainComponent : public juce::AudioAppComponent,
    public PlayerGUI::Listener,
    public juce::Slider::Listener,
    public juce::ListBoxModel
{
public:
    MainComponent();
    ~MainComponent() override;

    // ===== AudioAppComponent =====
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void paint(juce::Graphics& g) override;
    void resized() override;

    // ===== GUI Listeners =====
    void loadButtonClicked(PlayerGUI* whichGui) override;
    void playPauseButtonClicked(PlayerGUI* whichGui) override;
    void stopButtonClicked(PlayerGUI* whichGui) override;
    void muteButtonClicked(PlayerGUI* whichGui) override;
    void markButtonClicked(PlayerGUI* whichGui) override;
    void skipButtonClicked(PlayerGUI* whichGui) override;
    void backButtonClicked(PlayerGUI* whichGui) override;
    void marksMenuChanged(PlayerGUI* whichGui) override;
    void volumeSliderChanged(PlayerGUI* whichGui, float newValue) override;
    void speedSliderChanged(PlayerGUI* whichGui, float newValue) override;
    void loopButtonClicked(PlayerGUI* whichGui) override;
    void progressSliderChanged(PlayerGUI* whichGui, double newValue) override;
    void setAButtonClicked(PlayerGUI* whichGui) override;
    void setBButtonClicked(PlayerGUI* whichGui) override;
    void abLoopToggleButtonClicked(PlayerGUI* whichGui) override;
    void deleteMarkRequested(PlayerGUI* whichGui, int selectedID);

    void updateMarksMenu(PlayerGUI* whichGui) {
        PlayerGUI* gui = (whichGui == &gui1) ? &gui1 : &gui2;
        PlayerAudio* player = (whichGui == &gui1) ? &player1 : &player2;

        juce::Array<double> marks =  markManager.getMarks(std::to_string(player->getFile()->getFileIdentifier()));

        juce::ComboBox* marksMenu= gui->getMarksMenu();
        marksMenu->clear();

        for (int i = 1; i <= marks.size(); i++)
        {
            marksMenu->addItem(juce::String(i) + " [" + juce::String(marks[i - 1]) + " ]", i);
        }

    }

    // ===== Slider Listener Crossfader) =====
    void sliderValueChanged(juce::Slider* slider) override;

    SessionManager* getSessionManager() {
        return &sessionManager;
    }

    PlayerAudio* getPlayerAudio1() {
        return &player1;
    }

    PlayerAudio* getPlayerAudio2() {
        return &player2;
    }

private:
    PlayerGUI gui1;
    PlayerGUI gui2;
    PlayerAudio player1;
    PlayerAudio player2;

    juce::File marksFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
        .getChildFile("marks.json");

    juce::File sessionFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
        .getChildFile("session.json");

    MarkManager markManager{ marksFile };
    SessionManager sessionManager{ sessionFile };

    juce::MixerAudioSource mixer;

    juce::TextButton mixModeButton{ "Mix Mode" };
    bool isInMixMode{ false };

  
    juce::Slider crossfader;
    juce::Label crossfaderLabel;
    juce::Label deck1Label;
    juce::Label deck2Label;

    
    float player1ChannelVolume{ 0.5f };
    float player2ChannelVolume{ 0.5f };

  
    void updateGains();
   

    std::unique_ptr<juce::FileChooser> chooser;
    void updateLoopButtonText(PlayerGUI& gui, PlayerAudio& player);

  
    juce::ListBox playlistBox;
    juce::TextButton addButton{ "Add" };
    juce::TextButton deleteButton{ "Delete" };
    juce::TextButton clearButton{ "Clear" };

    juce::StringArray playlistFiles;
    juce::Array<juce::File> playlistFileObjects;

    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void listBoxItemClicked(int row, const juce::MouseEvent&) override;

    void addFileToPlaylist();
    void deleteSelectedItem();
    void clearPlaylist();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};