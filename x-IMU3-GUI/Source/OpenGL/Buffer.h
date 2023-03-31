#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>

class Buffer
{
public:
    enum VboType
    {
        vertexBuffer,
        colourBuffer,
        textureBuffer,
        normalBuffer,
        totalBuffers
    };

    // TODO: I am unsure what single fill vs multiple fill means?
    // Static draw means the vertices will be set once. Dynamic draw
    // means the vertices may be set multiple times. I think we should
    // remove this enum and just use the GL constants where necessary:
    // GL_STREAM_DRAW, GL_STATIC_DRAW, and GL_DYNAMIC_DRAW
    enum FillType
    {
        singleFill = juce::gl::GL_STATIC_DRAW,
        multipleFill = juce::gl::GL_DYNAMIC_DRAW
    };

    // TODO: Remove this and just stick with the OpenGL constants
    // theres also others like GL_TRIANGLE_STRIP.
    enum DrawType
    {
        points = juce::gl::GL_POINTS,
        triangles = juce::gl::GL_TRIANGLES,
        lines = juce::gl::GL_LINES,
        lineStrip = juce::gl::GL_LINE_STRIP,
    };

    // TODO: Remove this and just stick with the OpenGL constants
    enum DataType
    {
        integer = juce::gl::GL_INT,
        floatingPoint = juce::gl::GL_FLOAT,
        unsignedInteger = juce::gl::GL_UNSIGNED_INT
    };

    enum ComponentType
    {
        XY = 2,
        Xyz = 3,
        Rgb = 3,
        Rgba = 4,
        UV = 2
    };

    explicit Buffer(juce::OpenGLContext& context);

    ~Buffer();

    void create(GLuint totalVertices_, bool hasEbo_ = false);

    void fillVbo(VboType vboType, const GLfloat* data, GLsizeiptr bufferSize, FillType fillType = singleFill);

    void fillEbo(const GLuint* data, GLsizeiptr bufferSize, FillType fillType = singleFill);

    void appendVbo(VboType vboType, const GLfloat* data, GLsizeiptr size, GLuint offset);

    void appendEbo(const GLuint* data, GLsizeiptr size, GLuint offset);

    // UNIMPLEMENTED!
    void linkEbo()
    {
    }

    void linkVbo(GLuint attributeID, VboType vboType, ComponentType componentType, DataType dataType);

    void disableAttribute(GLuint attributeID);

    void render(DrawType drawType, const int numberOfVertices = -1);

private:
    juce::OpenGLContext& glContext;

    GLuint vao;
    bool hasEbo;
    GLuint ebo;
    GLuint totalVertices;
    GLuint vbos[totalBuffers];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Buffer)
};
