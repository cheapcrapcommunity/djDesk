#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager* _formatManager)
	: formatManager(_formatManager)
{

}

DJAudioPlayer::~DJAudioPlayer()
{}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double
	sampleRate)
{

	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo&
	bufferToFill)
{

	//loop audio
	if (looping)
	{
		//set position to section start if outside section
		if (getPositionRelative() < loopStart ||
			getPositionRelative() >= loopEnd)
		{
			setPositionRelative(loopStart);
		}
	}
	else
		//non-looping
	{
		//catch up start to section
		if (getPositionRelative() < loopStart)
		{
			setPositionRelative(loopStart);
		}
		//stop at end of selection
		if (getPositionRelative() >= loopEnd)
		{
			stop();
			setPositionRelative(loopEnd);
			sendChangeMessage(); //broadcast message to registered listeners
		}
	}
	resampleSource.getNextAudioBlock(bufferToFill);

}

void DJAudioPlayer::releaseResources()
{
	transportSource.releaseResources();
	resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(const juce::URL& audioURL)
{
	auto* reader =
		formatManager->createReaderFor(audioURL.createInputStream(false));
	if (reader) // good file!
	{
		std::unique_ptr<juce::AudioFormatReaderSource> newSource(new
			juce::AudioFormatReaderSource(reader, true));
		transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
		readerSource.reset(newSource.release());
	}
}

void DJAudioPlayer::play()
{
	if (transportSource.getCurrentPosition() != 0) {
		if (!looping) {
			transportSource.setPosition(transportSource.getCurrentPosition());
		}
		else {
			transportSource.setPosition(getLoopStart());
		}
	}
	else {
		transportSource.setPosition(0);
	}

	transportSource.start();
}

void DJAudioPlayer::stop()
{
	transportSource.stop();
}

void DJAudioPlayer::setPosition(double posInSecs)
{
	transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
	if (pos < 0 || pos > 1.0)
	{
		std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
	}
	else {

		double posInSecs = transportSource.getLengthInSeconds() * pos;

		setPosition(posInSecs);
	}
}

void DJAudioPlayer::setRewindPosition()
{
	//set time to move backward
	double currentPositionSeconds = transportSource.getCurrentPosition();
	if (currentPositionSeconds > 5.0)
	{
		setPosition(currentPositionSeconds - 5.0);
	}
	else
	{
		setPosition(0.0);
	}
}

void DJAudioPlayer::setForwardPosition()
{
	double currentPositionSeconds = transportSource.getCurrentPosition();
	double newForwardPosition = currentPositionSeconds + 5.0;
	double audioTrackLength = transportSource.getLengthInSeconds();

	//set time to move forward
	if (newForwardPosition > audioTrackLength)
	{
		setPosition(audioTrackLength);
	}
	else {
		setPosition(newForwardPosition);
	}

}

void DJAudioPlayer::setGain(double gain)
{
	if (gain < 0. || gain > 1.)
	{
		std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1"<< std::endl;
	}
	else {
		transportSource.setGain(gain);
	}
}

void DJAudioPlayer::setSpeed(double ratio)
{
	if (ratio <= 0.)
	{
		std::cout << "DJAudioPlayer::setSpeed ratio must be greater than 0"<< std::endl;
	}
	else {
		resampleSource.setResamplingRatio(ratio);
	}
}

double DJAudioPlayer::getPositionRelative()
{
	return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

double DJAudioPlayer::getPosition()
{
	return transportSource.getCurrentPosition();
}

void DJAudioPlayer::setLoopStart(const double& ratio)
{
	//set restriction for loop start point
	if (ratio >= loopEnd || ratio < 0 || ratio > 1)
	{
		return;
	}

	loopStart = ratio;
}

void DJAudioPlayer::setLoopEnd(const double& ratio)
{
	//set restriction for loop end point
	if (ratio <= loopStart || ratio < 0 || ratio > 1)
	{
		return;
	}

	loopEnd = ratio;
}

double DJAudioPlayer::getLoopStart() const
{
	return loopStart;
}

double DJAudioPlayer::getLoopEnd() const
{
	return loopEnd;
}

void DJAudioPlayer::setLoopState(const bool& loop)
{
	readerSource->setLooping(loop);
	looping = loop;
}

bool DJAudioPlayer::getLoopState() const
{
	return looping;
}

double DJAudioPlayer::getEndPos() {

	return transportSource.getLengthInSeconds();
}

juce::String DJAudioPlayer::getTrackDuration()
{
	//convert track length to track duration in time format
	double tempLengthInMin = transportSource.getLengthInSeconds() / 60;
	double secToMin = (tempLengthInMin - floor(tempLengthInMin)) * 0.6;
	double _duration = (floor(tempLengthInMin) + secToMin);
	//convert to string
	juce::String duration = juce::String(_duration).substring(0, 4).replaceCharacter('.', ':');
	return duration;
}

juce::String DJAudioPlayer::getTrackDurationWithoutLoadingIntoDeck(juce::URL audioURL)
{
	juce::String duration;
	auto* reader = formatManager->createReaderFor(audioURL.createInputStream(false));
	if (reader != nullptr) // good file!
	{
		std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));

		double tempLengthInMin = (reader->lengthInSamples / reader->sampleRate) / 60;
		double secToMin = (tempLengthInMin - floor(tempLengthInMin)) * 0.6;
		double _duration = (floor(tempLengthInMin) + secToMin);
		duration = juce::String(_duration).substring(0, 4).replaceCharacter('.', ':');

		newSource.reset();
	}
	return duration;
}