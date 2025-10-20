#pragma once
#include <JuceHeader.h>

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener
{
public:
    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void loadButtonClicked() = 0;
        virtual void playPauseButtonClicked() = 0;
        virtual void stopButtonClicked() = 0;
        virtual void muteButtonClicked() = 0;
        virtual void volumeSliderChanged(float newValue) = 0;
    };

    PlayerGUI();
    ~PlayerGUI() override = default;

    void resized() override;
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    void addListener(Listener* listenerToAdd); 

    void setFileName(const juce::String& name);
    void setMuteButtonText(const juce::String& text);
    void setPlayButtonText(const juce::String& text);

private:
    Listener* listener = nullptr;

  //  string mute_emoji_utf8 = "\xF0\x9F\x94\x87";

    juce::TextButton loadBtn{ "Load" };
    juce::TextButton playPauseBtn{ "Play" };
    juce::TextButton stopBtn{ "Stop" };
    juce::TextButton muteBtn{ "Mute"};
    juce::Slider volSlider;
    juce::Label fileLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};