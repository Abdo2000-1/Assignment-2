#pragma once
#include <JuceHeader.h>

class PlayerAudio : public juce::AudioSource
{
public:
    PlayerAudio();
    ~PlayerAudio() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    double getCurrentPosition() const;
    double getLengthInSeconds() const;

    void loadFile(const juce::File& file);
    void play();
    void pause();
    void stop();
    void skip(double newPositionSeconds);

    void setGain(float gain);
    bool isPlaying() const;

    void toggleMute();
    bool isMuted() const;

    void toggleLoop();
    bool isLooping() const;

    juce::AudioTransportSource& getTransportSource() { return player; }

private:
    juce::AudioFormatManager fmt;
    std::unique_ptr<juce::AudioFormatReaderSource> reader;
    juce::AudioTransportSource player;

    bool mutedState{ false };

    float volumeBeforeMute{ 0.5f };

    bool loopEnabled{ false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};
