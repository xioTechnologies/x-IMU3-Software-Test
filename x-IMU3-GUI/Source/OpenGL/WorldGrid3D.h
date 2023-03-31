#pragma once

#include <BinaryData.h>
#include <juce_opengl/juce_opengl.h>

#include <utility>
#include "GLUtil.hpp"

class WorldGrid3D
{
public:
    explicit WorldGrid3D(juce::OpenGLContext* context);

    ~WorldGrid3D() = default;

    void initGLData();

    void deinitGLData();

    void render();

    juce::Matrix3D<GLfloat> projectionMatrix;
    juce::Matrix3D<GLfloat> modelMatrix;

private:
    juce::OpenGLContext* openGLContext = nullptr;

    // Geometry and buffers being sent into the Shader Program pipeline.
    // It is possible you may want to send more than just this geometry into
    // a single Shader program, so should the idea of geometry be abstracted
    // or paired with the actual Shader Program? In original ThreeDView.h the
    // geometry was abstracted into Model.h and one or two shader programs
    // were being used to render multiple sets of geometry.
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    // A shader program used when rendering this class specifically.
    // If this were extracted into a base class, it is possible you
    // may want the ability to have more than one shader program???
    // Or would it be we want the ability to have more than one
    // of these base classes per OpenGLComponent?
    //
    // A more descriptive name for this particular shader program
    // would be something like "unlitShader". But, I am keeping this
    // generic for now as I think we can extract this into a more
    // generic class.
    std::unique_ptr<juce::OpenGLShaderProgram> shaderProgram;

    // Variables which control aspects of the Shader program pipeline
    // For any Shader pipeline, these uniforms could be kept in an array,
    // so when the shader program is compiled like in compileOpenGLShaderProgram()
    // you could easily loop through all Uniforms/Attributes to detach/attach.
    GLUtil::UniformWrapper projectionMatrixUniform { "projectionMatrix" };
    GLUtil::UniformWrapper modelMatrixUniform { "modelMatrix" };

    // GUI overlay status text to report compilation status and GLSL version
    // being used.
    // This is really a per-OpenGLShaderProgram set of data that could be used
    // for debugging.
    juce::String openGLStatusText;
    juce::Label openGLStatusLabel;

    static void createSquare(const float scale, std::vector<GLfloat>& verticesOut, std::vector<GLuint>& indicesOut);

    // This could be generified for any shader program. The only thing that would be
    // different per shader program is the pipeline of shaders. You could have a virtual
    // function for a generic shader program class the implements its pipeline:
    // Ex:
    //      bool defineShaderPipeline(OpenGLShaderProgram * program)
    //      {
    //          return program->addVertexShader ({ BinaryData::Unlit_Vertex_glsl })
    //              && program->addGeometryShader ({ BinaryData::Extrude_Geometry_glsl })
    //              && program->addFragmentShader ({ BinaryData::Unlit_Fragment_glsl });
    //      }
    // Then the generic shader program class would call this and run program->link()
    // then check for compilation success, attach uniforms to the compiled program,
    // and update a status string for debugging.
    //
    void compileOpenGLShaderProgram();
};