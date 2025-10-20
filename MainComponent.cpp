#include "MainComponent.h"

using namespace std; 

MainComponent::MainComponent()
{
    addAndMakeVisible(gui);
    gui.addListener(this);

    setSize(520, 260);
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::loadButtonClicked()
{
    chooser = make_unique<juce::FileChooser>("Select audio file...", juce::File{}, "*.wav;*.mp3");
    chooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& fc)
        {
            auto f = fc.getResult();
            if (f.existsAsFile())
            {
                player.loadFile(f);
                player.play();
                gui.setFileName(f.getFileName());
                string pause_button_utf8 = "\xE2\x8F\xB8";
                gui.setPlayButtonText(pause_button_utf8);
            }
        });
}

void MainComponent::playPauseButtonClicked()
{
    if (player.isPlaying())
    {
        player.pause();
        string play_button_utf8 = "\xE2\x96\xB6";


        gui.setPlayButtonText(play_button_utf8);
    }
    else
    {
        player.play();

        string pause_button_utf8 = "\xE2\x8F\xB8";
        gui.setPlayButtonText(pause_button_utf8);
    }
}

void MainComponent::stopButtonClicked()
{
    player.stop();

    string play_button_utf8 = "\xE2\x96\xB6"; 
  

    gui.setPlayButtonText(play_button_utf8);
}

void MainComponent::muteButtonClicked()
{
    player.toggleMute();
    string unmute_emoji_utf8 = "\xF0\x9F\x94\x8A";
    string mute_emoji_utf8 = "\xF0\x9F\x94\x87";
    gui.setMuteButtonText(player.isMuted() ?  mute_emoji_utf8 : unmute_emoji_utf8);
}

void MainComponent::volumeSliderChanged(float newValue)
{
    player.setGain(newValue);
}


void MainComponent::prepareToPlay(int samples, double rate) { player.prepareToPlay(samples, rate); }
void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& info) { player.getNextAudioBlock(info); }
void MainComponent::releaseResources() { player.releaseResources(); }
void MainComponent::paint(juce::Graphics& g) { g.fillAll(juce::Colour::fromRGB(40, 70, 50)); }
void MainComponent::resized() { gui.setBounds(getLocalBounds()); }