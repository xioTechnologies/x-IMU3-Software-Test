//
// Created by Tim Arterbury on 3/28/23.
//
#include "WorldGrid3D.h"

using namespace ::juce::gl;

WorldGrid3D::WorldGrid3D(juce::OpenGLContext* context)
{
    jassert (context);
    openGLContext = context;
}

void WorldGrid3D::initGLData()
{
    // Would be done for any generic GL program class
    compileOpenGLShaderProgram();

    createSquare(1.0f, vertices, indices);

    // Generate opengl vertex objects
    glGenVertexArrays(1, &VAO); // Vertex Array Object
    glGenBuffers(1, &VBO); // Vertex Buffer Object
    glGenBuffers(1, &EBO); // Element Buffer Object

    // Bind VAO which remembers all VBO, EBO, and vertex attribute state
    glBindVertexArray(VAO);

    // Fill VBO buffer with vertices array
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(sizeof(GLfloat) * vertices.size()), vertices.data(), GL_STATIC_DRAW);

    // Fill EBO buffer with indices array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLsizeiptr(sizeof(GLuint) * indices.size()), indices.data(), GL_STATIC_DRAW);

    // Define that our vertices are laid out as groups of 5 GLfloats (3 for position, 2 for texture coord)
    const GLsizei positionDimension = 3;
    const GLsizei texutreDimension = 2;
    const GLsizei vertexDataLength = (positionDimension + texutreDimension) * sizeof(GLfloat);

    // Position attribute (3 floats)
    const GLuint positionIndex = 0;
    glVertexAttribPointer(positionIndex, positionDimension, GL_FLOAT, GL_FALSE, vertexDataLength, nullptr);
    glEnableVertexAttribArray(positionIndex);

    // Texture coordinate attribute (2 floats)
    const GLuint textureCoordinateIndex = 1;
    glVertexAttribPointer(textureCoordinateIndex, texutreDimension, GL_FLOAT, GL_FALSE, vertexDataLength, (void*) (positionDimension * sizeof(GLfloat)));
    glEnableVertexAttribArray(textureCoordinateIndex);

    // Unbind buffers
    glBindVertexArray(0); // unbind VAO first since it remembers all bound buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind EBO
}

void WorldGrid3D::deinitGLData()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    shaderProgram.reset(nullptr);
    // glDeleteProgram(shaderProgram); gets called via OpenGLShaderProgram
    // via OpenGLShaderProgram destructor
    // NOTE: In the same way juce::OpenGLShaderProgram gives nice RAII for
    // OpenGL shader, we could utilize the already created Buffer.h to handle
    // deallocation of these kinds of data, maybe we should NOT get rid of
    // Buffer.h as I initially suggested. But Buffer.h could definitely be
    // refactored to prevent unnecessary extra bind calls.
}

void WorldGrid3D::render()
{
    // Select shader program
    shaderProgram->use();

    if (projectionMatrixUniform)
    {
        projectionMatrixUniform->setMatrix4(projectionMatrix.mat, 1, false);
    }
    if (modelMatrixUniform)
    {
        modelMatrixUniform->setMatrix4(modelMatrix.mat, 1, false);
    }

    // Allow front and back of faces to be seen. If we need to texture the plane
    // differently on top/bottom, then remove this and cull back faces.
    glDisable(GL_CULL_FACE);

    // Draw Vertices
    glBindVertexArray(VAO); // automatically binds VBO, EBO, and associated vertex attribute config
    glDrawElements(GL_TRIANGLES, (GLsizei) indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0); // unbind VAO to prevent modification to the VAO by subsequent GL calls

    glEnable(GL_CULL_FACE); // restore cull state
}

void WorldGrid3D::createSquare(const float scale, std::vector<GLfloat>& verticesOut, std::vector<GLuint>& indicesOut)
{
    // TODO: Consider replacing with GLM library but maybe that's overkill?
    // We could replace all linear algebra OpenGL math classes with GLM.
    struct vec2
    {
        GLfloat x;
        GLfloat y;
    };

    const float aspect = scale / 2.0f;
    std::vector<juce::Vector3D<GLfloat>> positions = {
            { aspect,  aspect,  0.0f }, // top right
            { aspect,  -aspect, 0.0f }, // bottom right
            { -aspect, -aspect, 0.0f }, // bottom left
            { -aspect, aspect,  0.0f }, // top left
    };

    std::vector<vec2> textureCoordinates = {
            { 1.0f, 1.0f }, // top right
            { 1.0f, 0.0f }, // bottom right
            { 0.0f, 0.0f }, // bottom left
            { 0.0f, 1.0f }, // top left
    };

    indicesOut = {
            0, 1, 3, // first triangle
            1, 2, 3, // second triangle
    };

    // Consolidate data into a GLfloat array for use by OpenGL
    jassert(positions.size() == textureCoordinates.size());
    const unsigned long numVertices = positions.size();
    verticesOut.clear();
    for (unsigned long i = 0; i < numVertices; ++i)
    {
        const auto position = positions[i];
        verticesOut.push_back(position.x);
        verticesOut.push_back(position.y);
        verticesOut.push_back(position.z);

        if (i < textureCoordinates.size()) // Ensure valid access
        {
            const auto textureCoordinate = textureCoordinates[i];
            verticesOut.push_back(textureCoordinate.x);
            verticesOut.push_back(textureCoordinate.y);
        }
    }
}

void WorldGrid3D::compileOpenGLShaderProgram()
{
    auto shaderProgramAttempt = std::make_unique<juce::OpenGLShaderProgram>(*openGLContext);

    // Attempt to compile the program
    if (shaderProgramAttempt->addVertexShader({ BinaryData::Grid3D_vertex_glsl })
        && shaderProgramAttempt->addFragmentShader({ BinaryData::Grid3D_fragment_glsl })
        && shaderProgramAttempt->link())
    {
        projectionMatrixUniform.disconnectFromShaderProgram();
        modelMatrixUniform.disconnectFromShaderProgram();

        shaderProgram = std::move(shaderProgramAttempt);

        projectionMatrixUniform.connectToShaderProgram(*openGLContext, *shaderProgram);
        modelMatrixUniform.connectToShaderProgram(*openGLContext, *shaderProgram);

        openGLStatusText = "GLSL: v" + juce::String(juce::OpenGLShaderProgram::getLanguageVersion(), 2);
    }
    else
    {
        openGLStatusText = shaderProgramAttempt->getLastError();
    }

    //triggerAsyncUpdate(); // Update status text
}