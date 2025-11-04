#pragma once
#include <JuceHeader.h>

class PlaylistComponent : public juce::Component,
    public juce::TableListBoxModel,
    public juce::Button::Listener
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;
    void paintRowBackground(juce::Graphics&, int, int, int, bool) override;
    void paintCell(juce::Graphics&, int, int, int, int, bool) override;

    void buttonClicked(juce::Button* button) override;

    juce::File getFileAt(int index) const;

private:
    juce::TableListBox playlistBox;
    juce::TextButton addButton{ "Add" };
    juce::TextButton removeButton{ "Delete" };
    juce::TextButton clearButton{ " Clear" };

    juce::StringArray playlistFiles;
    juce::Array<juce::File> playlistFileObjects;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
