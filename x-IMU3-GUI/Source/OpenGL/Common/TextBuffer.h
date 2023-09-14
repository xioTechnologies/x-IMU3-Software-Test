#pragma once

#include <juce_opengl/juce_opengl.h>

class TextBuffer
{
public:
    explicit TextBuffer()
    {
        using namespace ::juce::gl;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &ebo);
        glGenBuffers(1, &vbo);
    }

    ~TextBuffer()
    {
        using namespace ::juce::gl;
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(1, &vbo);
    }

    void fillBuffer(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices)
    {
        using namespace ::juce::gl;

        glBindVertexArray(vao); // bind VAO to cache all VBO, EBO, and vertex attribute state

        // Fill EBO buffer with indices array
        indicesCount = (GLuint) indices.size();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLsizeiptr(sizeof(GLuint) * (unsigned long) indicesCount), indices.data(), GL_DYNAMIC_DRAW); // TODO: Use GL_STATIC_DRAW

        // Fill VBO buffer with vertices array
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(sizeof(GLfloat) * vertices.size()), vertices.data(), GL_DYNAMIC_DRAW); // TODO: Use GL_STATIC_DRAW

        // Define that our vertices are laid out as groups of 5 GLfloats (3 for position (XYZ), 2 for texture coordinate (UV))
        const GLsizei positionDimension = 3;
        const GLsizei textureCoordDimension = 2;
        const GLsizei totalDimensions = positionDimension + textureCoordDimension;
        const GLsizei vertexDataLength = totalDimensions * sizeof(GLfloat);

        // Position attribute (3 floats)
        const GLuint positionIndex = 0;
        glVertexAttribPointer(positionIndex, positionDimension, GL_FLOAT, GL_FALSE, vertexDataLength, nullptr);
        glEnableVertexAttribArray(positionIndex);

        // Texture coordinate attribute (2 float)
        const GLuint textureCoordIndex = 1;
        glVertexAttribPointer(textureCoordIndex, textureCoordDimension, GL_FLOAT, GL_FALSE, vertexDataLength, (void*) (positionDimension * sizeof(GLfloat)));
        glEnableVertexAttribArray(textureCoordIndex);

        // Unbind buffers
        glBindVertexArray(0); // unbind VAO first since it remembers last bound VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind EBO
    }

    void draw() const
    {
        using namespace ::juce::gl;
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, (GLsizei) indicesCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

private:
    GLuint vao {};
    GLuint ebo {};
    GLuint vbo {};

    GLuint indicesCount = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextBuffer)
};
