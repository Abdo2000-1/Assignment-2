#include "MainComponent.h"

#include <taglib/fileref.h>
#include <taglib/tag.h>

using namespace std;

MainComponent::MainComponent()
{
    addAndMakeVisible(gui);
    gui.addListener(this);

    // Playlist UI
    addAndMakeVisible(playlistBox);
    playlistBox.setModel(this);

    addAndMakeVisible(addButton);
    addButton.setButtonText("+");
    addButton.onClick = [this] { addFileToPlaylist(); };

    addAndMakeVisible(deleteButton);
    deleteButton.setButtonText("Delete");
    deleteButton.onClick = [this] { deleteSelectedItem(); };

    addAndMakeVisible(clearButton);
    clearButton.setButtonText("Clear");
    clearButton.onClick = [this] { clearPlaylist(); };

    // Disable delete button initially
    deleteButton.setEnabled(false);

    setSize(520, 420);
    setAudioChannels(0, 2);

    gui.setPlayerAudioSource(player);
    updateLoopButtonText();
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::updateLoopButtonText()
{
    juce::String Loop = juce::String::fromUTF8("\xF0\x9F\x94\x81");
    juce::String Unloop = juce::String::fromUTF8("\xF0\x9F\x94\x80");
    if (player.isLooping())
        gui.setLoopButtonText(Loop);
    else
        gui.setLoopButtonText(Unloop);

    if (player.isABLooping())
        gui.setABLoopButtonText("A-B On");
    else
        gui.setABLoopButtonText("A-B");
}

// ================= PLAYLIST METHODS =================

int MainComponent::getNumRows()
{
    return playlistFiles.size();
}

void MainComponent::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    if (rowNumber < 0 || rowNumber >= playlistFiles.size())
        return;

    if (rowIsSelected)
        g.fillAll(juce::Colours::darkgreen);
    g.setColour(juce::Colours::white);
    g.drawText(playlistFiles[rowNumber], 4, 0, width - 4, height, juce::Justification::centredLeft);
}

void MainComponent::listBoxItemClicked(int row, const juce::MouseEvent&)
{
    if (row >= 0 && row < playlistFileObjects.size())
    {
        player.loadFile(playlistFileObjects[row]);
        player.play();

        gui.setFileName("Now Playing: " + playlistFiles[row]);
        gui.setPlayButtonText(juce::String::fromUTF8("\xE2\x8F\xB8"));
    }

    deleteButton.setEnabled(row >= 0);
}

void MainComponent::addFileToPlaylist()
{
    chooser = std::make_unique<juce::FileChooser>(
        "Add an audio file...", juce::File{}, "*.wav;*.mp3");

    chooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (!file.existsAsFile())
                return;

            playlistFiles.add(file.getFileName());
            playlistFileObjects.add(file);
            playlistBox.updateContent();

            playlistBox.selectRow(playlistFiles.size() - 1);
            deleteButton.setEnabled(true);
        });
}

void MainComponent::deleteSelectedItem()
{
    int selected = playlistBox.getSelectedRow();
    if (selected >= 0 && selected < playlistFiles.size())
    {
        playlistFiles.remove(selected);
        playlistFileObjects.remove(selected);
        playlistBox.updateContent();
    }

    deleteButton.setEnabled(playlistBox.getSelectedRow() >= 0);
}

void MainComponent::clearPlaylist()
{
    playlistFiles.clear();
    playlistFileObjects.clear();
    playlistBox.updateContent();

    deleteButton.setEnabled(false);
}


void MainComponent::loadButtonClicked()
{
    chooser = std::make_unique<juce::FileChooser>(
        "Select an audio file...", juce::File{}, "*.wav;*.mp3");

    chooser->launchAsync(
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (!file.existsAsFile())
                return;

            juce::String metadataText = "Loaded: " + file.getFileName() + "\n";

            TagLib::FileRef f(file.getFullPathName().toRawUTF8());
            if (!f.isNull() && f.tag())
            {
                TagLib::Tag* tag = f.tag();
                metadataText += "Title: " + juce::String(tag->title().toCString(true)) + "\n";
                metadataText += "Artist: " + juce::String(tag->artist().toCString(true)) + "\n";
                metadataText += "Album: " + juce::String(tag->album().toCString(true)) + "\n";
                metadataText += "Year: " + juce::String(tag->year()) + "\n";
                metadataText += "Comment: " + juce::String(tag->comment().toCString(true)) + "\n";
            }
            else
            {
                metadataText += "No metadata found.\n";
            }

            player.loadFile(file);
            player.play();

            gui.setFileName(metadataText);
            gui.setPlayButtonText(juce::String::fromUTF8("\xE2\x8F\xB8"));
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

void MainComponent::speedSliderChanged(float newValue)
{
    player.setSpeed(newValue);
}

void MainComponent::loopButtonClicked()
{
    player.toggleLoop();
    updateLoopButtonText();
}

void MainComponent::progressSliderChanged(double newValue)
{
    player.setPositionNormalized(newValue);
}

void MainComponent::setAButtonClicked()
{
    player.setLoopA();
}

void MainComponent::setBButtonClicked()
{
    player.setLoopB();
}

void MainComponent::abLoopToggleButtonClicked()
{
    player.toggleABLoop();
    updateLoopButtonText();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    player.getNextAudioBlock(bufferToFill);
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
    auto area = getLocalBounds();
    auto guiArea = area.removeFromTop(260);
    gui.setBounds(guiArea);

    auto playlistArea = area.removeFromTop(getHeight() - 260);
    addButton.setBounds(10, playlistArea.getY() + 5, 30, 25);
    deleteButton.setBounds(50, playlistArea.getY() + 5, 60, 25);
    clearButton.setBounds(120, playlistArea.getY() + 5, 60, 25);

    playlistBox.setBounds(10, playlistArea.getY() + 35,
        playlistArea.getWidth() - 20, playlistArea.getHeight() - 45);
}
