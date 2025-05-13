#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class OpenGLTest1AudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit OpenGLTest1AudioProcessorEditor(OpenGLTest1AudioProcessor&);
    ~OpenGLTest1AudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    OpenGLTest1AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGLTest1AudioProcessorEditor)
};
