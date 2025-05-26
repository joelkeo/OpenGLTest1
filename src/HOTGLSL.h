//
// Created by chels on 5/19/2025.
//

#ifndef HOTGLSL_H
#define HOTGLSL_H

#include <JuceHeader.h>
// TODO: add non hot reload mode for release
class HotGLSL {
public:
    HotGLSL(juce::String vertexShaderFilePath,
        juce::String fragmentShaderFilePath) {
        vertexShaderFile = juce::File(vertexShaderFilePath);
        fragmentShaderFile = juce::File(fragmentShaderFilePath);
        jassert(vertexShaderFile.existsAsFile() && fragmentShaderFile.existsAsFile());
        // Time of 0, so it definitely gets updated
        lastModified = juce::Time();
        checkIfFileChanged();
        reloader.startThread();
    }
    ~HotGLSL() {
        reloader.stopThread(400);
    }
    juce::OpenGLShaderProgram* getShaderProgram() {
        return isFirst ? shaderProgram1.get() : shaderProgram2.get();
    }
    void callInRenderLoop(juce::OpenGLContext& gl) {
        if (wasJustUpdated) {
            DBG("UPDATE DETECTED");
            if (isFirst) {shaderProgram1.reset(new juce::OpenGLShaderProgram(gl));}
            else {shaderProgram2.reset(new juce::OpenGLShaderProgram(gl));}
            juce::OpenGLShaderProgram* shaderProgram = getShaderProgram();
            wasJustUpdated = false;
            if (shaderProgram->addVertexShader(vertexShader)
                && shaderProgram->addFragmentShader(fragmentShader)
                && shaderProgram->link()) {
                DBG("not glsl compile errors");
                }
            else {
                // Oops - something went wrong with our shaders!
                // Check the output window of your IDE to see what the error might be.
                jassertfalse;
            }
        }
        getShaderProgram()->use();
    }
private:
    juce::String filePath;
    juce::String vertexShader;
    juce::String fragmentShader;
    std::unique_ptr<juce::OpenGLShaderProgram> shaderProgram1;
    std::unique_ptr<juce::OpenGLShaderProgram> shaderProgram2;
    bool wasJustUpdated = false;
    bool isFirst = true;
    juce::Time lastModified;
    juce::File vertexShaderFile;
    juce::File fragmentShaderFile;
    void revert() {

    }
    void checkIfFileChanged() {
        juce::Time vertexShaderUpdateTime = vertexShaderFile.getLastModificationTime();
        juce::Time fragmentShaderUpdateTime = fragmentShaderFile.getLastModificationTime();
        juce::Time maxTime = std::max(vertexShaderUpdateTime, fragmentShaderUpdateTime);
        if (maxTime > lastModified) {
            wasJustUpdated = true;
            lastModified = maxTime;
            vertexShader = vertexShaderFile.loadFileAsString();
            fragmentShader = fragmentShaderFile.loadFileAsString();
        }
    }
    class Reloader : public juce::Thread {
    public:
        Reloader(HotGLSL& hotglsl) : juce::Thread("glslHotReloader"), hotglsl(hotglsl) {}
        void run() {
            while (! threadShouldExit()) {
                hotglsl.checkIfFileChanged();
                wait(300);
            }
        }
    private:
        HotGLSL& hotglsl;
    };
    Reloader reloader = Reloader(*this);
};



#endif //HOTGLSL_H
