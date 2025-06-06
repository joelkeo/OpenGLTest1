//
// Created by chels on 5/13/2025.
//

#ifndef WAVEFORM_H
#define WAVEFORM_H


#include <JuceHeader.h>
#include <juce_opengl/juce_opengl.h>
#include "HOTGLSL.h"
#include "RhythmicRMSBuffer.h"
class Waveform : public juce::Component, public juce::OpenGLRenderer {
public:
    Waveform(RhythmicRMSBuffer& rmsBuffer);
    void newOpenGLContextCreated() override;
    void renderOpenGL() override;
    void openGLContextClosing() override;
private:
    // rms buffer
    RhythmicRMSBuffer& rmsBuffer;
    // context: should be recieved from PARENT if this isn't the only openGLContext
    juce::OpenGLContext openGLContext;
    // reference to buffers
    GLuint vbo; // Vertex buffer object.
    GLuint ibo; // Index buffer object.
    // C++ vertex struct
    struct Vertex
    {
        float position[2];
        float colour[4];
    };
    // C++ buffers
    std::vector<Vertex> vertexBuffer;
    std::vector<unsigned int> indexBuffer;
    // shader code
    juce::String vertexShader;
    juce::String fragmentShader;
    // shader program
    std::unique_ptr<juce::OpenGLShaderProgram> shaderProgram;
    HotGLSL hotGLSL = HotGLSL("C:/Users/chels/Downloads/basement_audio/scratchpad/OpenGLTest1/glsl/OpenGLTest1.vert",
                    "C:/Users/chels/Downloads/basement_audio/scratchpad/OpenGLTest1/glsl/OpenGLTest1.frag");
    // INIT HELPERS:
    void init_glGenBuffers();
    void init_glBindBuffers();
    void init_assignLocalVertexData();
    void init_glBufferData();
    void init_assignLocalShaderData();
    void init_linkShaders();
    // RENDER HELPERS:
    void render_glBindBuffers();
    void render_glEnableAttributes();
    void render_glDrawElements();
    void render_glDisableAttributes();

    // UNIFROMS
    GLint iTimeLocation;
};



#endif //WAVEFORM_H
