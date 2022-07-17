
#include <JuceHeader.h>
#include "DeckGUI.h"
#include "PlaylistComponent.h"

DeckGUI::DeckGUI(DJAudioPlayer* _player, juce::AudioFormatManager* formatManagerToUse,juce::AudioThumbnailCache& cacheToUse,PlaylistComponent* _playlistComponent)
	: player(_player), 
	waveformDisplay(formatManagerToUse, cacheToUse, _player),
	playlistComponent(_playlistComponent),
	trackDatas(playlistComponent->getTrackDatas())
{
	addAndMakeVisible(waveformDisplay);

	//set up posSlider to show the current position
	addAndMakeVisible(posSlider);
	posSlider.addListener(this);
	posSlider.setRange(0.0, 1., 0.001);
	posSlider.setColour(juce::Slider::ColourIds::trackColourId,juce::Colours::pink);
	posSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::dodgerblue);
	posSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

	//set up the Rotary speedSlider to control the current speed
	addAndMakeVisible(speedLabel);
    speedLabel.setText("speed", juce::dontSendNotification);
	speedLabel.setJustificationType(juce::Justification::centred);
	speedLabel.setColour(juce::Label::textColourId, juce::Colours::dodgerblue);
	speedLabel.setColour(juce::Label::backgroundColourId, juce::Colours::white);
	addAndMakeVisible(speedSlider);
	speedSlider.addListener(this);
	speedSlider.setRange(0.0, 2.0);
	speedSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	speedSlider.setValue(1.0);
	speedSlider.setSize(getWidth() / 6, getWidth() / 6);
	speedSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::hotpink);
	speedSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

	//set up the Rotary volSlider to control the current volume
	addAndMakeVisible(volLabel);
	volLabel.setText("volume", juce::dontSendNotification);
	volLabel.setJustificationType(juce::Justification::centred);
	volLabel.setColour(juce::Label::textColourId, juce::Colours::dodgerblue);
	volLabel.setColour(juce::Label::backgroundColourId, juce::Colours::white);	
	addAndMakeVisible(volSlider);
	volSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	volSlider.setValue(0.5);
	volSlider.addListener(this);
	volSlider.setRange(0.0, 1.0);
	volSlider.setSize(getWidth() /6, getWidth() /6);
	volSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::hotpink);
	volSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

	//set up the images for myPlayButton
	juce::Image playnormalButton = juce::ImageCache::getFromMemory(BinaryData::play_png, BinaryData::play_pngSize);
	juce::Image playoverButton = juce::ImageCache::getFromMemory(BinaryData::playover_png, BinaryData::playover_pngSize);
	juce::Image playdownButton = juce::ImageCache::getFromMemory(BinaryData::playdown_png, BinaryData::playdown_pngSize);
	myPlayButton.setImages(true, true, true, playnormalButton, 1.0f, {}, playoverButton, 1.0f, {}, playdownButton, 1.0f, {});
	addAndMakeVisible(myPlayButton);
	myPlayButton.addListener(this);
	myPlayButton.setClickingTogglesState(true);

	//set up the images for myStopButton
	juce::Image stopnormalButton = juce::ImageCache::getFromMemory(BinaryData::stop_png, BinaryData::stop_pngSize);
	juce::Image stopoverButton = juce::ImageCache::getFromMemory(BinaryData::stopover_png, BinaryData::stopover_pngSize);
	juce::Image stopdownButton = juce::ImageCache::getFromMemory(BinaryData::stopdown_png, BinaryData::stopdown_pngSize);
	myStopButton.setImages(true, true, true, stopnormalButton, 1.0f, {}, stopoverButton, 1.0f, {}, stopdownButton, 1.0f, {});
	addAndMakeVisible(myStopButton);
	myStopButton.addListener(this);

	//set up the images for myLoadButton
	juce::Image loadnormalButton = juce::ImageCache::getFromMemory(BinaryData::load_png, BinaryData::load_pngSize);
	juce::Image loadoverButton = juce::ImageCache::getFromMemory(BinaryData::loadover_png, BinaryData::loadover_pngSize);
	juce::Image loaddownButton = juce::ImageCache::getFromMemory(BinaryData::loaddown_png, BinaryData::loaddown_pngSize);
	myLoadButton.setImages(true, true, true, loadnormalButton, 1.0f, {}, loadoverButton, 1.0f, {}, loaddownButton, 1.0f, {});
	addAndMakeVisible(myLoadButton);
	myLoadButton.addListener(this);

	//set up the images for myLoopButton
	juce::Image loopnormalButton = juce::ImageCache::getFromMemory(BinaryData::loop_png, BinaryData::loop_pngSize);
	juce::Image loopoverButton = juce::ImageCache::getFromMemory(BinaryData::loopover_png, BinaryData::loopover_pngSize);
	juce::Image loopdownButton = juce::ImageCache::getFromMemory(BinaryData::loopdown_png, BinaryData::loopdown_pngSize);
	myLoopButton.setImages(true, true, true, loopnormalButton, 1.0f, {}, loopoverButton, 1.0f, {}, loopdownButton, 1.0f, {});
	addAndMakeVisible(myLoopButton);
	myLoopButton.addListener(this);
	myLoopButton.setClickingTogglesState(true);

	//set up the images for myBackwardButton
	juce::Image backwardnormalButton = juce::ImageCache::getFromMemory(BinaryData::backward_png, BinaryData::backward_pngSize);
	juce::Image backwardoverButton = juce::ImageCache::getFromMemory(BinaryData::backwardover_png, BinaryData::backwardover_pngSize);
	juce::Image backwarddownButton = juce::ImageCache::getFromMemory(BinaryData::backwarddown_png, BinaryData::backwarddown_pngSize);
	myBackwardButton.setImages(true, true, true, backwardnormalButton, 1.0f, {}, backwardoverButton, 1.0f, {}, backwarddownButton, 1.0f, {});
	addAndMakeVisible(myBackwardButton);
	myBackwardButton.addListener(this);

	//set up the images for myForwardButton
	juce::Image forwardnormalButton = juce::ImageCache::getFromMemory(BinaryData::forward_png, BinaryData::forward_pngSize);
	juce::Image forwardoverButton = juce::ImageCache::getFromMemory(BinaryData::forwardover_png, BinaryData::forwardover_pngSize);
	juce::Image forwarddownButton = juce::ImageCache::getFromMemory(BinaryData::forwarddown_png, BinaryData::forwarddown_pngSize);
	myForwardButton.setImages(true, true, true, forwardnormalButton, 1.0f, {}, forwardoverButton, 1.0f, {}, forwarddownButton, 1.0f, {});
	myForwardButton.addListener(this);
	addAndMakeVisible(myForwardButton);

	//initialize the timer
	startTimer(100);

}

DeckGUI::~DeckGUI()
{
	stopTimer();
}

void DeckGUI::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); 

	g.fillAll(juce::Colours::whitesmoke);

	//update posSlider while playing
	if (player->getPositionRelative()> 0.) {

		posSlider.setValue(player->getPositionRelative());

		posSlider.repaint();
	}
}
	
void DeckGUI::resized()
{
	//set resizable postion and size of GUI

	double rowH = getHeight() / 6;

	myLoadButton.setBounds(0, 0, getWidth() /6, rowH);

	myBackwardButton.setBounds(getWidth() / 6, 0, getWidth() / 6, rowH );

	myPlayButton.setBounds(2 * getWidth() / 6, 0, getWidth() / 6, rowH);

	myStopButton.setBounds(3 * getWidth() / 6, 0, getWidth() / 6, rowH );

	myForwardButton.setBounds(4 * getWidth() / 6, 0, getWidth() / 6, rowH);

	myLoopButton.setBounds(5 * getWidth() / 6, 0, getWidth() / 6, rowH );
	
	posSlider.setBounds(0, rowH * 3, getWidth(), rowH);
	
	volSlider.setBounds(getWidth() / 4 , rowH * 3.8, getWidth() / 6, getWidth() / 6);
	volLabel.setBounds(getWidth() / 4, rowH * 5.3, getWidth() / 6, rowH / 2);

	speedSlider.setBounds(getWidth()/4 * 2.5, rowH * 3.8, getWidth() / 6, getWidth() / 6);
	speedLabel.setBounds(getWidth() / 4 * 2.5, rowH * 5.3, getWidth()/6, rowH / 2);

	waveformDisplay.setBounds(0, rowH, getWidth(), rowH*2);	
}

void DeckGUI::buttonClicked(juce::Button* button)
{

	playlistComponent->trackFound = false;

	if (button == &myPlayButton)
	{
		//set myPlayButton denpending on the toggle state
		if (button->getToggleState()) {
			player->play();
		}
		else{ 
			player->stop();
		}
	}

	if (button == &myStopButton)
	{
		//set up myStopButton depending on toggle state
		player->stop();
		myPlayButton.setToggleState(false,false);

		//reset current position and loop position
		player->setPosition(0.);
		player->setLoopStart(0.);
		player->setLoopEnd(1.);
		posSlider.setValue(0.);
	}

	if (button == &myBackwardButton)
	{	
		//set positon when clicking myBackwardButton
		player->setRewindPosition();
	}

	if (button == &myForwardButton)
	{
		//set positon when clicking myForwardButton
		player->setForwardPosition();
	}

	if (button == &myLoadButton)
	{
		//set load new audio file function for myLoadButton
		juce::FileChooser chooser{ "Select a file..." };
		if (chooser.browseForFileToOpen())
		{
			auto audioURL = juce::URL{ chooser.getResult() };
			player->loadURL(audioURL);
			waveformDisplay.loadURL(audioURL);
			myPlayButton.setToggleState(false, false);
			//update playlist when new file added
			playlistComponent->addToTrackDatas(audioURL, player->getTrackDuration());
		}
		//reset loop position when new file loaded
		player->setLoopStart(0.);
		player->setLoopEnd(1.);
	}

	if (button == &myLoopButton)
	{
		//set loop state depending on the toggle stae
		player->setLoopState(button->getToggleState());
	}
}

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
	if (&volSlider == slider)
	{
		//set gain control of the volslider
		player->setGain(slider->getValue());
	}

	if (&speedSlider == slider)
	{
		//set speed control of the speed slider
		player->setSpeed(slider->getValue());
	}

	if (&posSlider == slider)
	{	
		if (slider->getValue() == slider->getMaximum()) { 
			//if reach the end of track, reset myPlayButton 
			myPlayButton.setToggleState(false, NULL);
		}
		else {
			//set position control of the posslider
			player->setPositionRelative(slider->getValue());
		}		
	}
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
	//enable drag and drop function
	return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
	if (files.size() == 1) {
		player->loadURL(juce::URL{ juce::File{files[0]} });
		//repaint waveform when new file dropped
		waveformDisplay.loadURL(juce::URL{ juce::File{files[0]} });
		//update trackDatas when new file dropped
		playlistComponent->addToTrackDatas(juce::URL{ juce::File{files[0]} }, player->getTrackDuration());
	}	
}

void DeckGUI::timerCallback()
{
	waveformDisplay.setPositionRelative(player->getPositionRelative());
	waveformDisplay.setTime(player->getPosition());
}


