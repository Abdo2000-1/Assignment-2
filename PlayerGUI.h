#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"
//Last Edition


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
        virtual void loopButtonClicked() = 0;               
        virtual void volumeSliderChanged(float newValue) = 0;
    };

    PlayerGUI();
    ~PlayerGUI() override = default;

    void resized() override;
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    void addListener(Listener* listenerToAdd);
    void timerCallback() override;
    void setPlayerAudioSource(PlayerAudio& p_player);
    void setFileName(const juce::String& name);
    void setMuteButtonText(const juce::String& text);
    void setPlayButtonText(const juce::String& text);
    void skipButtonText(const juce::String& text);
    void backButtonText(const juce::String& text);
    void setLoopButtonText(const juce::String& text);      

private:
    Listener* listener = nullptr;
    PlayerAudio* playerAudioSource{ nullptr };

    void paint(juce::Graphics& g) override;

    juce::TextButton loadBtn{ "Open" };
    juce::TextButton playPauseBtn{ juce::String::fromUTF8("\xE2\x96\xB6") };
    juce::TextButton stopBtn{ juce::String::fromUTF8("\xE2\x8F\xB9") };
    juce::TextButton muteBtn{ juce::String::fromUTF8("\xF0\x9F\x94\x8A") };
    juce::TextButton backBtn{ juce::String::fromUTF8("\xE2\x8F\xAA") };
    juce::TextButton skipBtn{ juce::String::fromUTF8("\xE2\x8F\xA9") };
    juce::TextButton loopBtn{ juce::String::fromUTF8("\xF0\x9F\x94\x81") };           

    juce::Slider progressSlider;
    juce::Slider volSlider;

    juce::Label fileLabel;

    bool isLooping = false;                              

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
