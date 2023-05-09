#pragma once

#include "OpenGL/Base/GLUtil.hpp"
#include "OpenGL/Base/Shader.h"

class UnlitShader : public Shader
{
public:
    explicit UnlitShader(juce::OpenGLContext& context_) : Shader("Unlit", context_, BinaryData::Unlit_vert, BinaryData::Unlit_frag)
    {
    }

    GLUtil::Uniform modelViewProjectionMatrix { *this, "modelViewProjectionMatrix" };

    GLUtil::Uniform colour { *this, "colour" };
    GLUtil::Uniform isTextured { *this, "isTextured" };
    GLUtil::Uniform textureImage { *this, "textureImage" };
};
