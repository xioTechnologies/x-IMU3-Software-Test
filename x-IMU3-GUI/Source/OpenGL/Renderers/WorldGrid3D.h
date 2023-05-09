#pragma once

#include <BinaryData.h>
#include "glm/mat4x4.hpp"
#include <juce_opengl/juce_opengl.h>
#include "OpenGL/Base/GLResources.h"
#include "utility"

class WorldGrid3D
{
public:
    explicit WorldGrid3D(const GLResources&);

    ~WorldGrid3D();

    void render();

    glm::mat4 modelViewProjectionMatrix;

private:
    const GLResources& resources;
};
