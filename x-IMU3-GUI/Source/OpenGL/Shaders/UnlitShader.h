#pragma once

#include <BinaryData.h>
#include "OpenGL/Common/GLUtil.h"
#include "OpenGL/Common/Shader.h"

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

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UnlitShader)
};
