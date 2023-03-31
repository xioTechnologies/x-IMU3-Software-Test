#include "../CustomLookAndFeel.h"
#include "GLRenderer.h"

GLRenderer::GLRenderer(juce::Component& attachTo)
{
    context.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
    context.setRenderer(this);
    context.setContinuousRepainting(true);
    context.attachTo(attachTo);
}

GLRenderer::~GLRenderer()
{
    context.detach();
}

juce::OpenGLContext& GLRenderer::getContext()
{
    return context;
}

void GLRenderer::addComponent(OpenGLComponent& component)
{
    std::lock_guard<std::mutex> _(sharedGLDataLock);
    OpenGLComponent* const componentPtr = &component;
    components.push_back(componentPtr);

    // Initialize the Component's OpenGL data on the OpenGL thread
    toExecuteOnGLThread.push_back([this, componentPtr](juce::OpenGLContext&)
                                  {
                                      // If component is still in the list, initialize its OpenGL data
                                      if (componentPtr && std::find(components.begin(), components.end(), componentPtr) != components.end())
                                      {
                                          componentPtr->initGLData();
                                      }
                                  });
}

void GLRenderer::removeComponent(OpenGLComponent& component)
{
    std::lock_guard<std::mutex> _(sharedGLDataLock);
    OpenGLComponent* const componentPtr = &component;
    auto componentItr = std::find(components.begin(), components.end(), componentPtr);

    // If the component still exists in the list
    if (componentItr != components.end())
    {
        components.erase(componentItr);

        // Deallocate the Component's OpenGL data on the OpenGL thread
        // TODO: This lambda causes a BAD_ACCESS when removing a graph OpenGL component.
        // The component is destroyed before the function can be called.
        // One solution is pass the OpenGLComponent to this function as a
        // shared_ptr so it will live until this lambda is called.
        // TODO: Reimplement with shared_ptr, etc.
        /*
        toExecuteOnGLThread.push_back([componentPtr](juce::OpenGLContext &){
            if (componentPtr)
            {
                componentPtr->deinitGLData();
            }
        });
         */
    }
}

GLResources& GLRenderer::getResources()
{
    return *resources;
}

void GLRenderer::turnCullingOff()
{
    juce::gl::glDisable(juce::gl::GL_CULL_FACE);
}

void GLRenderer::turnDepthTestOff()
{
    juce::gl::glDisable(juce::gl::GL_DEPTH_TEST);
}

void GLRenderer::refreshScreen(const juce::Colour& colour, const juce::Rectangle<GLint>& viewport)
{
    juce::gl::glEnable(juce::gl::GL_BLEND);
    juce::gl::glEnable(juce::gl::GL_CULL_FACE);
    juce::gl::glEnable(juce::gl::GL_DEPTH_TEST);
    juce::gl::glEnable(juce::gl::GL_SCISSOR_TEST);

    juce::gl::glLineWidth(1.0f);
    juce::gl::glEnable(juce::gl::GL_LINE_SMOOTH);
    juce::gl::glEnable(juce::gl::GL_MULTISAMPLE);

    juce::gl::glCullFace(juce::gl::GL_BACK);
    juce::gl::glFrontFace(juce::gl::GL_CCW);
    juce::gl::glBlendFunc(juce::gl::GL_SRC_ALPHA, juce::gl::GL_ONE_MINUS_SRC_ALPHA);

    juce::gl::glViewport(viewport.getX(), viewport.getY(), viewport.getWidth(), viewport.getHeight());
    juce::gl::glScissor(viewport.getX(), viewport.getY(), viewport.getWidth(), viewport.getHeight());
    juce::gl::glClearColor(colour.getFloatRed(), colour.getFloatGreen(), colour.getFloatBlue(), colour.getFloatAlpha());
    juce::gl::glClear(juce::gl::GL_COLOR_BUFFER_BIT | juce::gl::GL_DEPTH_BUFFER_BIT);

    juce::gl::glDisable(juce::gl::GL_SCISSOR_TEST);
}

const juce::Matrix3D<GLfloat>& GLRenderer::getProjectionMatrix(juce::Rectangle<GLint> viewport)
{
    const float farClip = 1000.0f;
    const float nearClip = 0.1f;
    const auto aspectRatio = viewport.toFloat().getAspectRatio();
    const auto tanHalfFov = tan(juce::degreesToRadians(30.0f) / 2.0f);

    // Formula to build a 3D perspective view using data above (taken from GLM math library)
    // We are using a column-major matrix implementation here
    projectionMatrix.mat[0] = 1.0f / (aspectRatio * tanHalfFov);
    projectionMatrix.mat[5] = 1.0f / (tanHalfFov);
    projectionMatrix.mat[10] = -(farClip + nearClip) / (farClip - nearClip);
    projectionMatrix.mat[11] = -1.0f;
    projectionMatrix.mat[14] = -(2.0f * farClip * nearClip) / (farClip - nearClip);
    projectionMatrix.mat[15] = 0.0f;

    return projectionMatrix;
}

void GLRenderer::newOpenGLContextCreated()
{
    //All texture, shader, model and buffer resources are created here
    resources = std::make_unique<GLResources>(context);
}

void GLRenderer::renderOpenGL()
{
    std::lock_guard<std::mutex> _(sharedGLDataLock);

    // Execute pending actions on the OpenGL thread
    for (const auto& func : toExecuteOnGLThread)
    {
        func(context);
    }
    toExecuteOnGLThread.clear();

    // Clear the entire OpenGL screen with the background color
    juce::OpenGLHelpers::clear(UIColours::backgroundDark);

    // Render components
    for (auto& component : components)
    {
        component->render();
    }
}

void GLRenderer::openGLContextClosing()
{
    {
        std::lock_guard<std::mutex> _(sharedGLDataLock);

        // Deallocate OpenGL data of all active components
        for (auto& component : components)
        {
            component->deinitGLData();
        }
        components.clear();
    }

    //All texture, shader, model and buffer resources are freed here
    resources.reset();
}
