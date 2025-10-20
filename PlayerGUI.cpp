#include "PlayerGUI.h"

using namespace std;

PlayerGUI::PlayerGUI()
{
    addAndMakeVisible(loadBtn);
    addAndMakeVisible(playPauseBtn);
    addAndMakeVisible(stopBtn);
    addAndMakeVisible(muteBtn);//20242201
    addAndMakeVisible(volSlider);
    addAndMakeVisible(fileLabel);

    volSlider.setRange(0.0, 1.0, 0.01);
    volSlider.setValue(0.5);

    loadBtn.addListener(this);
    playPauseBtn.addListener(this);
    stopBtn.addListener(this);
    muteBtn.addListener(this);//20242201
    volSlider.addListener(this);

    fileLabel.setText("No file loaded", juce::dontSendNotification);
}

void PlayerGUI::addListener(Listener* listenerToAdd)
{
    listener = listenerToAdd;
}

void PlayerGUI::resized()
{
    int y = 20;
    int buttonWidth = 70;
    int gap = 10;
    loadBtn.setBounds(20, y, 90, 40);
    playPauseBtn.setBounds(loadBtn.getRight() + gap, y, buttonWidth, 40);
    stopBtn.setBounds(playPauseBtn.getRight() + gap, y, buttonWidth, 40);
    muteBtn.setBounds(30, 200, 50, 40);//20242201
    volSlider.setBounds(20, 100, getWidth() - 40, 30);
    fileLabel.setBounds(20, 150, getWidth() - 40, 30);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (listener) 
    {
        if (button == &loadBtn) listener->loadButtonClicked();
        else if (button == &playPauseBtn) listener->playPauseButtonClicked();
        else if (button == &stopBtn) listener->stopButtonClicked();
        else if (button == &muteBtn) listener->muteButtonClicked();
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (listener && slider == &volSlider)
    {
        listener->volumeSliderChanged((float)slider->getValue());
    }
}

void PlayerGUI::setFileName(const juce::String& name) { fileLabel.setText(name, juce::dontSendNotification); }
void PlayerGUI::setMuteButtonText(const juce::String& text) { muteBtn.setButtonText(text); }
void PlayerGUI::setPlayButtonText(const juce::String& text) { playPauseBtn.setButtonText(text); }