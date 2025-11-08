#include "PlayerGUI.h"
#include "PlayerAudio.h"

juce::String PlayerGUI::formatTime(double seconds)
{
    int totalSecs = static_cast<int>(std::round(seconds));
    int mins = totalSecs / 60;
    int secs = totalSecs % 60;
    return juce::String::formatted("%02d:%02d", mins, secs);
}

PlayerGUI::PlayerGUI(juce::AudioFormatManager& formatManager)
    : waveformDisplay(formatManager)
{
    bgColour = juce::Colour::fromRGB(40, 50, 90);
    textColour = juce::Colours::lightsteelblue;
    accentColour = juce::Colours::cornflowerblue;

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
    addAndMakeVisible(waveformDisplay);

    addAndMakeVisible(titleLabel);
    addAndMakeVisible(artistLabel);
    addAndMakeVisible(durationLabel);

    addAndMakeVisible(setABtn);
    addAndMakeVisible(setBBtn);
    addAndMakeVisible(abLoopToggleBtn);
    abLoopToggleBtn.setButtonText("A-B");
    addAndMakeVisible(setButton);
    addAndMakeVisible(cueButton);
    addAndMakeVisible(currentTimeLabel);
    addAndMakeVisible(totalTimeLabel);
    addAndMakeVisible(deckNameLabel);

    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);

    volSlider.setRange(0.0, 1.0, 0.01);
    volSlider.setValue(0.5);
    volSlider.setSliderStyle(juce::Slider::LinearVertical);
    volSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    speedSlider.setRange(0.5, 2.0, 0.01);
    speedSlider.setValue(1.0);
    speedSlider.setSliderStyle(juce::Slider::LinearVertical);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    progressSlider.setRange(0.0, 1.0);
    progressSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    progressSlider.setSliderStyle(juce::Slider::LinearBar);

    progressSlider.setColour(juce::Slider::backgroundColourId, bgColour.brighter(0.2f));
    progressSlider.setColour(juce::Slider::trackColourId, accentColour);
    progressSlider.setColour(juce::Slider::thumbColourId, textColour);

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

    waveformDisplay.onPositionChange = [this](double newValue)
        {
            if (listener)
                listener->progressSliderChanged(this, newValue);
        };

    setABtn.addListener(this);
    setBBtn.addListener(this);
    abLoopToggleBtn.addListener(this);
    setButton.addListener(this);
    cueButton.addListener(this);

    titleLabel.setColour(juce::Label::textColourId, textColour);
    titleLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    titleLabel.setText("Title: N/A", juce::dontSendNotification);

    artistLabel.setColour(juce::Label::textColourId, textColour);
    artistLabel.setFont(juce::Font(14.0f));
    artistLabel.setText("Artist: N/A", juce::dontSendNotification);

    durationLabel.setColour(juce::Label::textColourId, textColour.withAlpha(0.7f));
    durationLabel.setFont(juce::Font(14.0f));
    durationLabel.setText("Duration: 00:00", juce::dontSendNotification);

    currentTimeLabel.setColour(juce::Label::textColourId, textColour);
    currentTimeLabel.setText("00:00", juce::dontSendNotification);
    currentTimeLabel.setJustificationType(juce::Justification::centredLeft);

    totalTimeLabel.setColour(juce::Label::textColourId, textColour);
    totalTimeLabel.setText("00:00", juce::dontSendNotification);
    totalTimeLabel.setJustificationType(juce::Justification::centredRight);

    deckNameLabel.setColour(juce::Label::textColourId, textColour);
    deckNameLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    deckNameLabel.setJustificationType(juce::Justification::centred);

    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.setFont(juce::Font(12.0f));
    volLabel.setJustificationType(juce::Justification::centred);

    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setFont(juce::Font(12.0f));
    speedLabel.setJustificationType(juce::Justification::centred);
}

void PlayerGUI::setPlayerAudioSource(PlayerAudio& audioSource)
{
    playerAudioSource = &audioSource;
    waveformDisplay.setTransportSource(&audioSource.getTransportSource());
}

void PlayerGUI::loadFileForThumbnail(const juce::File& file)
{
    waveformDisplay.loadFile(file);
}

void PlayerGUI::timerCallback()
{
    if (playerAudioSource != nullptr)
    {
        double currentTime = playerAudioSource->getCurrentPosition();
        double totalLength = playerAudioSource->getLengthInSeconds();

        if (totalLength > 0.0)
        {
            if (!progressSlider.isMouseButtonDown())
            {
                progressSlider.setValue(currentTime / totalLength, juce::dontSendNotification);
            }

            currentTimeLabel.setText(formatTime(currentTime), juce::dontSendNotification);
            totalTimeLabel.setText(formatTime(totalLength), juce::dontSendNotification);
        }
        else
        {
            progressSlider.setValue(0.0, juce::dontSendNotification);
            currentTimeLabel.setText("00:00", juce::dontSendNotification);
            totalTimeLabel.setText("00:00", juce::dontSendNotification);
        }
    }
}

void PlayerGUI::addListener(Listener* listenerToAdd)
{
    listener = listenerToAdd;
}

void PlayerGUI::resized()
{
    auto bounds = getLocalBounds().reduced(10);

    int gap = 8;
    int tinyGap = 4;
    int timeLabelWidth = 50;
    int controlsAreaWidth = 40;
    int labelHeight = 20;

    auto controlsArea = bounds.removeFromRight(controlsAreaWidth);
    bounds.removeFromRight(gap);

    muteBtn.setBounds(controlsArea.removeFromTop(35).reduced(5));
    controlsArea.removeFromTop(gap);

    auto volArea = controlsArea.removeFromTop(controlsArea.getHeight() * 0.5f);
    auto speedArea = controlsArea;
    speedArea.removeFromTop(gap);

    volLabel.setBounds(volArea.removeFromTop(labelHeight));
    volSlider.setBounds(volArea.reduced(5, 0));

    speedLabel.setBounds(speedArea.removeFromTop(labelHeight));
    speedSlider.setBounds(speedArea.reduced(5, 0));


    float totalRowHeight = bounds.getHeight() - (4 * gap);

    float nameRowH = totalRowHeight * 0.12f;
    float metaRowH = totalRowHeight * 0.14f;
    float progressRowH = totalRowHeight * 0.24f;
    float transportRowH = totalRowHeight * 0.25f;
    float abLoopRowH = totalRowHeight * 0.25f;


    deckNameLabel.setBounds(bounds.removeFromTop(nameRowH));
    bounds.removeFromTop(gap);

    auto topRow = bounds.removeFromTop(metaRowH);
    loadBtn.setBounds(topRow.removeFromLeft(topRow.getWidth() * 0.25f));
    topRow.removeFromLeft(gap);
    titleLabel.setBounds(topRow.removeFromTop(topRow.getHeight() * 0.4f));
    artistLabel.setBounds(topRow.removeFromTop(topRow.getHeight() * 0.5f));
    durationLabel.setBounds(topRow);

    bounds.removeFromTop(gap);

    auto progressRowArea = bounds.removeFromTop(progressRowH);
    currentTimeLabel.setBounds(progressRowArea.removeFromLeft(timeLabelWidth));
    totalTimeLabel.setBounds(progressRowArea.removeFromRight(timeLabelWidth));
    progressRowArea.reduce(tinyGap, 0);
    waveformDisplay.setBounds(progressRowArea.removeFromTop(progressRowArea.getHeight() * 0.6f));
    progressRowArea.removeFromTop(tinyGap / 2);
    progressSlider.setBounds(progressRowArea);

    bounds.removeFromTop(gap);

    auto transportRow = bounds.removeFromTop(transportRowH);
    int btnCount = 5;
    float btnWidth = (transportRow.getWidth() - (gap * (btnCount - 1))) / (float)btnCount;

    backBtn.setBounds(transportRow.removeFromLeft(btnWidth));
    transportRow.removeFromLeft(gap);
    playPauseBtn.setBounds(transportRow.removeFromLeft(btnWidth));
    transportRow.removeFromLeft(gap);
    stopBtn.setBounds(transportRow.removeFromLeft(btnWidth));
    transportRow.removeFromLeft(gap);
    skipBtn.setBounds(transportRow.removeFromLeft(btnWidth));
    transportRow.removeFromLeft(gap);
    loopBtn.setBounds(transportRow);

    bounds.removeFromTop(gap);

    auto abRow = bounds.removeFromTop(abLoopRowH);
    btnCount = 5;
    btnWidth = (abRow.getWidth() - (gap * (btnCount - 1))) / (float)btnCount;

    setABtn.setBounds(abRow.removeFromLeft(btnWidth));
    abRow.removeFromLeft(gap);
    setBBtn.setBounds(abRow.removeFromLeft(btnWidth));
    abRow.removeFromLeft(gap);
    abLoopToggleBtn.setBounds(abRow.removeFromLeft(btnWidth));
    abRow.removeFromLeft(gap);
    setButton.setBounds(abRow.removeFromLeft(btnWidth));
    abRow.removeFromLeft(gap);
    cueButton.setBounds(abRow);
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(bgColour);
    g.setColour(textColour.withAlpha(0.2f));
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(2.0f), 5.0f, 2.0f);
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (listener && slider == &volSlider)
        listener->volumeSliderChanged(this, (float)slider->getValue());
    else if (listener && slider == &speedSlider)
    {
        listener->speedSliderChanged(this, (float)slider->getValue());
    }
    else if (listener && slider == &progressSlider)
    {
        if (slider->isMouseButtonDown())
            listener->progressSliderChanged(this, slider->getValue());
    }
}

void PlayerGUI::setMetadata(const juce::String& title, const juce::String& artist, const juce::String& duration)
{
    titleLabel.setText(title, juce::dontSendNotification);
    artistLabel.setText(artist, juce::dontSendNotification);
    durationLabel.setText(duration, juce::dontSendNotification);
}

void PlayerGUI::setDeckName(const juce::String& name)
{
    deckNameLabel.setText(name, juce::dontSendNotification);
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

void PlayerGUI::setCueMarker(double normalizedPosition)
{
    waveformDisplay.setCueMarker(normalizedPosition);
}

void PlayerGUI::setProgress(double normalizedPosition)
{
    progressSlider.setValue(normalizedPosition, juce::dontSendNotification);
}

void PlayerGUI::setTheme(juce::Colour bg, juce::Colour text, juce::Colour accent, juce::Colour playhead)
{
    bgColour = bg;
    textColour = text;
    accentColour = accent;

    waveformDisplay.setTheme(bgColour.brighter(0.2f), accentColour, juce::Colours::red.withAlpha(0.8f), playhead);

    progressSlider.setColour(juce::Slider::backgroundColourId, bgColour.brighter(0.2f));
    progressSlider.setColour(juce::Slider::trackColourId, accentColour);
    progressSlider.setColour(juce::Slider::thumbColourId, textColour);

    titleLabel.setColour(juce::Label::textColourId, textColour);
    artistLabel.setColour(juce::Label::textColourId, textColour);
    durationLabel.setColour(juce::Label::textColourId, textColour.withAlpha(0.7f));

    currentTimeLabel.setColour(juce::Label::textColourId, textColour);
    totalTimeLabel.setColour(juce::Label::textColourId, textColour);
    deckNameLabel.setColour(juce::Label::textColourId, textColour);

    volLabel.setColour(juce::Label::textColourId, textColour.withAlpha(0.7f));
    speedLabel.setColour(juce::Label::textColourId, textColour.withAlpha(0.7f));

    repaint();
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (listener == nullptr)
        return;

    if (button == &loadBtn)
        listener->loadButtonClicked(this);
    else if (button == &playPauseBtn)
        listener->playPauseButtonClicked(this);
    else if (button == &stopBtn)
        listener->stopButtonClicked(this);
    else if (button == &backBtn)
        listener->backButtonClicked(this);
    else if (button == &skipBtn)
        listener->skipButtonClicked(this);
    else if (button == &muteBtn)
        listener->muteButtonClicked(this);
    else if (button == &loopBtn)
        listener->loopButtonClicked(this);
    else if (button == &setABtn)
        listener->setAButtonClicked(this);
    else if (button == &setBBtn)
        listener->setBButtonClicked(this);
    else if (button == &abLoopToggleBtn)
        listener->abLoopToggleButtonClicked(this);
    else if (button == &setButton)
        listener->setCueButtonClicked(this);
    else if (button == &cueButton)
        listener->cueButtonClicked(this);
}