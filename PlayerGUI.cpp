#include "PlayerGUI.h"
#include "PlayerAudio.h"

//Last Edition

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
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(progressSlider);
    addAndMakeVisible(fileLabel);
    addAndMakeVisible(speedLabel);

    addAndMakeVisible(setABtn);
    addAndMakeVisible(setBBtn);
    addAndMakeVisible(abLoopToggleBtn);
    abLoopToggleBtn.setButtonText("A-B");

    volSlider.setRange(0.0, 1.0, 0.01);
    volSlider.setValue(0.5);
    volSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    speedSlider.setRange(0.5, 2.0, 0.01);
    speedSlider.setValue(1.0);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    progressSlider.setRange(0.0, 1.0);
    progressSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    progressSlider.setSliderStyle(juce::Slider::LinearBar);

    startTimer(30);

    loadBtn.addListener(this);
    playPauseBtn.addListener(this);
    stopBtn.addListener(this);
    muteBtn.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    backBtn.addListener(this);
    skipBtn.addListener(this);
    loopBtn.addListener(this);
    progressSlider.addListener(this);

    setABtn.addListener(this);
    setBBtn.addListener(this);
    abLoopToggleBtn.addListener(this);

    fileLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    fileLabel.setText("No file loaded", juce::dontSendNotification);

    speedLabel.setColour(juce::Label::textColourId, juce::Colours::darkgrey);
    speedLabel.setText("1.00x", juce::dontSendNotification);
    speedLabel.setJustificationType(juce::Justification::centred);
}

void PlayerGUI::setPlayerAudioSource(PlayerAudio& audioSource)
{
    playerAudioSource = &audioSource;
}

void PlayerGUI::timerCallback()
{
    if (playerAudioSource != nullptr)
    {
        if (!progressSlider.isMouseButtonDown())
        {
            double currentTime = playerAudioSource->getCurrentPosition();
            double totalLength = playerAudioSource->getLengthInSeconds();

            if (totalLength > 0.0)
                progressSlider.setValue(currentTime / totalLength, juce::dontSendNotification);
        }
    }
}

void PlayerGUI::addListener(Listener* listenerToAdd)
{
    listener = listenerToAdd;
}

void PlayerGUI::resized()
{
    auto area = getLocalBounds().reduced(10);

    auto topRow = area.removeFromTop(40);
    loadBtn.setBounds(topRow.removeFromLeft(70));
    playPauseBtn.setBounds(topRow.removeFromLeft(70));
    stopBtn.setBounds(topRow.removeFromLeft(70));
    muteBtn.setBounds(topRow.removeFromLeft(70));
    backBtn.setBounds(topRow.removeFromLeft(70));
    skipBtn.setBounds(topRow.removeFromLeft(70));
    loopBtn.setBounds(topRow.removeFromLeft(70));

    auto abRow = area.removeFromTop(35);
    setABtn.setBounds(abRow.removeFromLeft(80).reduced(2));
    setBBtn.setBounds(abRow.removeFromLeft(80).reduced(2));
    abLoopToggleBtn.setBounds(abRow.removeFromLeft(80).reduced(2));

    auto sliderRow = area.removeFromTop(30);
    progressSlider.setBounds(sliderRow.reduced(5));

    auto controlRow = area.removeFromTop(50);
    volSlider.setBounds(controlRow.removeFromLeft(getWidth() / 2 - 15).reduced(10));
    speedSlider.setBounds(controlRow.reduced(10));

    fileLabel.setBounds(area.removeFromTop(80).reduced(10));
    fileLabel.setJustificationType(juce::Justification::centredLeft);
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(35, 60, 40));
    g.setColour(juce::Colours::white);
    g.setFont(14.0f);

    g.drawText("Audio Player", 10, 5, getWidth(), 20, juce::Justification::centredLeft);
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (listener && slider == &volSlider)
        listener->volumeSliderChanged((float)slider->getValue());
    else if (listener && slider == &speedSlider)
    {
        speedLabel.setText(juce::String(slider->getValue(), 2) + "x", juce::dontSendNotification);
        listener->speedSliderChanged((float)slider->getValue());
    }
    else if (listener && slider == &progressSlider)
    {
        if (slider->isMouseButtonDown())
            listener->progressSliderChanged(slider->getValue());
    }
}

void PlayerGUI::setFileName(const juce::String& name)
{
    fileLabel.setText(name, juce::dontSendNotification);
}

void PlayerGUI::setMuteButtonText(const juce::String& text)
{
    muteBtn.setButtonText(text);
}

void PlayerGUI::setPlayButtonText(const juce::String& text)
{
    playPauseBtn.setButtonText(text);
}

void PlayerGUI::setLoopButtonText(const juce::String& text)
{
    loopBtn.setButtonText(text);
}

void PlayerGUI::setABLoopButtonText(const juce::String& text)
{
    abLoopToggleBtn.setButtonText(text);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (listener == nullptr)
        return;

    if (button == &loadBtn)
        listener->loadButtonClicked();
    else if (button == &playPauseBtn)
        listener->playPauseButtonClicked();
    else if (button == &stopBtn)
        listener->stopButtonClicked();
    else if (button == &backBtn)
        listener->backButtonClicked();
    else if (button == &skipBtn)
        listener->skipButtonClicked();
    else if (button == &muteBtn)
        listener->muteButtonClicked();
    else if (button == &loopBtn)
        listener->loopButtonClicked();
    else if (button == &setABtn)
        listener->setAButtonClicked();
    else if (button == &setBBtn)
        listener->setBButtonClicked();
    else if (button == &abLoopToggleBtn)
        listener->abLoopToggleButtonClicked();
}
