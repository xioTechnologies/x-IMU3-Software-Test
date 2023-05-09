#pragma once

#include "OpenGL/Base/GLUtil.hpp"
#include "OpenGL/Base/Shader.h"

class ScreenSpaceUnlitShader : public Shader
{
public:
    explicit ScreenSpaceUnlitShader(juce::OpenGLContext& context_) : Shader("Screen Space Unlit", context_, BinaryData::ScreenSpaceUnlit_vert, BinaryData::Unlit_frag)
    {
    }

    GLUtil::Uniform modelViewProjectionMatrix { *this, "modelViewProjectionMatrix" };

    GLUtil::Uniform colour { *this, "colour" };
    GLUtil::Uniform isTextured { *this, "isTextured" };
    GLUtil::Uniform textureImage { *this, "textureImage" };

    GLUtil::Uniform inverseScreenScale { *this, "inverseScreenScale"};
};
