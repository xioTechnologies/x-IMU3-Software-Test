#pragma once

#include <BinaryData.h>
#include "OpenGL/Common/GLUtil.hpp"
#include "OpenGL/Common/Shader.h"

class CompassUnlitShader : public Shader
{
public:
    explicit CompassUnlitShader(juce::OpenGLContext& context_) : Shader("CompassUnlit", context_, BinaryData::Unlit_vert, BinaryData::CompassUnlit_frag)
    {
    }

    GLUtil::Uniform modelViewProjectionMatrix { *this, "modelViewProjectionMatrix" };

    GLUtil::Uniform textureImage { *this, "textureImage" };
    GLUtil::Uniform tint { *this, "tint" };
    GLUtil::Uniform northColour { *this, "northColour" };
};
