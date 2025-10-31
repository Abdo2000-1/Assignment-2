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
    player.prepareToPlay(samplesPerBlockExpected, sampleRate);
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

    player.getNextAudioBlock(bufferToFill);
}

void PlayerAudio::releaseResources()
{
    player.releaseResources();
}

void PlayerAudio::loadFile(const juce::File& file)
{
    if (auto* r = fmt.createReaderFor(file))
    {
        player.stop();
        player.setSource(nullptr);
        reader.reset(new juce::AudioFormatReaderSource(r, true));
        player.setSource(reader.get(), 0, nullptr, r->sampleRate);

        // Reset A-B points on new file load
        loopStartPointSeconds = 0.0;
        loopEndPointSeconds = 0.0;
        abLoopEnabled = false;
    }
}

// ... (play, pause, stop functions remain the same) ...

void PlayerAudio::play()
{
    if (!player.isPlaying())
        player.start();
}

void PlayerAudio::pause()
{
    if (player.isPlaying())
        player.stop(); // transportSource.stop() correctly pauses without resetting position.
}

void PlayerAudio::stop()
{
    player.stop();
    player.setPosition(0); // Explicitly rewind to start.
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
    // If the user changes volume, unmute the player automatically
    if (mutedState && gain > 0.0f) {
        mutedState = false;
    }
    player.setGain(gain);
    if (!mutedState) {
        volumeBeforeMute = gain; // Store the last non-mute volume
    }
}

bool PlayerAudio::isPlaying() const
{
    return player.isPlaying();
}

// ... (Mute functions remain the same) ...
void PlayerAudio::toggleMute()
{
    mutedState = !mutedState;
    if (mutedState)
    {
        // Store current volume only if it's not already 0
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

    // A-B loop and full loop are mutually exclusive
    if (loopEnabled && abLoopEnabled)
    {
        abLoopEnabled = false;
    }
}


bool PlayerAudio::isLooping() const
{
    return loopEnabled;
}

// --- Task 1: Seeking Implementation ---
void PlayerAudio::setPositionNormalized(double normPos)
{
    if (normPos < 0.0) normPos = 0.0;
    if (normPos > 1.0) normPos = 1.0;
    double newPositionSeconds = player.getLengthInSeconds() * normPos;
    player.setPosition(newPositionSeconds);
}

// --- Task 2: A-B Loop Implementation ---
void PlayerAudio::setLoopA()
{
    loopStartPointSeconds = player.getCurrentPosition();
    // If A is set after B, or B is 0, B should be the end
    if (loopEndPointSeconds < loopStartPointSeconds || loopEndPointSeconds == 0.0)
    {
        loopEndPointSeconds = player.getLengthInSeconds();
    }
}

void PlayerAudio::setLoopB()
{
    loopEndPointSeconds = player.getCurrentPosition();
    // If B is set before A, A should be the start
    if (loopStartPointSeconds > loopEndPointSeconds)
    {
        loopStartPointSeconds = 0.0;
    }
}

void PlayerAudio::toggleABLoop()
{
    abLoopEnabled = !abLoopEnabled;

    // A-B loop and full loop are mutually exclusive
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