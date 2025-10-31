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
    addAndMakeVisible(progressSlider);
    addAndMakeVisible(fileLabel);

    // --- Task 2: Add new buttons ---
    addAndMakeVisible(setABtn);
    addAndMakeVisible(setBBtn);
    addAndMakeVisible(abLoopToggleBtn);
    abLoopToggleBtn.setButtonText("A-B");

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

    // --- Task 1: Listen to progress slider ---
    progressSlider.addListener(this);

    // --- Task 2: Listen to new buttons ---
    setABtn.addListener(this);
    setBBtn.addListener(this);
    abLoopToggleBtn.addListener(this);

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
        // --- Task 1: Don't update slider if user is dragging it ---
        if (playerAudioSource != nullptr && !progressSlider.isMouseButtonDown())
        {
            double currentTime = playerAudioSource->getCurrentPosition();
            double totalLength = playerAudioSource->getLengthInSeconds();

            if (totalLength > 0.0)
                // Use dontSendNotification to prevent recursive loop with sliderValueChanged
                progressSlider.setValue(currentTime / totalLength, juce::dontSendNotification);
        }
    }
}

void PlayerGUI::addListener(Listener* listenerToAdd) { listener = listenerToAdd; }

void PlayerGUI::resized()
{
    // --- Re-designed layout to fit new buttons and center controls ---
    auto bounds = getLocalBounds();
    int side = 50, gap = 10;

    // Progress Bar
    progressSlider.setBounds(50, bounds.getBottom() - 100, bounds.getWidth() - 100, 10);

    // Row 1: Transport Controls (Above progress bar)
    int y_transport = progressSlider.getY() - side - gap;
    // 7 buttons: A, B, AB-Toggle, Back, Play, Stop, Skip
    int transportWidth = (side * 7) + (gap * 6);
    int x_transport = (bounds.getWidth() - transportWidth) / 2;

    setABtn.setBounds(x_transport, y_transport, side, side);
    setBBtn.setBounds(setABtn.getRight() + gap, y_transport, side, side);
    abLoopToggleBtn.setBounds(setBBtn.getRight() + gap, y_transport, side, side);
    backBtn.setBounds(abLoopToggleBtn.getRight() + gap, y_transport, side, side);
    playPauseBtn.setBounds(backBtn.getRight() + gap, y_transport, side, side);
    stopBtn.setBounds(playPauseBtn.getRight() + gap, y_transport, side, side);
    skipBtn.setBounds(stopBtn.getRight() + gap, y_transport, side, side);

    // Row 2: Volume / Other Controls (Below progress bar)
    int y_controls = progressSlider.getBottom() + 20;
    int volWidth = 150, btnWidth = 50, loopWidth = 80;
    // 3 controls: Vol, Mute, Loop
    int controlsWidth = volWidth + btnWidth + loopWidth + (gap * 2);
    int x_controls = (bounds.getWidth() - controlsWidth) / 2;

    volSlider.setBounds(x_controls, y_controls, volWidth, 30);
    muteBtn.setBounds(volSlider.getRight() + gap, y_controls, btnWidth, 30);
    loopBtn.setBounds(muteBtn.getRight() + gap, y_controls, loopWidth, 30);

    // File Label & Load Button
    loadBtn.setBounds(20, 10, 60, 30);
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
    // --- Task 2: Handle new button clicks ---
    else if (button == &setABtn) listener->setAButtonClicked();
    else if (button == &setBBtn) listener->setBButtonClicked();
    else if (button == &abLoopToggleBtn) listener->abLoopToggleButtonClicked();
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (listener && slider == &volSlider)
        listener->volumeSliderChanged((float)slider->getValue());
    // --- Task 1: Handle progress slider drag ---
    else if (listener && slider == &progressSlider)
    {
        // Only update position if the user is the one moving the slider
        if (slider->isMouseButtonDown())
        {
            listener->progressSliderChanged(slider->getValue());
        }
    }
}

void PlayerGUI::setFileName(const juce::String& name) { fileLabel.setText(name, juce::dontSendNotification); }
void PlayerGUI::setMuteButtonText(const juce::String& text) { muteBtn.setButtonText(text); }
void PlayerGUI::setPlayButtonText(const juce::String& text) { playPauseBtn.setButtonText(text); }
void PlayerGUI::setLoopButtonText(const juce::String& text) { loopBtn.setButtonText(text); }

// --- Task 2: Implement new setter ---
void PlayerGUI::setABLoopButtonText(const juce::String& text) { abLoopToggleBtn.setButtonText(text); }