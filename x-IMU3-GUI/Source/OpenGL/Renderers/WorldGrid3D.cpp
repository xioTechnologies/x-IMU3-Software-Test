//
// Created by Tim Arterbury on 3/28/23.
//
#include "WorldGrid3D.h"

WorldGrid3D::WorldGrid3D(const GLResources& resources_) : resources(resources_)
{
}

WorldGrid3D::~WorldGrid3D()
{
}

void WorldGrid3D::render()
{
    using namespace ::juce::gl;

    // Select shader program
    resources.grid3DShader.use();

    // Set shader uniforms
    if (resources.grid3DShader.modelViewProjectionMatrix)
    {
        resources.grid3DShader.modelViewProjectionMatrix.set(modelViewProjectionMatrix);
    }

    // TODO: The necessary GL_CULL_FACE disable/enable is currently happening in ThreeDView.cpp because it needs to be wrapped around both Grid and compass. Consider just dissolving this class OR moving more code from ThreeDView into there so its one simple .render() call.
    // Allow front and back of faces to be seen. If we need to texture the plane
    // differently on top/bottom, then remove this and cull back faces.
    //glDisable(GL_CULL_FACE);

    resources.plane.render();

    //glEnable(GL_CULL_FACE); // restore cull state
}
