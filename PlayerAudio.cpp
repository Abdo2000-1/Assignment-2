#include "PlayerAudio.h"
#include "PlayerGUI.h"

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
    }
}

// --- Logic adjustment clarification ---
// The logic for play, pause, and stop is correct as it follows standard player behavior.
// play() resumes or starts from the beginning if stopped.
// pause() stops at the current position.
// stop() stops and rewinds to the beginning.

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

// --- Mute Functionality Implementation (NEW) ---
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
}


bool PlayerAudio::isLooping() const
{
    return loopEnabled;
}