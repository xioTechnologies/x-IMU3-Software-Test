#include "PlaneModel.h"

PlaneModel::PlaneModel()
{
    initGLData();
}

PlaneModel::~PlaneModel()
{
    deinitGLData();
}

void PlaneModel::initGLData()
{
    using namespace ::juce::gl;

    makePlaneGeometry(1.0f, vertices, indices);

    // Generate opengl vertex objects
    glGenVertexArrays(1, &VAO); // Vertex Array Object
    glGenBuffers(1, &VBO); // Vertex Buffer Object
    glGenBuffers(1, &EBO); // Element Buffer Object

    glBindVertexArray(VAO); // Bind VAO to cache all VBO, EBO, and vertex attribute state

    // Fill VBO buffer with vertices array
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(sizeof(GLfloat) * vertices.size()), vertices.data(), GL_STATIC_DRAW);

    // Fill EBO buffer with indices array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLsizeiptr(sizeof(GLuint) * indices.size()), indices.data(), GL_STATIC_DRAW);

    // Define that our vertices are laid out as groups of 5 GLfloats (3 for position, 2 for texture coord)
    const GLsizei positionDimension = 3;
    const GLsizei normalDimension = 3;
    const GLsizei textureDimension = 2;
    const GLsizei vertexDataLength = (positionDimension + normalDimension + textureDimension) * sizeof(GLfloat);

    // Position attribute (3 floats)
    const GLuint positionIndex = 0;
    glVertexAttribPointer(positionIndex, positionDimension, GL_FLOAT, GL_FALSE, vertexDataLength, nullptr);
    glEnableVertexAttribArray(positionIndex);

    // Normal attribute (3 floats)
    const GLuint normalIndex = 1;
    glVertexAttribPointer(normalIndex, normalDimension, GL_FLOAT, GL_FALSE, vertexDataLength, (void*) (positionDimension * sizeof(GLfloat)));
    glEnableVertexAttribArray(normalIndex);

    // Texture coordinate attribute (2 floats)
    const GLuint textureCoordinateIndex = 2;
    glVertexAttribPointer(textureCoordinateIndex, textureDimension, GL_FLOAT, GL_FALSE, vertexDataLength, (void*) ((positionDimension + normalDimension) * sizeof(GLfloat)));
    glEnableVertexAttribArray(textureCoordinateIndex);

    // Unbind buffers
    glBindVertexArray(0); // unbind VAO first since it remembers all bound buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind EBO
}

void PlaneModel::deinitGLData()
{
    using namespace ::juce::gl;

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void PlaneModel::render() const
{
    using namespace ::juce::gl;

    // Draw Vertices
    glBindVertexArray(VAO); // automatically binds VBO, EBO, and associated vertex attribute config
    glDrawElements(GL_TRIANGLES, (GLsizei) indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0); // unbind VAO to prevent modification to the VAO by subsequent GL calls
}

void PlaneModel::makePlaneGeometry(const float extent, std::vector <GLfloat>& verticesOut, std::vector <GLuint>& indicesOut)
{
    // Oriented flat on the XZ plane of OpenGL's default coordinate system, where "close" means
    // +z direction out of the screen and "far" means -z direction into the screen
    std::vector <glm::vec3> positions = {
            { extent,  0.0f, -extent }, // far right
            { extent,  0.0f, extent }, // close right
            { -extent, 0.0f, extent }, // close left
            { -extent, 0.0f, -extent }, // far left
    };

    // All vertices have the same normal vector pointing up
    glm::vec3 normal = { 0.0f, 1.0f, 0.0f };

    std::vector <glm::vec2> textureCoordinates = {
            { 1.0f, 1.0f }, // far right
            { 1.0f, 0.0f }, // close right
            { 0.0f, 0.0f }, // close left
            { 0.0f, 1.0f }, // far left
    };

    // All triangles have counter-clockwise winding order so they are front-facing when viewed from above
    indicesOut = {
            0, 3, 1, // first triangle
            1, 3, 2, // second triangle
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

        verticesOut.push_back(normal.x);
        verticesOut.push_back(normal.y);
        verticesOut.push_back(normal.z);

        if (i < textureCoordinates.size()) // ensure valid access
        {
            const auto textureCoordinate = textureCoordinates[i];
            verticesOut.push_back(textureCoordinate.x);
            verticesOut.push_back(textureCoordinate.y);
        }
    }
}
