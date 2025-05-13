#include "PluginProcessor.h"
#include "PluginEditor.h"

OpenGLTest1AudioProcessorEditor::OpenGLTest1AudioProcessorEditor(OpenGLTest1AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(400, 300);
}

OpenGLTest1AudioProcessorEditor::~OpenGLTest1AudioProcessorEditor() {}

void OpenGLTest1AudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawFittedText("OpenGLTest1 Plugin", getLocalBounds(), juce::Justification::centred, 1);
}

void OpenGLTest1AudioProcessorEditor::resized()
{
    // Layout child components here
}
