#pragma once

#include "OpenGL/Base/GLUtil.hpp"
#include "OpenGL/Base/Shader.h"

class Grid3DShader : public Shader
{
public:
    explicit Grid3DShader(juce::OpenGLContext& context_) : Shader("Grid3D", context_, BinaryData::Grid3D_vert, BinaryData::Grid3D_frag)
    {
    }

    GLUtil::Uniform modelViewProjectionMatrix { *this, "modelViewProjectionMatrix" };
};
