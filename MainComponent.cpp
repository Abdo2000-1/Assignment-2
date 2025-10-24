#include "MainComponent.h"

using namespace std;//Last Edition


MainComponent::MainComponent()
{
    addAndMakeVisible(gui);
    gui.addListener(this);

    setSize(520, 260);
    setAudioChannels(0, 2);

    gui.setPlayerAudioSource(player);
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
                gui.setPlayButtonText(juce::String::fromUTF8("\xE2\x8F\xB8"));
            }
        });
}

void MainComponent::playPauseButtonClicked()
{
    if (player.isPlaying())
    {
        player.pause();
        gui.setPlayButtonText(juce::String::fromUTF8("\xE2\x96\xB6"));
    }
    else
    {
        player.play();
        gui.setPlayButtonText(juce::String::fromUTF8("\xE2\x8F\xB8"));
    }
}

void MainComponent::stopButtonClicked()
{
    player.stop();
    gui.setPlayButtonText(juce::String::fromUTF8("\xE2\x96\xB6"));

}

void MainComponent::muteButtonClicked()
{
    player.toggleMute();
    string unmute_emoji_utf8 = "\xF0\x9F\x94\x8A";
    string mute_emoji_utf8 = "\xF0\x9F\x94\x87";
    gui.setMuteButtonText(player.isMuted() ? mute_emoji_utf8 : unmute_emoji_utf8);
}

void MainComponent::skipButtonClicked()
{
    player.skip(10);
}

void MainComponent::backButtonClicked()
{
    player.skip(-10);
}

void MainComponent::volumeSliderChanged(float newValue)
{
    player.setGain(newValue);
}

void MainComponent::loopButtonClicked()
{
    player.toggleLoop();      


    string Loop = "\xF0\x9F\x94\x81";
    string Unloop = "\xF0\x9F\x94\x80";
    if (player.isLooping())
        gui.setLoopButtonText(Loop);
    else
        gui.setLoopButtonText(Unloop);
}

void MainComponent::prepareToPlay(int samples, double rate)
{
    player.prepareToPlay(samples, rate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& info)
{
    player.getNextAudioBlock(info);
}

void MainComponent::releaseResources()
{
    player.releaseResources();
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(40, 70, 50));
}

void MainComponent::resized()
{
    gui.setBounds(getLocalBounds());
}
