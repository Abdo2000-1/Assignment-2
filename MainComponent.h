#pragma once

#include <JuceHeader.h>
#include "PlayerGUI.h"
#include "PlayerAudio.h"

namespace juce { class PropertiesFile; }

struct AppTheme
{
    juce::Colour bg;
    juce::Colour guiBg;
    juce::Colour text;
    juce::Colour accent;
    juce::Colour accentDark;
    juce::Colour playhead;
};

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
    void skipButtonClicked(PlayerGUI* whichGui) override;
    void backButtonClicked(PlayerGUI* whichGui) override;
    void volumeSliderChanged(PlayerGUI* whichGui, float newValue) override;
    void speedSliderChanged(PlayerGUI* whichGui, float newValue) override;
    void loopButtonClicked(PlayerGUI* whichGui) override;
    void progressSliderChanged(PlayerGUI* whichGui, double newValue) override;
    void setAButtonClicked(PlayerGUI* whichGui) override;
    void setBButtonClicked(PlayerGUI* whichGui) override;
    void abLoopToggleButtonClicked(PlayerGUI* whichGui) override;
    void setCueButtonClicked(PlayerGUI* whichGui) override;
    void cueButtonClicked(PlayerGUI* whichGui) override;

    // ===== Slider Listener Crossfader) =====
    void sliderValueChanged(juce::Slider* slider) override;

    // ===== Session State =====
    void saveSessionState(juce::PropertiesFile& props);
    void loadSessionState(juce::PropertiesFile& props);


private:
    PlayerGUI gui1;
    PlayerGUI gui2;
    PlayerAudio player1;
    PlayerAudio player2;

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

    juce::TextButton themeToggleButton;
    bool isDarkMode = true;
    AppTheme currentTheme;
    void updateTheme();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};