/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize(800, 600);

    formatManager.registerBasicFormats();

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted) { if (granted)  setAudioChannels(2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(0, 2);
    }

    addAndMakeVisible(deck1);
    addAndMakeVisible(deck2);

    addAndMakeVisible(playlistComponent);

    //set up the crossfader between two decks
    addAndMakeVisible(crossfader);
    crossfader.addListener(this);
    crossfader.setTooltip("CrossFader");
    crossfader.setValue(0.5);
    crossfader.setRange(0.0, 1., 0.001);
    crossfader.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::dodgerblue);
    crossfader.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::hotpink);
    crossfader.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::dodgerblue);
    crossfader.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
}

MainComponent::~MainComponent()
{
    //shut down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);

}

void MainComponent::releaseResources()
{
    mixerSource.removeAllInputs();
    mixerSource.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::whitesmoke);
    
    //draw blue triangle for track1
    juce::Path track1;
    track1.addTriangle(0., getHeight() / 2 - getHeight() / 16,
        0., getHeight() / 2,
        getWidth() / 2 - getWidth() / 16, getHeight() / 2);

    g.setColour(juce::Colours::dodgerblue);
    g.fillPath(track1);

    //draw pink triangle for track2
    juce::Path track2;
    track2.addTriangle(getWidth(), getHeight() / 2 - getHeight() / 16,
        getWidth(), getHeight() / 2,
        getWidth() / 2 + getWidth() / 16, getHeight() / 2);

    g.setColour(juce::Colours::hotpink);
    g.fillPath(track2);
}

void MainComponent::resized()
{
    double rowH = getHeight() / 2;
    double rowW = getWidth() / 2;

    deck1.setBounds(0, 0, rowW, rowH - rowH / 6);
    deck2.setBounds(rowW, 0 , rowW, rowH - rowH / 6);

    crossfader.setBounds(rowW - rowW/4, rowH - rowH/4, rowW/2, rowH / 4);
    crossFadeLabel.setBounds(rowW - rowW/8, rowH - rowH/3.7, rowW/4, rowH / 16);

    playlistComponent.setBounds(0, rowH, rowW*2, rowH);
}

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    // set up volume relationship between two players for the crossfader   
    if (slider == &crossfader) {
        double player1Gain = 1.0-crossfader.getValue();
        double player2Gain =  crossfader.getValue();
        player1.setGain(player1Gain);
        player2.setGain(player2Gain);
    }
}
