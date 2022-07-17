/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

class MainComponent : public juce::AudioAppComponent,
                      public juce::Slider::Listener
    
{
public:

    MainComponent();

    ~MainComponent();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void releaseResources() override;

    void paint(juce::Graphics& g) override;

    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;

private:
    
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbCache{ 200 };

    DJAudioPlayer player1{ &formatManager };
    DJAudioPlayer player2{ &formatManager };

    DeckGUI deck1{ &player1, &formatManager, thumbCache, &playlistComponent };
    DeckGUI deck2{ &player2, &formatManager, thumbCache, &playlistComponent };

    juce::MixerAudioSource mixerSource;

    juce::Slider crossfader;
    juce::Label  crossFadeLabel;

    PlaylistComponent playlistComponent{ &deck1, &deck2 };

    juce::TooltipWindow tooltipWindow{ this, 500 };

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
