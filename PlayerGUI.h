#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"
#include "WaveformDisplay.h" 


class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer,
    public WaveformDisplay::Listener 
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
        virtual void marksMenuChanged(PlayerGUI* whichGui) = 0;
        virtual void markButtonClicked(PlayerGUI* whichGui) = 0;
        virtual void skipButtonClicked(PlayerGUI* whichGui) = 0;
        virtual void muteButtonClicked(PlayerGUI* whichGui) = 0;
        virtual void loopButtonClicked(PlayerGUI* whichGui) = 0;
        virtual void deleteMarkRequested(PlayerGUI* whichGui, int selectedID) = 0;
        virtual void volumeSliderChanged(PlayerGUI* whichGui, float newValue) = 0;
        virtual void speedSliderChanged(PlayerGUI* whichGui, float newValue) = 0;

        virtual void progressSliderChanged(PlayerGUI* whichGui, double newValue) = 0;
        virtual void setAButtonClicked(PlayerGUI* whichGui) = 0;
        virtual void setBButtonClicked(PlayerGUI* whichGui) = 0;
        virtual void abLoopToggleButtonClicked(PlayerGUI* whichGui) = 0;
    };

    PlayerGUI();
    ~PlayerGUI() override = default;

    void resized() override;
    void paint(juce::Graphics& g) override;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;

    // --- Listener for Waveform ---
    void waveformClicked(double normalizedPosition) override; 

    void addListener(Listener* listenerToAdd);
    void setPlayerAudioSource(PlayerAudio& player);
    void marksMenuChanged(PlayerGUI* whichGui);
    void deleteMarkBtnClicked(PlayerGUI* whichGui);

    void loadFileForWaveform(const juce::File& file); 


    // ---- Setters ----
    void setFileName(const juce::String& name);
    void setMuteButtonText(const juce::String& text);
    void setPlayButtonText(const juce::String& text);
    void backButtonText(const juce::String& text);
    void skipButtonText(const juce::String& text);
    void setLoopButtonText(const juce::String& text);
    void setABLoopButtonText(const juce::String& text);

    void setDeckName(const juce::String& name);

    juce::ComboBox* getMarksMenu() {
        return &marksMenu;
    }


private:
    Listener* listener = nullptr;
    PlayerAudio* playerAudioSource{ nullptr };

    // For loading waveform thumbnails
    juce::AudioFormatManager formatManager;

    juce::TextButton loadBtn{ "Open" };
    juce::TextButton playPauseBtn{ juce::String::fromUTF8("\xE2\x96\xB6") };
    juce::TextButton stopBtn{ juce::String::fromUTF8("\xE2\x8F\xB9") };
    juce::TextButton markBtn{ juce::String::fromUTF8("\xF0\x9F\x94\x96") };
    juce::TextButton muteBtn{ juce::String::fromUTF8("\xF0\x9F\x94\x8A") };
    juce::TextButton backBtn{ juce::String::fromUTF8("\xE2\x8F\xAA") };
    juce::TextButton skipBtn{ juce::String::fromUTF8("\xE2\x8F\xA9") };
    juce::TextButton deleteMarkBtn{ "X" };
    juce::TextButton loopBtn{ juce::String::fromUTF8("\xF0\x9F\x94\x81") };

    juce::TextButton setABtn{ "Set A" };
    juce::TextButton setBBtn{ "Set B" };
    juce::TextButton abLoopToggleBtn{ "A-B" };


    WaveformDisplay waveformDisplay; 

    juce::Slider volSlider;
    juce::Slider speedSlider;

    juce::Label fileLabel;
    juce::Label speedLabel;
    juce::Label deckNameLabel;

    juce::ComboBox marksMenu;

    // ----------------------------

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};