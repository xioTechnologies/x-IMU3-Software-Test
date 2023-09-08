#pragma once

#include <BinaryData.h>
#include "OpenGL/Common/Shader.h"
#include "OpenGL/Common/GLUtil.h"

class NewGraphGridShader : public Shader
{
public:
    explicit NewGraphGridShader(juce::OpenGLContext& context_) : Shader("NewGraphGrid", context_, BinaryData::NewGraphGrid_vert, BinaryData::NewGraphGrid_frag)
    {
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NewGraphGridShader)
};
