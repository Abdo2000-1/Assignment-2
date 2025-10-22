#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h" 
#include "PlayerGUI.h"

class MainComponent : public juce::AudioAppComponent, public PlayerGUI::Listener
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int, double) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& info) override;
    void releaseResources() override;
    void paint(juce::Graphics&) override;
    void resized() override;

    void loadButtonClicked() override;
    void playPauseButtonClicked() override;
    void stopButtonClicked() override;
    void muteButtonClicked() override;
    void skipButtonClicked() override;
    void backButtonClicked() override;

    void volumeSliderChanged(float newValue) override;

private:
    PlayerAudio player;
    PlayerGUI gui;
    std::unique_ptr<juce::FileChooser> chooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};