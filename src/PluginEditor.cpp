#include "PluginProcessor.h"
#include "PluginEditor.h"

OpenGLTest1AudioProcessorEditor::OpenGLTest1AudioProcessorEditor(OpenGLTest1AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
    setSize(400, 300);
    addAndMakeVisible(&waveform);
}

OpenGLTest1AudioProcessorEditor::~OpenGLTest1AudioProcessorEditor() {}

void OpenGLTest1AudioProcessorEditor::paint(juce::Graphics& g) {}

void OpenGLTest1AudioProcessorEditor::resized() {
    waveform.setBounds(getLocalBounds());
}
