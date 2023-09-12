#pragma once

#include <BinaryData.h>
#include "OpenGL/Common/GLHelpers.h"
#include "OpenGL/Common/Shader.h"

class UnlitShader : public Shader
{
public:
    explicit UnlitShader(juce::OpenGLContext& context_) : Shader("Unlit", context_, BinaryData::Unlit_vert, BinaryData::Unlit_frag)
    {
    }

    GLHelpers::Uniform modelViewProjectionMatrix { *this, "modelViewProjectionMatrix" };

    GLHelpers::Uniform colour { *this, "colour" };
    GLHelpers::Uniform isTextured { *this, "isTextured" };
    GLHelpers::Uniform textureImage { *this, "textureImage" };

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UnlitShader)
};
