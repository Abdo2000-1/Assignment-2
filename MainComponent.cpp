#include "MainComponent.h"
#include <string> 
using namespace std;

MainComponent::MainComponent()
{
    addAndMakeVisible(gui1);
    addAndMakeVisible(gui2);
    gui1.addListener(this);
    gui2.addListener(this);
    gui1.setPlayerAudioSource(player1);
    gui2.setPlayerAudioSource(player2);
    gui1.setDeckName("Player 1");
    gui2.setDeckName("Player 2");

    mixer.addInputSource(&player1, false);
    mixer.addInputSource(&player2, false);

    addAndMakeVisible(mixModeButton);
    mixModeButton.setClickingTogglesState(true);
    mixModeButton.onClick = [this]
        {
            isInMixMode = mixModeButton.getToggleState();
            mixModeButton.setButtonText(isInMixMode ? "Mix Mode: ON" : "Mix Mode: OFF");
            if (!isInMixMode)
            {

                if (player1.isPlaying()) player2.pause();
                if (player2.isPlaying()) player1.pause();
            }
        };

    addAndMakeVisible(crossfader);
    crossfader.setSliderStyle(juce::Slider::LinearHorizontal);
    crossfader.setRange(0.0, 1.0, 0.01);
    crossfader.setValue(0.5);
    crossfader.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    crossfader.addListener(this);

    addAndMakeVisible(crossfaderLabel);
    crossfaderLabel.setText("Crossfader", juce::dontSendNotification);
    crossfaderLabel.setJustificationType(juce::Justification::centred);
    crossfaderLabel.setColour(juce::Label::textColourId, juce::Colours::lightsteelblue);

    addAndMakeVisible(deck1Label);
    deck1Label.setText("P1", juce::dontSendNotification);
    deck1Label.setJustificationType(juce::Justification::centredLeft);
    deck1Label.setColour(juce::Label::textColourId, juce::Colours::lightsteelblue);

    addAndMakeVisible(deck2Label);
    deck2Label.setText("P2", juce::dontSendNotification);
    deck2Label.setJustificationType(juce::Justification::centredRight);
    deck2Label.setColour(juce::Label::textColourId, juce::Colours::lightsteelblue);

    addAndMakeVisible(playlistBox);
    playlistBox.setModel(this);

    addAndMakeVisible(addButton);
    addButton.setButtonText("+");
    addButton.onClick = [this] { addFileToPlaylist(); };


    setSize(400, 200);


    addAndMakeVisible(deleteButton);
    deleteButton.setButtonText("Delete");
    deleteButton.onClick = [this] { deleteSelectedItem(); };

    addAndMakeVisible(clearButton);
    clearButton.setButtonText("Clear");
    clearButton.onClick = [this] { clearPlaylist(); };



    deleteButton.setEnabled(false);

    setSize(700, 800);
    setAudioChannels(0, 2);


    updateGains();
    updateLoopButtonText(gui1, player1);
    updateLoopButtonText(gui2, player2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}



void MainComponent::updateLoopButtonText(PlayerGUI& gui, PlayerAudio& player)
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
        g.fillAll(juce::Colours::cornflowerblue.darker(0.5f));
    else
        g.fillAll(juce::Colour::fromRGB(40, 50, 90));

    g.setColour(juce::Colours::lightsteelblue);
    g.drawText(playlistFiles[rowNumber], 4, 0, width - 4, height, juce::Justification::centredLeft);
}

void MainComponent::listBoxItemClicked(int row, const juce::MouseEvent&)
{

    if (row >= 0 && row < playlistFileObjects.size())
    {
        auto file = playlistFileObjects[row]; 

        player1.loadFile(file); 
        if (!isInMixMode) player2.pause();
        player1.play();

        
        auto metadata = player1.getMetadata();
        juce::String title = metadata["TITLE"];
        juce::String artist = metadata["ARTIST"];
        if (title.isEmpty()) title = file.getFileName();

        juce::String displayTitle = title;
        if (artist.isNotEmpty()) displayTitle += " - " + artist;

        gui1.setFileName(displayTitle);
        // -------------------------------

        gui1.setDeckName(file.getFileName());
        gui1.setPlayButtonText(juce::String::fromUTF8("\xE2\x8F\xB8"));
        gui2.setPlayButtonText(juce::String::fromUTF8("\xE2\x96\xB6"));

        
        gui1.loadFileForWaveform(file);
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

// ================= GUI LISTENERS =================

void MainComponent::loadButtonClicked(PlayerGUI* whichGui)
{
    chooser = std::make_unique<juce::FileChooser>(
        "Select an audio file...", juce::File{}, "*.wav;*.mp3");

    chooser->launchAsync(
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        [this, whichGui](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (!file.existsAsFile())
                return;

            PlayerAudio* playerToLoad = (whichGui == &gui1) ? &player1 : &player2;
            PlayerAudio* otherPlayer = (whichGui == &gui1) ? &player2 : &player1;
            PlayerGUI* guiToUpdate = (whichGui == &gui1) ? &gui1 : &gui2;
            PlayerGUI* otherGui = (whichGui == &gui1) ? &gui2 : &gui1;

         
            playerToLoad->loadFile(file);

            auto metadata = playerToLoad->getMetadata();
            juce::String title = metadata["TITLE"];
            juce::String artist = metadata["ARTIST"];
            if (title.isEmpty()) title = file.getFileName();

            juce::String displayTitle = title;
            if (artist.isNotEmpty()) displayTitle += " - " + artist;

            guiToUpdate->setFileName(displayTitle);
            // -------------------------------

            if (!isInMixMode) otherPlayer->pause();
            playerToLoad->play();

            guiToUpdate->setDeckName(file.getFileName());
            guiToUpdate->setPlayButtonText(juce::String::fromUTF8("\xE2\x8F\xB8"));
            if (!isInMixMode) otherGui->setPlayButtonText(juce::String::fromUTF8("\xE2\x96\xB6"));

       
            guiToUpdate->loadFileForWaveform(file); 
        });
}

void MainComponent::playPauseButtonClicked(PlayerGUI* whichGui)
{
    PlayerAudio* player = (whichGui == &gui1) ? &player1 : &player2;
    PlayerAudio* otherPlayer = (whichGui == &gui1) ? &player2 : &player1;
    PlayerGUI* gui = (whichGui == &gui1) ? &gui1 : &gui2;
    PlayerGUI* otherGui = (whichGui == &gui1) ? &gui2 : &gui1;

    if (player->isPlaying())
    {
        player->pause();
        gui->setPlayButtonText(juce::String::fromUTF8("\xE2\x96\B6"));
    }
    else
    {
        if (!isInMixMode)
        {
            otherPlayer->pause();
            otherGui->setPlayButtonText(juce::String::fromUTF8("\xE2\x96\B6"));
        }
        player->play();
        gui->setPlayButtonText(juce::String::fromUTF8("\xE2\x8F\xB8"));
    }
}

void MainComponent::stopButtonClicked(PlayerGUI* whichGui)
{
    PlayerAudio* player = (whichGui == &gui1) ? &player1 : &player2;
    PlayerGUI* gui = (whichGui == &gui1) ? &gui1 : &gui2;

    player->stop();
    gui->setPlayButtonText(juce::String::fromUTF8("\xE2\x96\B6"));
}


void MainComponent::muteButtonClicked(PlayerGUI* whichGui)
{
    PlayerAudio* player = (whichGui == &gui1) ? &player1 : &player2;
    PlayerGUI* gui = (whichGui == &gui1) ? &gui1 : &gui2;

    player->toggleMute();
    string unmute_emoji_utf8 = "\xF0\x9F\x94\x8A";
    string mute_emoji_utf8 = "\xF0\x9F\x94\x87";
    gui->setMuteButtonText(player->isMuted() ? mute_emoji_utf8 : unmute_emoji_utf8);
}

void MainComponent::skipButtonClicked(PlayerGUI* whichGui)
{
    PlayerAudio* player = (whichGui == &gui1) ? &player1 : &player2;
    player->skip(10);
}

void MainComponent::backButtonClicked(PlayerGUI* whichGui)
{
    PlayerAudio* player = (whichGui == &gui1) ? &player1 : &player2;
    player->skip(-10);
}

void MainComponent::markButtonClicked(PlayerGUI* whichGui)
{
    PlayerAudio* player = (whichGui == &gui1) ? &player1 : &player2;
    juce::uint64 uid = player->getFile()->getFileIdentifier();
    juce::String fileID = std::to_string(uid);
    markManager.addMark(fileID, player->getCurrentPosition());
    updateMarksMenu(whichGui);
    juce::ComboBox* marksMenu = whichGui->getMarksMenu();

    marksMenu->setSelectedId(0);

}

void MainComponent::deleteMarkRequested(PlayerGUI* whichGui, int selectedID) {
    PlayerAudio* player = (whichGui == &gui1) ? &player1 : &player2;
    juce::uint64 uid = player->getFile()->getFileIdentifier();
    juce::String fileID = std::to_string(uid);
    markManager.removeMark(fileID, selectedID - 1);
    updateMarksMenu(whichGui);
    juce::ComboBox* marksMenu = whichGui->getMarksMenu();
    marksMenu->setSelectedId(0);

}

void MainComponent::marksMenuChanged(PlayerGUI* whichGui)
{
    PlayerGUI* gui = (whichGui == &gui1) ? &gui1 : &gui2;
    PlayerAudio* player = (whichGui == &gui1) ? &player1 : &player2;


    juce::ComboBox* marksMenu = gui->getMarksMenu();

    juce::String selectedMark = marksMenu->getText();

    int start = selectedMark.indexOfChar('[');
    int end = selectedMark.indexOfChar(']');

    if (start >= 0 && end > start)
    {
        int length = end - start - 1;

        selectedMark = selectedMark.substring(start + 1, start + 1 + length);
    }


    juce::Array<double> marks = markManager.getMarks(std::to_string(player->getFile()->getFileIdentifier()));



    if (marksMenu->getText() != "") {
        player->getTransportSource().setPosition(selectedMark.getDoubleValue());
    }

}


void MainComponent::volumeSliderChanged(PlayerGUI* whichGui, float newValue)
{
    if (whichGui == &gui1)
    {
        player1ChannelVolume = newValue;
    }
    else if (whichGui == &gui2)
    {
        player2ChannelVolume = newValue;
    }

    updateGains();
}

void MainComponent::speedSliderChanged(PlayerGUI* whichGui, float newValue)
{
    PlayerAudio* player = (whichGui == &gui1) ? &player1 : &player2;
    player->setSpeed(newValue);
}

void MainComponent::loopButtonClicked(PlayerGUI* whichGui)
{
    PlayerAudio* player = (whichGui == &gui1) ? &player1 : &player2;
    PlayerGUI* gui = (whichGui == &gui1) ? &gui1 : &gui2;
    player->toggleLoop();
    updateLoopButtonText(*gui, *player);
}

void MainComponent::progressSliderChanged(PlayerGUI* whichGui, double newValue)
{
    PlayerAudio* player = (whichGui == &gui1) ? &player1 : &player2;
    player->setPositionNormalized(newValue);
}

void MainComponent::setAButtonClicked(PlayerGUI* whichGui)
{
    PlayerAudio* player = (whichGui == &gui1) ? &player1 : &player2;
    player->setLoopA();
}

void MainComponent::setBButtonClicked(PlayerGUI* whichGui)
{
    PlayerAudio* player = (whichGui == &gui1) ? &player1 : &player2;
    player->setLoopB();
}

void MainComponent::abLoopToggleButtonClicked(PlayerGUI* whichGui)
{
    PlayerAudio* player = (whichGui == &gui1) ? &player1 : &player2;
    PlayerGUI* gui = (whichGui == &gui1) ? &gui1 : &gui2;
    player->toggleABLoop();
    updateLoopButtonText(*gui, *player);
}





void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &crossfader)
    {
        updateGains();
    }
}


void MainComponent::updateGains()
{

    float val = crossfader.getValue();


    float crossfaderGain1 = std::cos(val * juce::MathConstants<double>::halfPi);
    float crossfaderGain2 = std::cos((1.0 - val) * juce::MathConstants<double>::halfPi);


    player1.setGain(player1ChannelVolume * crossfaderGain1);
    player2.setGain(player2ChannelVolume * crossfaderGain2);
}

// ================= AUDIO METHODS  =================

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{

    mixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{

    mixer.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{

    mixer.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
}

// ================= LAYOUT  =================

void MainComponent::paint(juce::Graphics& g)
{

    g.fillAll(juce::Colour::fromRGB(30, 35, 50));
}

void MainComponent::resized()
{
    auto area = getLocalBounds();
    int gap = 10;
    int controlHeight = 40;
    int playerHeight = 310;


    int playlistHeight = area.getHeight() - (playerHeight * 2) - controlHeight - (gap * 3);
    playlistHeight = juce::jmax(150, playlistHeight);


    if (playlistHeight == 150)
    {
        playerHeight = (area.getHeight() - controlHeight - playlistHeight - (gap * 3)) / 2;
    }

    gui1.setBounds(area.removeFromTop(playerHeight).reduced(gap, 0));
    area.removeFromTop(gap);
    gui2.setBounds(area.removeFromTop(playerHeight).reduced(gap, 0));
    area.removeFromTop(gap);


    auto controlArea = area.removeFromTop(controlHeight).reduced(gap, 0);
    mixModeButton.setBounds(controlArea.removeFromRight(120));
    controlArea.removeFromRight(gap);
    crossfaderLabel.setBounds(controlArea.removeFromTop(15));
    deck1Label.setBounds(controlArea.removeFromLeft(20));
    deck2Label.setBounds(controlArea.removeFromRight(20));
    crossfader.setBounds(controlArea);


    area.removeFromTop(gap);


    auto playlistArea = area;
    auto playlistButtons = playlistArea.removeFromTop(35).reduced(gap, 4);

    addButton.setBounds(playlistButtons.removeFromLeft(30));
    playlistButtons.removeFromLeft(gap);
    deleteButton.setBounds(playlistButtons.removeFromLeft(60));
    playlistButtons.removeFromLeft(gap);
    clearButton.setBounds(playlistButtons.removeFromLeft(60));

    playlistBox.setBounds(playlistArea.reduced(gap, 0));
    playlistBox.setRowHeight(25);

}