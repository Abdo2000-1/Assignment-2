#include "PlayerGUI.h"
#include "PlayerAudio.h"

//Last Edition


juce::String PlayerGUI::formatTime(double seconds)
{
    int totalSecs = static_cast<int>(std::round(seconds));
    int mins = totalSecs / 60;
    int secs = totalSecs % 60;
    return juce::String::formatted("%02d:%02d", mins, secs);
}
// --------------------------------------------------------

PlayerGUI::PlayerGUI()
{
  
    juce::Colour backgroundColour = juce::Colour::fromRGB(40, 50, 90);
    juce::Colour trackColour = juce::Colours::cornflowerblue;
    juce::Colour thumbColour = juce::Colours::white;
    juce::Colour textColour = juce::Colours::lightsteelblue;


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

    // ---------------------------------

    addAndMakeVisible(currentTimeLabel);
    addAndMakeVisible(totalTimeLabel);
    addAndMakeVisible(deckNameLabel);
    // ---------------------------------

    volSlider.setRange(0.0, 1.0, 0.01);
    volSlider.setValue(0.5);
    volSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    speedSlider.setRange(0.5, 2.0, 0.01);
    speedSlider.setValue(1.0);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    progressSlider.setRange(0.0, 1.0);
    progressSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    progressSlider.setSliderStyle(juce::Slider::LinearBar);


    progressSlider.setColour(juce::Slider::backgroundColourId, backgroundColour.brighter(0.2f));
    progressSlider.setColour(juce::Slider::trackColourId, trackColour);
    progressSlider.setColour(juce::Slider::thumbColourId, thumbColour);
    // ------------------------------------

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

    fileLabel.setColour(juce::Label::textColourId, textColour);
    fileLabel.setText("No file loaded", juce::dontSendNotification);

    speedLabel.setColour(juce::Label::textColourId, textColour.withAlpha(0.7f));
    speedLabel.setText("1.00x", juce::dontSendNotification);
    speedLabel.setJustificationType(juce::Justification::centred);


    currentTimeLabel.setColour(juce::Label::textColourId, textColour);
    currentTimeLabel.setText("00:00", juce::dontSendNotification);
    currentTimeLabel.setJustificationType(juce::Justification::centredLeft);

    totalTimeLabel.setColour(juce::Label::textColourId, textColour);
    totalTimeLabel.setText("00:00", juce::dontSendNotification);
    totalTimeLabel.setJustificationType(juce::Justification::centredRight);

    deckNameLabel.setColour(juce::Label::textColourId, textColour);
    deckNameLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    deckNameLabel.setJustificationType(juce::Justification::centred);
    // ---------------------------------
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
        {
            if (!progressSlider.isMouseButtonDown())
            {
                progressSlider.setValue(currentTime / totalLength, juce::dontSendNotification);
            }
          
            currentTimeLabel.setText(formatTime(currentTime), juce::dontSendNotification);
            totalTimeLabel.setText(formatTime(totalLength), juce::dontSendNotification);
            // ---------------------------
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
    float rowHeight = bounds.getHeight() * 0.15f; 
    rowHeight = juce::jlimit(25.0f, 40.0f, rowHeight); 

    
    deckNameLabel.setBounds(bounds.removeFromTop(rowHeight * 0.8f));
    bounds.removeFromTop(gap);

  
    auto topRow = bounds.removeFromTop(rowHeight);
    loadBtn.setBounds(topRow.removeFromLeft(topRow.getWidth() * 0.2f));
    topRow.removeFromLeft(gap);
    fileLabel.setBounds(topRow);

    bounds.removeFromTop(gap);

  
    auto progressRow = bounds.removeFromTop(rowHeight * 0.8f);
    currentTimeLabel.setBounds(progressRow.removeFromLeft(timeLabelWidth));
    totalTimeLabel.setBounds(progressRow.removeFromRight(timeLabelWidth));
    progressRow.reduce(tinyGap, 0);
    progressSlider.setBounds(progressRow);

    bounds.removeFromTop(gap);

    
    auto transportRow = bounds.removeFromTop(rowHeight * 1.2f);
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

   
    auto abRow = bounds.removeFromTop(rowHeight);
    btnCount = 3;
    btnWidth = (abRow.getWidth() - (gap * (btnCount - 1))) / (float)btnCount;
    setABtn.setBounds(abRow.removeFromLeft(btnWidth));
    abRow.removeFromLeft(gap);
    setBBtn.setBounds(abRow.removeFromLeft(btnWidth));
    abRow.removeFromLeft(gap);
    abLoopToggleBtn.setBounds(abRow);

    bounds.removeFromTop(gap);

   
    auto sliderRow = bounds;
    auto volArea = sliderRow.removeFromLeft(sliderRow.getWidth() * 0.48f);
    auto speedArea = sliderRow.removeFromRight(sliderRow.getWidth() * 0.96f); // 0.48 / (1 - 0.48)

    muteBtn.setBounds(volArea.removeFromRight(volArea.getWidth() * 0.2f));
    volArea.removeFromRight(tinyGap);
    volSlider.setBounds(volArea);

    speedLabel.setBounds(speedArea.removeFromRight(speedArea.getWidth() * 0.2f));
    speedArea.removeFromRight(tinyGap);
    speedSlider.setBounds(speedArea);
}

void PlayerGUI::paint(juce::Graphics& g)
{
   
    g.fillAll(juce::Colour::fromRGB(40, 50, 90));
  
    g.setColour(juce::Colours::lightsteelblue.withAlpha(0.2f));
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(2.0f), 5.0f, 2.0f);
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (listener && slider == &volSlider)
        listener->volumeSliderChanged(this, (float)slider->getValue()); 
    else if (listener && slider == &speedSlider)
    {
        speedLabel.setText(juce::String(slider->getValue(), 2) + "x", juce::dontSendNotification);
        listener->speedSliderChanged(this, (float)slider->getValue()); 
    }
    else if (listener && slider == &progressSlider)
    {
        if (slider->isMouseButtonDown())
            listener->progressSliderChanged(this, slider->getValue()); 
    }
}

void PlayerGUI::setFileName(const juce::String& name)
{
    fileLabel.setText(name, juce::dontSendNotification);
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
}