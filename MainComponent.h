#pragma once
#include <JuceHeader.h>
#include "picosha2.h"
#include "PlayerAudio.h" 
#include "PlayerGUI.h"

class MainComponent : public juce::AudioAppComponent, public PlayerGUI::Listener
{
public:
    MainComponent();
    ~MainComponent() override;


    void setFile(juce::File m_file) ;
    juce::File getFile() ;
    PlayerAudio& getPlayerAudio() ;
    PlayerGUI* getPlayerGUI() ;

    void prepareToPlay(int, double) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& info) override;
    void releaseResources() override;
    void paint(juce::Graphics&) override;
    void resized() override;

    void loadButtonClicked() override;
    void playPauseButtonClicked() override;
    void stopButtonClicked() override;
    void muteButtonClicked() override;
    void skipButtonClicked() override;
    void backButtonClicked() override;

    juce::String createFileId();

    void volumeSliderChanged(float newValue) override;
private:
    PlayerAudio player;
    PlayerGUI gui;
    juce::File file;
    std::unique_ptr<juce::FileChooser> chooser;

    juce::String sha256First5KB()
    {
        constexpr size_t bytesToRead = 5 * 1024;
        juce::FileInputStream stream(file);
        if (!stream.openedOk())
            return {};

        juce::MemoryBlock buffer;
        buffer.setSize(bytesToRead);

        auto bytesRead = stream.read(buffer.getData(), bytesToRead);

        
        // Convert JUCE MemoryBlock into std::vector<unsigned char>
        std::vector<unsigned char> data(
            static_cast<unsigned char*>(buffer.getData()),
            static_cast<unsigned char*>(buffer.getData()) + bytesRead
        );

        // Compute the SHA-256 hash of the first 5 KB
        std::string hash = picosha2::hash256_hex_string(data);

        return juce::String(hash);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};