#include "freetype/freetype.h"
#include "OpenGL/Common/GLResources.h"
#include "Text.h"

static FT_Library freetypeLibrary; // TODO: Make member of Initializer and rename Initializer

Text::Initializer::Initializer()
{
    FT_Init_FreeType(&freetypeLibrary);
}

Text::Initializer::~Initializer()
{
    FT_Done_FreeType(freetypeLibrary);
}

Text::Text(const bool isFirstLetterCentered_) : isFirstLetterCentered(isFirstLetterCentered_)
{
}

bool Text::loadFont(const char* data, size_t dataSize, unsigned int fontSize_)
{
    using namespace ::juce::gl;

    fontSize = (GLuint) fontSize_;

    FT_Face face = nullptr;
    if (FT_New_Memory_Face(freetypeLibrary, reinterpret_cast<const FT_Byte*>(data), (FT_Long) dataSize, 0, &face))
    {
        return false;
    }

    FT_Set_Pixel_Sizes(face, fontSize, fontSize);

    descender = (float) face->size->metrics.descender / 64.0f;


    // Create OpenGL Textures for every font character that will be used
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable OpenGL default 4-byte alignment restriction since we store grayscale data with 1 byte per pixel
    glActiveTexture(GL_TEXTURE0); // use first texture unit for all textures bound below because shader only uses 1 texture at a time
    for (int i = 0; i < 256; i++)
    {
        if (FT_Load_Char(face, (FT_ULong) i, FT_LOAD_RENDER)) // if freetype fails to load the current glyph index
        {
            continue;
        }

        // Generate texture
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     (GLsizei) face->glyph->bitmap.width,
                     (GLsizei) face->glyph->bitmap.rows,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);

        // Set texture wrapping preferences
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Set magnification/minification filters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        Glyph glyph = { textureID,
                        face->glyph->bitmap.width,
                        face->glyph->bitmap.rows,
                        face->glyph->bitmap_left,
                        face->glyph->bitmap_top,
                        (GLint) face->glyph->advance.x };

        alphabet[(GLchar) i] = glyph;
    }

    FT_Done_Face(face);
    return true;
}

void Text::unloadFont()
{
    for (auto& glyph : alphabet)
    {
        juce::gl::glDeleteTextures(1, &(glyph.second.textureID));
    }

    alphabet.clear();
}

unsigned int Text::getFontSize() const
{
    return fontSize;
}

GLuint Text::getTotalWidth()
{
    unsigned int totalWidth = 0;

    for (int i = 0; i < text.length(); i++)
    {
        Glyph glyph = alphabet[(GLchar) text[i]];
        totalWidth += (GLuint) (glyph.advance / 64.0f);
    }

    return totalWidth;
}

int Text::getStringWidthGLPixels(const juce::String& string) const
{
    int width = 0;

    for (const auto& character : string)
    {
        const Glyph glyph = alphabet.at((GLchar) character);
        width += (int) std::ceil(glyph.advance / 64.0f); // TODO: What is magic number 64.0f?
    }

    return width;
}

int Text::getStringWidthJucePixels(const juce::String& string) const
{
    return (int) std::ceil(getStringWidthGLPixels(string) / juce::OpenGLContext::getCurrentContext()->getRenderingScale());
}

float Text::getDescender() const
{
    return descender;
}

const juce::String& Text::getText() const
{
    return text;
}

void Text::setText(const juce::String& text_)
{
    text = text_;
}

void Text::setScale(const juce::Point<GLfloat>& scale_)
{
    scale = scale_;
}

// TODO: Unused consider removing
void Text::setPosition(const juce::Vector3D<GLfloat>& position_)
{
    position = position_;
}

void Text::renderScreenSpace(GLResources* const resources, const juce::String& label, const juce::Colour& colour, const glm::mat4& transform)
{
    // Calculate Normalized Device Coordinates (NDC) transformation to place text at position on screen with a constant size
    glm::vec2 ndcCoord = glm::vec2(transform[3][0], transform[3][1]) / transform[3][3]; // get x, y of matrix translation then divide by w of translation for constant size in pixels
    const auto zTranslation = transform[3][2]; // use z of matrix translation so 2D elements have proper layering
    const auto ndcMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(ndcCoord, zTranslation));

    resources->textShader.colour.setRGBA(colour);
    resources->textShader.transform.set(ndcMatrix);

    setText(label);
    render(resources);
}

void Text::render(GLResources* const resources)
{
    // TODO: Unused textOrigin, remove
    auto textOrigin = position;

    for (size_t index = 0; index < (size_t) text.length(); index++)
    {
        Glyph glyph = alphabet[(GLchar) text[(int) index]];

        auto halfWidth = glyph.width * 0.5f;
        auto halfBearingY = glyph.bearingY * 0.5f;

        if (isFirstLetterCentered)
        {
            GLfloat vertices[] = { textOrigin.x - (halfWidth * scale.x), textOrigin.y + (halfBearingY * scale.y), 0.0f,
                                   textOrigin.x + (halfWidth * scale.x), textOrigin.y + (halfBearingY * scale.y), 0.0f,
                                   textOrigin.x + (halfWidth * scale.x), textOrigin.y + (halfBearingY * scale.y) - (glyph.height * scale.y), 0.0f,
                                   textOrigin.x - (halfWidth * scale.x), textOrigin.y + (halfBearingY * scale.y) - (glyph.height * scale.y), 0.0f };

            resources->textBuffer.fillVbo(TextBuffer::vertexBuffer, vertices, sizeof(vertices), TextBuffer::multipleFill);
        }

        else
        {
            GLfloat vertices[] = { textOrigin.x + (glyph.bearingX * scale.x), textOrigin.y + (glyph.bearingY * scale.y), 0.0f,
                                   textOrigin.x + (glyph.bearingX * scale.x) + (glyph.width * scale.x), textOrigin.y + (glyph.bearingY * scale.y), 0.0f,
                                   textOrigin.x + (glyph.bearingX * scale.x) + (glyph.width * scale.x), textOrigin.y + (glyph.bearingY * scale.y) - (glyph.height * scale.y), 0.0f,
                                   textOrigin.x + (glyph.bearingX * scale.x), textOrigin.y + (glyph.bearingY * scale.y) - (glyph.height * scale.y), 0.0f };

            resources->textBuffer.fillVbo(TextBuffer::vertexBuffer, vertices, sizeof(vertices), TextBuffer::multipleFill);
        }

        GLfloat UVs[] = { 0.0f, 0.0f,
                          1.0f, 0.0f,
                          1.0f, 1.0f,
                          0.0f, 1.0f };

        GLuint indices[] = { 0, 1, 3,
                             3, 1, 2 };

        resources->textBuffer.linkEbo();
        resources->textBuffer.linkVbo(resources->textShader.vertexIn.attributeID, TextBuffer::vertexBuffer, TextBuffer::Xyz, TextBuffer::floatingPoint);
        resources->textBuffer.linkVbo(resources->textShader.textureIn.attributeID, TextBuffer::textureBuffer, TextBuffer::UV, TextBuffer::floatingPoint);

        resources->textBuffer.fillEbo(indices, sizeof(indices), TextBuffer::multipleFill);
        resources->textBuffer.fillVbo(TextBuffer::textureBuffer, UVs, sizeof(UVs), TextBuffer::multipleFill);

        {
            resources->textShader.setTextureImage(juce::gl::GL_TEXTURE_2D, glyph.textureID);
            resources->textBuffer.render(TextBuffer::triangles);
        }

        textOrigin.x += (glyph.advance * scale.x) / 64.0f;
    }
}
