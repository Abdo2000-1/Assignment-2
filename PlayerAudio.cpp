#include "PlayerAudio.h"
#include "PlayerGUI.h"

//Last Edition

PlayerAudio::PlayerAudio()
{
    fmt.registerBasicFormats();
}

PlayerAudio::~PlayerAudio()
{
    player.setSource(nullptr);
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    currentSamplesPerBlock = samplesPerBlockExpected;
    currentSampleRate = sampleRate;
    player.prepareToPlay(samplesPerBlockExpected, sampleRate);
    if (resampler)
        resampler->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

double PlayerAudio::getCurrentPosition() const {
    return player.getCurrentPosition();
}

double PlayerAudio::getLengthInSeconds() const {
    return player.getLengthInSeconds();
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // --- Task 2: A-B Loop Logic ---
    if (abLoopEnabled && loopEndPointSeconds > loopStartPointSeconds && player.isPlaying())
    {
        if (player.getCurrentPosition() >= loopEndPointSeconds)
        {
            player.setPosition(loopStartPointSeconds);
        }
    }

    if (resampler)
        resampler->getNextAudioBlock(bufferToFill);
    else
        player.getNextAudioBlock(bufferToFill);
}

void PlayerAudio::releaseResources()
{
    if (resampler)
        resampler->releaseResources();
    player.releaseResources();
}

void PlayerAudio::loadFile(const juce::File& file)
{
    if (auto* r = fmt.createReaderFor(file))
    {
        player.stop();
        player.setSource(nullptr);
        reader.reset(new juce::AudioFormatReaderSource(r, true));
        reader->setLooping(loopEnabled);
        player.setSource(reader.get(), 0, nullptr, r->sampleRate);
        resampler.reset(new juce::ResamplingAudioSource(&player, false));
        resampler->setResamplingRatio(1.0);
        if (currentSampleRate > 0.0 && currentSamplesPerBlock > 0)
            resampler->prepareToPlay(currentSamplesPerBlock, currentSampleRate);

        // Reset A-B points on new file load
        loopStartPointSeconds = 0.0;
        loopEndPointSeconds = 0.0;
        abLoopEnabled = false;

        // store reader metadata (basic)
        metadata = r->metadataValues;
    }
}

void PlayerAudio::play()
{
    if (!player.isPlaying())
        player.start();
}

void PlayerAudio::pause()
{
    if (player.isPlaying())
        player.stop();
}

void PlayerAudio::stop()
{
    player.stop();
    player.setPosition(0); 
}

void PlayerAudio::skip(double skipSeconds) {
    double newPositionSeconds = player.getCurrentPosition();
    newPositionSeconds += skipSeconds;

    if (newPositionSeconds > player.getLengthInSeconds()) {
        newPositionSeconds = player.getLengthInSeconds();
    }
    else if (newPositionSeconds < 0) {
        newPositionSeconds = 0;
    }

    player.setPosition(newPositionSeconds);
}

void PlayerAudio::setGain(float gain)
{
    if (mutedState && gain > 0.0f) {
        mutedState = false;
    }
    player.setGain(gain);
    if (!mutedState) {
        volumeBeforeMute = gain;
    }
}

bool PlayerAudio::isPlaying() const
{
    return player.isPlaying();
}

void PlayerAudio::setSpeed(float speed)
{
    if (resampler)
        resampler->setResamplingRatio(speed);
}

void PlayerAudio::toggleMute()
{
    mutedState = !mutedState;
    if (mutedState)
    {
        if (player.getGain() > 0.0f) {
            volumeBeforeMute = player.getGain();
        }
        player.setGain(0.0f);
    }
    else
    {
        player.setGain(volumeBeforeMute);
    }
}

bool PlayerAudio::isMuted() const
{
    return mutedState;
}

void PlayerAudio::toggleLoop()
{
    loopEnabled = !loopEnabled;
    if (reader)
        reader->setLooping(loopEnabled);

    if (loopEnabled && abLoopEnabled)
    {
        abLoopEnabled = false;
    }
}

bool PlayerAudio::isLooping() const
{
    return loopEnabled;
}

void PlayerAudio::setPositionNormalized(double normPos)
{
    if (normPos < 0.0) normPos = 0.0;
    if (normPos > 1.0) normPos = 1.0;
    double newPositionSeconds = player.getLengthInSeconds() * normPos;
    player.setPosition(newPositionSeconds);
}

void PlayerAudio::setLoopA()
{
    loopStartPointSeconds = player.getCurrentPosition();
    if (loopEndPointSeconds < loopStartPointSeconds || loopEndPointSeconds == 0.0)
    {
        loopEndPointSeconds = player.getLengthInSeconds();
    }
}

void PlayerAudio::setLoopB()
{
    loopEndPointSeconds = player.getCurrentPosition();
    if (loopStartPointSeconds > loopEndPointSeconds)
    {
        loopStartPointSeconds = 0.0;
    }
}

void PlayerAudio::toggleABLoop()
{
    abLoopEnabled = !abLoopEnabled;

    if (abLoopEnabled && loopEnabled)
    {
        loopEnabled = false;
        if (reader)
            reader->setLooping(loopEnabled);
    }
}

bool PlayerAudio::isABLooping() const
{
    return abLoopEnabled;
}

juce::AudioTransportSource& PlayerAudio::getTransportSource()
{
    return player;
}