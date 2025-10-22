#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer
{
public:
    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void loadButtonClicked() = 0;
        virtual void playPauseButtonClicked() = 0;
        virtual void stopButtonClicked() = 0;
        virtual void backButtonClicked() = 0;
        virtual void skipButtonClicked() = 0;
        virtual void muteButtonClicked() = 0;
        virtual void volumeSliderChanged(float newValue) = 0;
    };

    PlayerGUI();
    ~PlayerGUI() override = default;

    void resized() override;
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    void addListener(Listener* listenerToAdd);
    void timerCallback() override;
    void setPlayerAudioSource(PlayerAudio& p_player)
    {
        playerAudioSource = &p_player;
    }
    void setFileName(const juce::String& name);
    void setMuteButtonText(const juce::String& text);
    void setPlayButtonText(const juce::String& text);
    void skipButtonText(const juce::String& text);
    void backButtonText(const juce::String& text);


private:
    Listener* listener = nullptr;

    //std::unique_ptr<juce::Drawable> forwardIcon;
    //std::unique_ptr<juce::Drawable> backwardIcon;
    //  string mute_emoji_utf8 = "\xF0\x9F\x94\x87";

    PlayerAudio* playerAudioSource{ nullptr };
    void paint(juce::Graphics& g) override;

    juce::TextButton loadBtn{ "Load" };
    juce::TextButton playPauseBtn{ "Play" };
    juce::TextButton stopBtn{ "Stop" };
    juce::TextButton muteBtn{ "Mute" };
    juce::TextButton backBtn{ "-10s" };
    juce::TextButton skipBtn{ "10s"};

    juce::Slider progressSlider;
    juce::Slider volSlider;

    juce::Label fileLabel;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};