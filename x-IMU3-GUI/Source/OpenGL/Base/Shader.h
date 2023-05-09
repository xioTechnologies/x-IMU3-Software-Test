#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class Shader : public juce::OpenGLShaderProgram
{
public:
    Shader(const juce::String& name_, juce::OpenGLContext& context_, const juce::String& vertexShader, const juce::String& fragmentShader) : juce::OpenGLShaderProgram(context_), name(name_)
    {
        // Attempt to compile the shader program
        if ((addVertexShader(vertexShader) == false) || (addFragmentShader(fragmentShader) == false) || (link() == false))
        {
            std::cout << "OpenGL compilation failed for shader program: " << name << "\n" << getLastError() << std::endl;
            jassertfalse;
        }
    }

private:
    const juce::String name;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Shader)
};
