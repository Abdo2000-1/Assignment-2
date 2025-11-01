#pragma once
#include <JuceHeader.h>
//Last Edition


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
    void setSpeed(float speed);
    bool isPlaying() const;

    void toggleMute();
    bool isMuted() const;

    void toggleLoop();
    bool isLooping() const;

    // --- Task 1: Seeking ---
    void setPositionNormalized(double normPos);

    // --- Task 2: A-B Loop ---
    void setLoopA();
    void setLoopB();
    void toggleABLoop();
    bool isABLooping() const;

    juce::AudioTransportSource& getTransportSource() { return player; }

private:
    juce::AudioFormatManager fmt;
    std::unique_ptr<juce::AudioFormatReaderSource> reader;
    juce::AudioTransportSource player;
    std::unique_ptr<juce::ResamplingAudioSource> resampler;

    bool mutedState{ false };

    float volumeBeforeMute{ 0.5f };

    bool loopEnabled{ false };

    // --- A-B Loop State ---
    bool abLoopEnabled{ false };
    double loopStartPointSeconds{ 0.0 };
    double loopEndPointSeconds{ 0.0 };

    int currentSamplesPerBlock{ 0 };
    double currentSampleRate{ 0.0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};