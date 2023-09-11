#pragma once

#include <BinaryData.h>
#include "OpenGL/Common/GLUtil.h"
#include "OpenGL/Common/Shader.h"

class TextShader : public Shader
{
public:
    TextShader(juce::OpenGLContext& context_) : Shader("Text", context_, BinaryData::Text_vert, BinaryData::Text_frag)
    {
    }

    GLHelpers::Uniform colour { *this, "colour" };
    Uniform textureImage { *this, "textureImage" };
    GLHelpers::Uniform transformation { *this, "transformation" };

    // TODO: Attribute variables can be removed in refactor
    Attribute vertexIn { *this, "vertexIn" };
    Attribute textureIn { *this, "textureIn" };

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextShader)
};
