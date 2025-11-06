#include "PlaylistComponent.h"

PlaylistComponent::PlaylistComponent()
{
    addAndMakeVisible(playlistBox);
    addAndMakeVisible(addButton);
    addAndMakeVisible(removeButton);
    addAndMakeVisible(clearButton);

    playlistBox.getHeader().addColumn("Track", 1, 400);
    playlistBox.setModel(this);

    addButton.addListener(this);
    removeButton.addListener(this);
    clearButton.addListener(this);
}

void PlaylistComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkslategrey);
}

void PlaylistComponent::resized()
{
    auto area = getLocalBounds().reduced(8);
    auto buttonArea = area.removeFromTop(35);

    addButton.setBounds(buttonArea.removeFromLeft(80).reduced(2));
    removeButton.setBounds(buttonArea.removeFromLeft(100).reduced(2));
    clearButton.setBounds(buttonArea.removeFromLeft(100).reduced(2));

    playlistBox.setBounds(area);
}

int PlaylistComponent::getNumRows()
{
    return playlistFiles.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::green);
    else if (rowNumber % 2 == 0)
        g.fillAll(juce::Colours::darkgrey);
    else
        g.fillAll(juce::Colours::grey);
}

void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool)
{
    if (rowNumber < playlistFiles.size())
    {
        g.setColour(juce::Colours::white);
        g.drawText(playlistFiles[rowNumber],
            5, 0, width - 10, height,
            juce::Justification::centredLeft,
            true);
    }
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button == &addButton)
    {
        auto chooser = std::make_unique<juce::FileChooser>(
            "Select audio files...",
            juce::File{},
            "*.mp3;*.wav;*.flac;*.ogg");

        chooser->launchAsync(
            juce::FileBrowserComponent::openMode |
            juce::FileBrowserComponent::canSelectFiles |
            juce::FileBrowserComponent::canSelectMultipleItems,
            [this](const juce::FileChooser& fc)
            {
                auto files = fc.getResults();
                for (auto& file : files)
                {
                    playlistFiles.add(file.getFileName());
                    playlistFileObjects.add(file);
                }
                playlistBox.updateContent();
            });
    }
    else if (button == &removeButton)
    {
        int selectedRow = playlistBox.getSelectedRow();
        if (juce::isPositiveAndBelow(selectedRow, playlistFiles.size()))
        {
            playlistFiles.remove(selectedRow);
            playlistFileObjects.remove(selectedRow);
            playlistBox.updateContent();
        }
    }
    else if (button == &clearButton)
    {
        playlistFiles.clear();
        playlistFileObjects.clear();
        playlistBox.updateContent();
    }
}

juce::File PlaylistComponent::getFileAt(int index) const
{
    if (juce::isPositiveAndBelow(index, playlistFileObjects.size()))
        return playlistFileObjects[index];
    return {};
}