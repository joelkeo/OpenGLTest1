//
// Created by chels on 5/13/2025.
//

#include "Waveform.h"
Waveform::Waveform()
{
    // IM_JIMM BOILERPLATE
    // Indicates that no part of this Component is transparent.
    setOpaque(true);
    // Set this instance as the renderer for the context.
    openGLContext.setRenderer(this);
    // Tell the context to repaint on a loop.
    openGLContext.setContinuousRepainting(true);
    // Finally - we attach the context to this Component.
    openGLContext.attachTo(*this);
}

void Waveform::newOpenGLContextCreated()
{
    // STEP 1: CREATE BUFFERS (gen and bind)
    init_glGenBuffers();
    init_glBindBuffers();
    // STEP 2: INIT VERTICES
    init_assignLocalVertexData();
    // STEP 3: assign index buffer on GLs end
    init_glBufferData();
    // STEP 4: Create shader data on our end
    init_assignLocalShaderData();
    // STEP 4: assign shaders
    init_linkShaders();
    // MY CODE:
    iTimeLocation = shaderProgram->getUniformIDFromName("iTime");
}

void Waveform::renderOpenGL()
{
    // STEP1: Clear the screen by filling it with black.
    juce::OpenGLHelpers::clear(juce::Colours::black);
    // STEP2: compute shaders
    shaderProgram->use();
    // STEP3: use buffers
    // render_glBindBuffers(); <-- was in original code, but doesn't seem to be needed
    // MY CODE:
    float currentTime = (float) juce::Time::getMillisecondCounterHiRes() / 1000.f;
    shaderProgram->setUniform("iTime", currentTime);
    // STEP4: assign attrib data metadata
    render_glEnableAttributes();
    // STEP 5: draw
    render_glDrawElements();
    // STEP 6: clear attrib data
    render_glDisableAttributes();
}

void Waveform::openGLContextClosing()
{
}

// ---------------------------------------------------------------------------------------------------------
// INIT HELPERS:
void Waveform::init_glGenBuffers() {
    // Generate 1 buffer, using our vbo variable to store its ID.
    openGLContext.extensions.glGenBuffers(1, &vbo);
    // Generate 1 more buffer, this time using our IBO variable.
    openGLContext.extensions.glGenBuffers(1, &ibo);
}
void Waveform::init_glBindBuffers() {
    // bind VBO
    openGLContext.extensions.glBindBuffer(juce::gl::GL_ARRAY_BUFFER, vbo);
    // bind IBO
    openGLContext.extensions.glBindBuffer(juce::gl::GL_ELEMENT_ARRAY_BUFFER, ibo);
}
void Waveform::init_assignLocalVertexData() {
    // Create 4 vertices each with a different colour.
    vertexBuffer = {{
            { -.8f, 0.8f },
            { 1.f, 0.f, 0.f, 1.f }},
        {
    { 0.8f, 0.8f },
                { 1.f, 0.5f, 0.f, 1.f }},

        {
            { 0.8f, -0.8f },
                { 1.f, 1.f, 0.f, 1.f }},

        {
            { -0.8f, -0.8f },
                { 1.f, 0.f, 1.f, 1.f }}
    };

    // We need 6 indices, 1 for each corner of the two triangles.
    indexBuffer = {
        0, 1, 2,
        0, 2, 3
    };
}
void Waveform::init_glBufferData() {
    // Send the vertices data.
    openGLContext.extensions.glBufferData(
        juce::gl::GL_ARRAY_BUFFER,                        // The type of data we're sending.
        sizeof(Vertex) * vertexBuffer.size(),   // The size (in bytes) of the data.
        vertexBuffer.data(),                    // A pointer to the actual data.
        juce::gl::GL_STATIC_DRAW                          // How we want the buffer to be drawn.
    );
    // Send the indices data.
    openGLContext.extensions.glBufferData(
        juce::gl::GL_ELEMENT_ARRAY_BUFFER,
        sizeof(unsigned int) * indexBuffer.size(),
        indexBuffer.data(),
        juce::gl::GL_STATIC_DRAW
    );
}
void Waveform::init_assignLocalShaderData() {
    vertexShader =
           R"(
            #version 330 core

            // Input attributes.
            in vec4 position;
            in vec4 sourceColour;

            // Output to fragment shader.
            out vec4 fragColour;

            void main()
            {
                // Set the position to the attribute we passed in.
                gl_Position = position;

                // Set the frag colour to the attribute we passed in.
                // This value will be interpolated for us before the fragment
                // shader so the colours we defined ateach vertex will create a
                // gradient across the shape.
                fragColour = sourceColour;
            }
        )";

    fragmentShader =
        R"(
            #version 330 core

            // The value that was output by the vertex shader.
            // This MUST have the exact same name that we used in the vertex shader.
            in vec4 fragColour;
uniform float iTime;
            void main()
            {
                 // Example base color
    vec4 baseColor = vec4(0.3, 0.6, 0.9, 1.0);

    // Modulate color brightness with sine wave over time
    float brightness = 0.5 + 0.5 * sin(5. * iTime);

    // Apply modulation
    vec4 fragColour = baseColor * brightness;

    gl_FragColor = fragColour;
            }
        )";
}
void Waveform::init_linkShaders() {
    // Create an instance of OpenGLShaderProgram
    shaderProgram.reset(new juce::OpenGLShaderProgram(openGLContext));
    if (shaderProgram->addVertexShader(vertexShader)
        && shaderProgram->addFragmentShader(fragmentShader)
        && shaderProgram->link())
    {}
    else {
        // Oops - something went wrong with our shaders!
        // Check the output window of your IDE to see what the error might be.
        jassertfalse;
    }
}
// RENDER HELPERS:
void Waveform::render_glBindBuffers() {
    init_glBindBuffers();
}
void Waveform::render_glEnableAttributes() {
    // Enable the position attribute.
    openGLContext.extensions.glVertexAttribPointer(
        0,              // The attribute's index (AKA location).
        2,              // How many values this attribute contains.
        juce::gl::GL_FLOAT,       // The attribute's type (float).
        juce::gl::GL_FALSE,       // Tells OpenGL NOT to normalise the values.
        sizeof(Vertex), // How many bytes to move to find the attribute with
                        // the same index in the next vertex.
        nullptr         // How many bytes to move from the start of this vertex
                        // to find this attribute (the default is 0 so we just
                        // pass nullptr here).
    );
    openGLContext.extensions.glEnableVertexAttribArray(0);

    // Enable to colour attribute.
    openGLContext.extensions.glVertexAttribPointer(
        1,                              // This attribute has an index of 1
        4,                              // This time we have four values for the
        juce::gl::
                                        // attribute (r, g, b, a)
        GL_FLOAT,
        juce::gl::GL_FALSE,
        sizeof(Vertex),
        (GLvoid*)(sizeof(float) * 2)    // This attribute comes after the
                                        // position attribute in the Vertex
                                        // struct, so we need to skip over the
                                        // size of the position array to find
                                        // the start of this attribute.
    );
    openGLContext.extensions.glEnableVertexAttribArray(1);
}
void Waveform::render_glDrawElements() {
    juce::gl::glDrawElements(
     juce::gl::GL_TRIANGLES,       // Tell OpenGL to render triangles.
        indexBuffer.size(), // How many indices we have.
        juce::gl::GL_UNSIGNED_INT,    // What type our indices are.
        nullptr             // We already gave OpenGL our indices so we don't
                            // need to pass that again here, so pass nullptr.
    );
}
void Waveform::render_glDisableAttributes() {
    openGLContext.extensions.glDisableVertexAttribArray(0);
    openGLContext.extensions.glDisableVertexAttribArray(1);
}
