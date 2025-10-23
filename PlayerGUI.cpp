#include "PlayerGUI.h"
#include "PlayerAudio.h"

PlayerGUI::PlayerGUI()
{
    addAndMakeVisible(loadBtn);
    addAndMakeVisible(playPauseBtn);
    addAndMakeVisible(stopBtn);
    addAndMakeVisible(skipBtn);
    addAndMakeVisible(backBtn);
    addAndMakeVisible(muteBtn);
    addAndMakeVisible(loopBtn);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(progressSlider);
    addAndMakeVisible(fileLabel);

    volSlider.setRange(0.0, 1.0, 0.01);
    volSlider.setValue(0.5);
    volSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    progressSlider.setRange(0.0, 1.0);
    progressSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    progressSlider.setSliderStyle(juce::Slider::LinearBar);

    startTimer(30);

    loadBtn.addListener(this);
    playPauseBtn.addListener(this);
    stopBtn.addListener(this);
    muteBtn.addListener(this);
    volSlider.addListener(this);
    backBtn.addListener(this);
    skipBtn.addListener(this);
    loopBtn.addListener(this);

    fileLabel.setColour(juce::Label::textColourId, juce::Colours::darkgrey);
    fileLabel.setText("No file loaded", juce::dontSendNotification);
}

void PlayerGUI::setPlayerAudioSource(PlayerAudio& audioSource)
{
    playerAudioSource = &audioSource;
}

void PlayerGUI::timerCallback()
{
    if (playerAudioSource != nullptr)
    {
        double currentTime = playerAudioSource->getCurrentPosition();
        double totalLength = playerAudioSource->getLengthInSeconds();

        if (totalLength > 0.0)
            progressSlider.setValue(currentTime / totalLength);
    }
}

void PlayerGUI::addListener(Listener* listenerToAdd) { listener = listenerToAdd; }

void PlayerGUI::resized()
{
    auto bounds = getLocalBounds();
    int side = 50, gap = 10;
    progressSlider.setBounds(50, bounds.getBottom() - 100, bounds.getWidth() - 100, 10);

    int y = progressSlider.getBottom() - 70;

    loadBtn.setBounds(20, 10, 60, 30);
    playPauseBtn.setBounds(bounds.getCentreX() - 150, y, side, side);
    stopBtn.setBounds(playPauseBtn.getRight() + gap, y, side, side);
    backBtn.setBounds(stopBtn.getRight() + gap, y, side, side);
    skipBtn.setBounds(backBtn.getRight() + gap, y, side, side);
    volSlider.setBounds(bounds.getCentreX() - 102, progressSlider.getBottom() + 20, 150, 30);
    muteBtn.setBounds(volSlider.getRight() - 5, volSlider.getY(), 50, 30);
    loopBtn.setBounds(muteBtn.getRight() + 10, volSlider.getY(), 80, 30);
    fileLabel.setBounds(20, loadBtn.getBottom() + 10, getWidth() - 40, 30);
}

void PlayerGUI::paint(juce::Graphics& g) { g.fillAll(juce::Colours::lightgrey); }

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (!listener) return;

    if (button == &loadBtn) listener->loadButtonClicked();
    else if (button == &playPauseBtn) listener->playPauseButtonClicked();
    else if (button == &stopBtn) listener->stopButtonClicked();
    else if (button == &muteBtn) listener->muteButtonClicked();
    else if (button == &skipBtn) listener->skipButtonClicked();
    else if (button == &backBtn) listener->backButtonClicked();
    else if (button == &loopBtn) listener->loopButtonClicked();
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (listener && slider == &volSlider)
        listener->volumeSliderChanged((float)slider->getValue());
}

void PlayerGUI::setFileName(const juce::String& name) { fileLabel.setText(name, juce::dontSendNotification); }
void PlayerGUI::setMuteButtonText(const juce::String& text) { muteBtn.setButtonText(text); }
void PlayerGUI::setPlayButtonText(const juce::String& text) { playPauseBtn.setButtonText(text); }
void PlayerGUI::setLoopButtonText(const juce::String& text) { loopBtn.setButtonText(text); }
