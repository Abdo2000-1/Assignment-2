#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"
#include "WaveformDisplay.h" 

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
        virtual void loadButtonClicked(PlayerGUI* whichGui) = 0;
        virtual void playPauseButtonClicked(PlayerGUI* whichGui) = 0;
        virtual void stopButtonClicked(PlayerGUI* whichGui) = 0;
        virtual void backButtonClicked(PlayerGUI* whichGui) = 0;
        virtual void skipButtonClicked(PlayerGUI* whichGui) = 0;
        virtual void muteButtonClicked(PlayerGUI* whichGui) = 0;
        virtual void loopButtonClicked(PlayerGUI* whichGui) = 0;
        virtual void volumeSliderChanged(PlayerGUI* whichGui, float newValue) = 0;
        virtual void speedSliderChanged(PlayerGUI* whichGui, float newValue) = 0;

        virtual void progressSliderChanged(PlayerGUI* whichGui, double newValue) = 0;
        virtual void setAButtonClicked(PlayerGUI* whichGui) = 0;
        virtual void setBButtonClicked(PlayerGUI* whichGui) = 0;
        virtual void abLoopToggleButtonClicked(PlayerGUI* whichGui) = 0;

        virtual void setCueButtonClicked(PlayerGUI* whichGui) = 0;
        virtual void cueButtonClicked(PlayerGUI* whichGui) = 0;
    };

    PlayerGUI(juce::AudioFormatManager& formatManager);
    ~PlayerGUI() override = default;

    void resized() override;
    void paint(juce::Graphics& g) override;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;

    void addListener(Listener* listenerToAdd);
    void setPlayerAudioSource(PlayerAudio& player);

    void loadFileForThumbnail(const juce::File& file);
    void setCueMarker(double normalizedPosition);
    void setProgress(double normalizedPosition);

    void setTheme(juce::Colour bg, juce::Colour text, juce::Colour accent, juce::Colour playhead);


    void setMetadata(const juce::String& title, const juce::String& artist, const juce::String& duration);


    void setMuteButtonText(const juce::String& text);
    void setPlayButtonText(const juce::String& text);
    void backButtonText(const juce::String& text);
    void skipButtonText(const juce::String& text);
    void setLoopButtonText(const juce::String& text);
    void setABLoopButtonText(const juce::String& text);

    void setDeckName(const juce::String& name);

private:
    Listener* listener = nullptr;
    PlayerAudio* playerAudioSource{ nullptr };

    juce::TextButton loadBtn{ "Open" };
    juce::TextButton playPauseBtn{ juce::String::fromUTF8("\xE2\x96\xB6") };
    juce::TextButton stopBtn{ juce::String::fromUTF8("\xE2\x8F\xAE") };
    juce::TextButton muteBtn{ juce::String::fromUTF8("\xF0\x9F\x94\x8A") };
    juce::TextButton backBtn{ juce::String::fromUTF8("\xE2\x8F\xAA") };
    juce::TextButton skipBtn{ juce::String::fromUTF8("\xE2\x8F\xA9") };
    juce::TextButton loopBtn{ juce::String::fromUTF8("\xF0\x9F\x94\x81") };

    juce::TextButton setABtn{ "Set A" };
    juce::TextButton setBBtn{ "Set B" };
    juce::TextButton abLoopToggleBtn{ "A-B" };



    juce::TextButton setButton{ juce::String::fromUTF8("\xF0\x9F\x93\x8D") };
    juce::TextButton cueButton{ juce::String::fromUTF8("Go") };

    juce::Slider progressSlider;
    WaveformDisplay waveformDisplay;
    juce::Slider volSlider;
    juce::Slider speedSlider;


    juce::Label titleLabel;
    juce::Label artistLabel;
    juce::Label durationLabel;

    juce::Label deckNameLabel;
    juce::Label currentTimeLabel;
    juce::Label totalTimeLabel;

    juce::Label volLabel;
    juce::Label speedLabel;

    juce::String formatTime(double seconds);

    juce::Colour bgColour;
    juce::Colour textColour;
    juce::Colour accentColour;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};