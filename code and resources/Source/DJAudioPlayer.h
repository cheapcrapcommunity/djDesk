#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


class DJAudioPlayer :public juce::AudioSource,
					 public juce::ChangeBroadcaster
{
public:

	DJAudioPlayer(juce::AudioFormatManager* _formatManager);

	~DJAudioPlayer();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

	void releaseResources() override;

	void loadURL(const juce::URL& file);

	void play();

	void stop();

	void setPosition(double posInSecs);

	void setPositionRelative(double pos);

	void setGain(double gain);

	void setSpeed(double ratio);

	void setRewindPosition();

	void setForwardPosition();

	double getPositionRelative();

	double getPosition();

	double getEndPos();

	void setLoopStart(const double& ratio);

	double getLoopStart() const;

	void setLoopEnd(const double& ratio);

	double getLoopEnd() const;

	void setLoopState(const bool& loop);

	bool getLoopState() const;

	juce::String getTrackDuration();

	juce::String getTrackDurationWithoutLoadingIntoDeck(juce::URL audioURL);


private:

	juce::AudioFormatManager* formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	juce::AudioTransportSource transportSource;
	juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

	double  loopStart{ 0.0 };   
	double  loopEnd{ 1.0 }; 
	bool    looping{ false };  
};
