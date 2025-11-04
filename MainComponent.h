#pragma once

#include <JuceHeader.h>
#include "PlayerGUI.h"
#include "PlayerAudio.h"

class MainComponent : public juce::AudioAppComponent,
    public PlayerGUI::Listener,
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

    // ===== GUI Button Listeners =====
    void loadButtonClicked() override;
    void playPauseButtonClicked() override;
    void stopButtonClicked() override;
    void muteButtonClicked() override;
    void skipButtonClicked() override;
    void backButtonClicked() override;
    void volumeSliderChanged(float newValue) override;
    void speedSliderChanged(float newValue) override;
    void loopButtonClicked() override;
    void progressSliderChanged(double newValue) override;
    void setAButtonClicked() override;
    void setBButtonClicked() override;
    void abLoopToggleButtonClicked() override;

private:
    PlayerGUI gui;
    PlayerAudio player;
    std::unique_ptr<juce::FileChooser> chooser;
    void updateLoopButtonText();

    juce::ListBox playlistBox;
    juce::TextButton addButton{ "Add" };
    juce::TextButton deleteButton{ "Delete" }; 
    juce::TextButton clearButton{ "Clear" };  

    juce::StringArray playlistFiles;
    juce::Array<juce::File> playlistFileObjects;

    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void listBoxItemClicked(int row, const juce::MouseEvent&) override;

    // ===== Playlist control functions =====
    void addFileToPlaylist();
    void deleteSelectedItem(); 
    void clearPlaylist();      

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
