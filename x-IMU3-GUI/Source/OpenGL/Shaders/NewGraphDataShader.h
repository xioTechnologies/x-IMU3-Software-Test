#pragma once

#include <BinaryData.h>
#include "OpenGL/Common/GLUtil.h"
#include "OpenGL/Common/Shader.h"

class NewGraphDataShader : public Shader
{
public:
    explicit NewGraphDataShader(juce::OpenGLContext& context_) : Shader("NewGraphData", context_, BinaryData::NewGraphData_vert, BinaryData::NewGraphData_frag)
    {
    }

    GLUtil::Uniform colour { *this, "colour" };

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NewGraphDataShader)
};
