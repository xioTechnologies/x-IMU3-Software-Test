#pragma once

#include <vector>
#include <algorithm>
#include "GLResources.h"
#include "OpenGLComponent.h"

class GLRenderer : private juce::OpenGLRenderer
{
public:
    explicit GLRenderer(juce::Component& attachTo);

    ~GLRenderer() override;

    juce::OpenGLContext& getContext();

    void addComponent(OpenGLComponent& component);

    void removeComponent(OpenGLComponent& component);

    GLResources& getResources();

    void turnCullingOff();

    void turnDepthTestOff();

    void refreshScreen(const juce::Colour& colour, const juce::Rectangle<GLint>& viewport);

    const juce::Matrix3D<GLfloat>& getProjectionMatrix(juce::Rectangle<GLint> viewport);

private:
    juce::OpenGLContext context;

    /** Used to synchronize access to data shared between the JUCE Message thread
     *  and the OpenGL thread such as the components list, and the toExecuteOnGLThread
     *  list.
     */
    std::mutex sharedGLDataLock;

    std::vector<OpenGLComponent*> components;

    std::unique_ptr<GLResources> resources;

    // TODO: I don't think we need these in a rendering base class like this. Only necessary
    // per each GL rendering program like the ThreeDView or a 2D graph, etc.
    juce::Matrix3D<GLfloat> viewMatrix;
    juce::Matrix3D<GLfloat> projectionMatrix;

    /** List of functions to execute on the GL thread.
     */
    std::vector<std::function<void(juce::OpenGLContext&)>> toExecuteOnGLThread;

    void newOpenGLContextCreated() override;

    void renderOpenGL() override;

    void openGLContextClosing() override;
};
