 #version 330 core

// The value that was output by the vertex shader.
// This MUST have the exact same name that we used in the vertex shader.
in vec4 fragColour;
uniform float iTime;
void main() {
 
    vec4 baseColor = vec4(0.5176, 0.6706, 0.7059, 1.0);

    // Modulate color brightness with sine wave over time
    float brightness = 0.5 + 0.5 * sin(5. * iTime);

    // Apply modulation
    vec4 fragColour = baseColor * brightness;

    gl_FragColor = fragColour;
}