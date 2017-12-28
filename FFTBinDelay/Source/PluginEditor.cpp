/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
FftbinDelayAudioProcessorEditor::FftbinDelayAudioProcessorEditor (FftbinDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
	, mSlider(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

	setSize(400, 300);

	addAndMakeVisible(&mSlider);

	addAndMakeVisible(&openButton);
	openButton.setButtonText("Open...");
	openButton.addListener(this);

	addAndMakeVisible(&playStopButton);
	playStopButton.setButtonText("Play");
	playStopButton.addListener(this);
	playStopButton.setColour(TextButton::buttonColourId, Colours::green);
	playStopButton.setEnabled(true);

	addAndMakeVisible(&feedbackSlider);
	feedbackSlider.addListener(this);
	feedbackSlider.setRange(0.0, 1.0, 0.001);

	addAndMakeVisible(&bypassButton);
	bypassButton.addListener(this);
	playStopButton.setColour(TextButton::buttonColourId, Colours::aliceblue);
	bypassButton.setButtonText("ByPass");

	addAndMakeVisible(&delayRangeButton);
	delayRangeButton.addListener(this);
	delayRangeButton.setButtonText("delayTime * 100");

	refreshButtons();
	refreshSliders();

	setSize(500, 500);

	processor.transportSource.addChangeListener(this);   // zorgt ervoor dat elke change in transportSource de listener functie acti
}

FftbinDelayAudioProcessorEditor::~FftbinDelayAudioProcessorEditor()
{
}

//==============================================================================
void FftbinDelayAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("2.6.1 Stereo Chaotic Delay's. Files cleaned and working on the TODO list", getLocalBounds(), Justification::bottomRight, 1);
}

void FftbinDelayAudioProcessorEditor::resized()
{
	auto space = getLocalBounds();

	auto topHeader = space.removeFromTop(50);
	openButton.setBounds(topHeader.removeFromLeft(getWidth() / nButtons));
	playStopButton.setBounds(topHeader.removeFromLeft(getWidth() / nButtons));
	bypassButton.setBounds(topHeader.removeFromLeft(getWidth() / nButtons));
	delayRangeButton.setBounds(topHeader.removeFromLeft(getWidth() / nButtons));

	auto midHeader = space.removeFromTop(50);
	feedbackSlider.setBounds(midHeader.removeFromTop( midHeader.getHeight()) );

	space.reduce(50, 50);
	mSlider.setBounds(space);
}

// callback when something changes in the processor.
void FftbinDelayAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == &processor.transportSource) {
		refreshButtons();
	}
}

// the overrided abstract sliderValueChanged function. Called if a slider value is changed.
void FftbinDelayAudioProcessorEditor::sliderValueChanged(Slider* slider) {
	if (slider == &feedbackSlider) {
		//slider->setColour(slider->thumbColourId, Colours::rosybrown);
		newFeedbackSliderValue();
	}
}

void FftbinDelayAudioProcessorEditor::refreshButtons() {
	if (processor.transportSource.getLengthInSeconds() <= 0.0) {
		playStopButton.setEnabled(false);
	}
	else {
		playStopButton.setEnabled(true);
		openButton.setColour(TextButton::buttonColourId, Colours::green);
	}

	if (processor.transportSource.isPlaying()) {
		playStopButton.setButtonText("Stop");
		openButton.setEnabled(false);
	}
	else {
		playStopButton.setButtonText("Play");
		openButton.setEnabled(true);
	}

	if (processor.transportSource.hasStreamFinished()) {
		processor.transportSource.setPosition(0.0);
	}

	if (processor.bypass == true) {
		bypassButton.setColour(TextButton::buttonColourId, Colours::green);
	}
	else {
		bypassButton.setColour(TextButton::buttonColourId, Colours::black);
	}

	refreshRangeButton();
}

void FftbinDelayAudioProcessorEditor::refreshRangeButton() {
	if (processor.delayTime == rangeHigh) {
		delayRangeButton.setColour(TextButton::buttonColourId, Colours::green);
	}
	else {
		delayRangeButton.setColour(TextButton::buttonColourId, Colours::black);
	}
}

// the overrided abstract button clicked function. Called when a button is clicked.
void FftbinDelayAudioProcessorEditor::buttonClicked(Button* button)
{
	if (button == &openButton) {
		openButtonClicked();
	}
	if (button == &playStopButton) {
		playButtonClicked();
	}
	if (button == &bypassButton) {
		bypassButtonClicked();
	}
	if (button == &delayRangeButton) {
		rangeButtonClicked();
	}
}

void FftbinDelayAudioProcessorEditor::playButtonClicked() {
	playStopButton.setColour(TextButton::buttonColourId, Colours::rebeccapurple); //test de functie call.
	processor.playStopButtonClicked();
};

void FftbinDelayAudioProcessorEditor::openButtonClicked() {
	openButton.setColour(TextButton::buttonColourId, Colours::rebeccapurple);
	processor.openButtonClicked();
};

void FftbinDelayAudioProcessorEditor::bypassButtonClicked() {
	if (processor.bypass == false) {
		bypassButton.setColour(TextButton::buttonColourId, Colours::green);
		processor.bypass = true;
	}
	else {
		bypassButton.setColour(TextButton::buttonColourId, Colours::black);
		processor.bypass = false;
	}
};

void FftbinDelayAudioProcessorEditor::rangeButtonClicked() {
	if (processor.delayTime == rangeHigh) {
		delayRangeButton.setColour(TextButton::buttonColourId, Colours::black);
		processor.delayTime = rangeLow;
		delayRangeButton.setButtonText("Short");
		delayRangeButton.setColour(TextButton::textColourOffId, Colours::lightblue);
	}
	else {
		delayRangeButton.setColour(TextButton::buttonColourId, Colours::green);
		processor.delayTime = rangeHigh;
		delayRangeButton.setButtonText("Long");
		delayRangeButton.setColour(TextButton::textColourOffId, Colours::white);
	}

	mSlider.refreshBinDelayTimeValues();
}

void FftbinDelayAudioProcessorEditor::newFeedbackSliderValue() {
	processor.setFeedbackValue( feedbackSlider.getValue());
};

void FftbinDelayAudioProcessorEditor::refreshSliders() {
	feedbackSlider.setValue( processor.getFeedbackValue());
	mSlider.refreshDelaySliderValues(processor.getBinDelayArray());
};

