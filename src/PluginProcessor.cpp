#include "PluginProcessor.h"
#include "PluginEditor.h"

OpenGLTest1AudioProcessor::OpenGLTest1AudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

OpenGLTest1AudioProcessor::~OpenGLTest1AudioProcessor() {}

const juce::String OpenGLTest1AudioProcessor::getName() const {
    return "OpenGLTest1";
}

bool OpenGLTest1AudioProcessor::acceptsMidi() const { return false; }
bool OpenGLTest1AudioProcessor::producesMidi() const { return false; }
bool OpenGLTest1AudioProcessor::isMidiEffect() const { return false; }
double OpenGLTest1AudioProcessor::getTailLengthSeconds() const { return 0.0; }

int OpenGLTest1AudioProcessor::getNumPrograms() { return 1; }
int OpenGLTest1AudioProcessor::getCurrentProgram() { return 0; }
void OpenGLTest1AudioProcessor::setCurrentProgram(int) {}
const juce::String OpenGLTest1AudioProcessor::getProgramName(int) { return {}; }
void OpenGLTest1AudioProcessor::changeProgramName(int, const juce::String&) {}

void OpenGLTest1AudioProcessor::prepareToPlay(double, int) {}
void OpenGLTest1AudioProcessor::releaseResources() {}

bool OpenGLTest1AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo()
        && layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo();
}

void OpenGLTest1AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    DBG("TOP OF PROCESS BLOCK");
    auto playHead = getPlayHead();
    float foundPosition = 0.f;
    if (playHead) {
        auto position = playHead->getPosition();
        if (position) {
            auto optPosition = position->getPpqPosition();
            if (optPosition) {
                foundPosition = *optPosition;
            }
        }
    }
    rmsBuffer.addData(buffer, foundPosition);
    for (int i = 0; i < 10; i++) {
        DBG("SAMPLE: " + juce::String(i) + ", val: " + juce::String(rmsBuffer.rmsBuffer[i]));
    }
    for (int channel = 0; channel < getTotalNumInputChannels(); ++channel)
        buffer.clear(channel, 0, buffer.getNumSamples());
}

bool OpenGLTest1AudioProcessor::hasEditor() const {
    return true;
}

juce::AudioProcessorEditor* OpenGLTest1AudioProcessor::createEditor() {
    return new OpenGLTest1AudioProcessorEditor(*this);
}

void OpenGLTest1AudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
    juce::MemoryOutputStream(destData, true).writeString("PluginState");
}

void OpenGLTest1AudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    juce::MemoryInputStream stream(data, static_cast<size_t>(sizeInBytes), false);
    juce::String state = stream.readString();
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OpenGLTest1AudioProcessor(); // Replace with your actual processor class name
}
