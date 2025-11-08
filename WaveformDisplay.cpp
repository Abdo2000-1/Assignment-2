#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManager)
    : thumbnailCache(5),
    thumbnail(512, formatManager, thumbnailCache)
{
    thumbnail.addChangeListener(this);
    startTimer(30);

   
    setTheme(juce::Colour::fromRGB(40, 50, 90).brighter(0.2f), 
        juce::Colours::cornflowerblue,                    // wave
        juce::Colours::red.withAlpha(0.8f),               // cue
        juce::Colours::white.withAlpha(0.7f));            // playhead
   
}

WaveformDisplay::~WaveformDisplay()
{
    stopTimer();
    thumbnail.removeChangeListener(this);
}

void WaveformDisplay::loadFile(const juce::File& audioFile)
{
    clearCueMarker();
    thumbnail.setSource(new juce::FileInputSource(audioFile));
}

void WaveformDisplay::setTransportSource(juce::AudioTransportSource* transport)
{
    transportSource = transport;
}

void WaveformDisplay::paint(juce::Graphics& g)
{
   
    g.fillAll(bgColour);

    
    g.setColour(waveColour);
    thumbnail.drawChannels(g, getLocalBounds(), 0.0, thumbnail.getTotalLength(), 1.0f);

   
    if (cuePointPosition > 0.0)
    {
        g.setColour(cueColour);
        auto xPos = cuePointPosition * getWidth();
        g.drawRect(xPos - 1, 0.0f, 2.0f, (float)getHeight());
    }

   
    if (transportSource != nullptr && transportSource->getLengthInSeconds() > 0)
    {
        g.setColour(playheadColour);
        auto xPosition = playheadPosition * getWidth();
        g.drawRect(xPosition, 0.0f, 1.5f, (float)getHeight());
    }
   
}

void WaveformDisplay::resized()
{
   
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &thumbnail)
    {
        repaint();
    }
}

void WaveformDisplay::timerCallback()
{
    if (transportSource != nullptr && transportSource->getLengthInSeconds() > 0)
    {
        double newPos = transportSource->getCurrentPosition() / transportSource->getLengthInSeconds();
        if (newPos != playheadPosition)
        {
            playheadPosition = newPos;
            repaint();
        }
    }
}

void WaveformDisplay::mouseDown(const juce::MouseEvent& e)
{
    if (onPositionChange)
    {
        double newPos = (double)e.getPosition().getX() / getWidth();
        onPositionChange(newPos);
    }
}

void WaveformDisplay::mouseDrag(const juce::MouseEvent& e)
{
    if (onPositionChange)
    {
        double newPos = (double)e.getPosition().getX() / getWidth();
        newPos = juce::jlimit(0.0, 1.0, newPos);
        onPositionChange(newPos);
    }
}

void WaveformDisplay::setCueMarker(double normalizedPosition)
{
    cuePointPosition = normalizedPosition;
    repaint();
}

void WaveformDisplay::clearCueMarker()
{
    if (cuePointPosition != 0.0)
    {
        cuePointPosition = 0.0;
        repaint();
    }
}


void WaveformDisplay::setTheme(juce::Colour background, juce::Colour waveform, juce::Colour cueMarker, juce::Colour playhead)
{
    bgColour = background;
    waveColour = waveform;
    cueColour = cueMarker;
    playheadColour = playhead;

    repaint();
}
