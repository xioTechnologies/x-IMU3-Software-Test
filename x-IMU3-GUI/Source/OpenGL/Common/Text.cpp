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

    descender = toPixels((float) face->size->metrics.descender);

    // TODO: Optionally generate textures ONLY for the characters we need for graph & axes: numbers 0-9, -, x, y, z, E

    // Create OpenGL Textures for every font character that will be used
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable OpenGL default 4-byte alignment restriction since we store grayscale data with 1 byte per pixel
    glActiveTexture(GL_TEXTURE0); // use first texture unit for all textures bound below because shader only uses 1 texture at a time
    for (unsigned char c = 0; c < 128; c++) // TODO: Was originally first 256 characters
    {
        if (FT_Load_Char(face, (FT_ULong) c, FT_LOAD_RENDER)) // if freetype fails to load the current glyph index
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

        // Set magnification/minification filters - GL_NEAREST for sharper text
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        Glyph glyph = { textureID,
                        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                        toPixels((float) face->glyph->advance.x) };
        alphabet[c] = glyph;
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

float Text::getTotalWidth()
{
    float width = 0.0f;

    for (auto character : text)
    {
        auto glyphSearch = alphabet.find(static_cast<unsigned char>(character));
        if (glyphSearch == alphabet.end())
        {
            continue;
        }
        const Glyph glyph = glyphSearch->second;
        width += glyph.advance;
    }

    return width;
}

int Text::getStringWidthGLPixels(const juce::String& string) const
{
    int width = 0;

    for (const auto& character : string)
    {
        auto glyphSearch = alphabet.find(static_cast<unsigned char>(character));
        if (glyphSearch == alphabet.end())
        {
            continue;
        }
        const Glyph glyph = glyphSearch->second;
        width += (int) std::ceil(glyph.advance);
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
    scale_old = scale_;
}

// TODO: Unused consider removing
void Text::setPosition(const juce::Vector3D<GLfloat>& position_)
{
    position = position_;
}

void Text::renderScreenSpace(GLResources* const resources, const juce::String& label, const juce::Colour& colour, const glm::mat4& transform, juce::Rectangle<int> viewportBounds)
{
    setText(label);

    // Calculate Normalized Device Coordinates (NDC) transformation to place text at position on screen with a constant size
    glm::vec2 ndcCoord = glm::vec2(transform[3][0], transform[3][1]) / transform[3][3]; // get x, y of matrix translation then divide by w of translation for constant size in pixels
    const auto zTranslation = transform[3][2]; // use z of matrix translation so 2D elements have proper layering
    const auto ndcMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(ndcCoord, zTranslation));

    auto& textShader = resources->textShader;
    textShader.use();
    textShader.colour.setRGBA(colour);
    auto textOrigin = juce::Point<float>(0.0f, 0.0f);
    for (size_t index = 0; index < (size_t) text.length(); index++)
    {
        auto glyphSearch = alphabet.find(static_cast<unsigned char>(text[(int) index]));
        if (glyphSearch == alphabet.end())
        {
            continue;
        }
        const Glyph glyph = glyphSearch->second;

        // TODO: Add back this functionality, but make it a parameter instead
        //  Basically only difference is, isFirstLetterCentered translates all letters back to reach the center of the first letter.
//        if (isFirstLetterCentered)
//        {
//
//        }
//        else
//        {
//
//        }

        auto scaleFactor = glm::vec2 ((float) glyph.size.x / (float) viewportBounds.getWidth(), (float) glyph.size.y / (float) viewportBounds.getHeight());
        auto scale = glm::scale(glm::mat4(1.0), glm::vec3(scaleFactor, 1.0f));
        auto newTransform = ndcMatrix * scale;
        //auto glyphCentre = glm::vec2((float) glyph.bearing.x + (0.5f * (float) glyph.size.x), (0.5f * (float) glyph.size.y) - ((float)glyph.size.y - (float) glyph.bearing.y));
        //auto translation = glm::translate(glm::mat4(1.0), glm::vec3(textOrigin + glyphCentre, 0.0f));
        //glm::mat4 newTransform = ndcMatrix * translation * scale;
        textShader.transform.set(newTransform);

        textShader.setTextureImage(juce::gl::GL_TEXTURE_2D, glyph.textureID);
        resources->textQuad.draw();

        textOrigin.x += glyph.advance; // move origin to next character
    }
}

void Text::render(GLResources* const resources, const juce::String& text_, glm::vec2 screenPosition, juce::Rectangle<int> viewport, const juce::Colour& colour, juce::Justification justification)
{
    setText(text_);

    if (justification.testFlags(juce::Justification::horizontallyCentred))
    {
        screenPosition.x -= getTotalWidth() / 2.0f;
    }
    else if (justification.testFlags(juce::Justification::right))
    {
        screenPosition.x -= getTotalWidth();
    }

    if (justification.testFlags(juce::Justification::verticallyCentred))
    {
        const auto offset = (GLfloat) getFontSize() / 2.0f + getDescender();
        screenPosition.y -= offset;
    }

    auto projection = glm::ortho((float) viewport.getX(), (float) viewport.getRight(), (float) viewport.getY(), (float) viewport.getY() + (float) viewport.getHeight());

    auto& textShader = resources->textShader;
    textShader.use();
    textShader.colour.setRGBA(colour);
    auto textOrigin = screenPosition;
    for (size_t index = 0; index < (size_t) text.length(); index++)
    {
        auto glyphSearch = alphabet.find(static_cast<unsigned char>(text[(int) index]));
        if (glyphSearch == alphabet.end())
        {
            continue;
        }
        const Glyph glyph = glyphSearch->second;

        // TODO: Add back this functionality, but make it a parameter instead
        //  Basically only difference is, isFirstLetterCentered translates all letters back to reach the center of the first letter.
        /*
        if (isFirstLetterCentered)
        {

        }
        else
        {

        }
         */

        // NOTE: The following is for when NOT isFirstLetterCentered

        auto scale = glm::scale(glm::mat4(1.0), glm::vec3(glyph.size, 0.0f));
        auto glyphCentre = glm::vec2((float) glyph.bearing.x + (0.5f * (float) glyph.size.x), (0.5f * (float) glyph.size.y) - ((float)glyph.size.y - (float) glyph.bearing.y));
        auto translation = glm::translate(glm::mat4(1.0), glm::vec3(textOrigin + glyphCentre, 0.0f));
        glm::mat4 transform = projection * translation * scale;
        textShader.transform.set(transform);

        textShader.setTextureImage(juce::gl::GL_TEXTURE_2D, glyph.textureID);
        resources->textQuad.draw();

        textOrigin.x += glyph.advance; // move origin to next character
    }
}

void Text::render(GLResources* const resources)
{
    // TODO: Unused textOrigin, remove
    // ACTUALLY NO, it is used, it is iterated every loop, BUT it begins at 0.0 because position is never modified!
    auto textOrigin = position;
    //  Translation is already accomplished via the transformation uniform in TextShader, so this is redundant.
    //  Let's also move the scale variable into the matrix as a scale matrix. Just scaling on X and Y
    //  For uniforms we may also need to pass glyph.width, glyph.height, and glyph.bearing to get proper results.
    //  It would also be possible to batch render the text glyphs in one draw call using an array of uniforms

    // From what I can tell,
    // isFirstLetterCentered == true
    //      X values are multiplied (scaled) by     glyph.width * 0.5f * scale.x
    //      Y values are multiplied (scaled) by     halfBearingY * scale.y
    //      THEN
    //      Y values of specific vertices are subtracted (translated) by  - (glyph.height * scale.y)
    //      XY values are all translated by  textOrigin.x
    // else
    //

    for (size_t index = 0; index < (size_t) text.length(); index++)
    {
        Glyph glyph = alphabet[static_cast<unsigned char>(text[(int) index])];

        //auto sizeScaled = glyph.size * scale.x 

        auto halfWidth = (float) glyph.size.x * 0.5f;
        auto halfBearingY = (float) glyph.bearing.y * 0.5f;

        // TODO: I would keep vertices in a static buffer but use matrix scaling to resize that square to the proper width height. Then we only pass uniforms.
        std::vector<GLfloat> vertices;
        if (isFirstLetterCentered)
        {
            vertices = { textOrigin.x - (halfWidth * scale_old.x), textOrigin.y + (halfBearingY * scale_old.y), 0.0f, 0.0f, 0.0f,
                         textOrigin.x + (halfWidth * scale_old.x), textOrigin.y + (halfBearingY * scale_old.y), 0.0f, 1.0f, 0.0f,
                         textOrigin.x + (halfWidth * scale_old.x), textOrigin.y + (halfBearingY * scale_old.y) - (glyph.size.y * scale_old.y), 0.0f, 1.0f, 1.0f,
                         textOrigin.x - (halfWidth * scale_old.x), textOrigin.y + (halfBearingY * scale_old.y) - (glyph.size.y * scale_old.y), 0.0f, 0.0f, 1.0f };
        }
        else
        {
            vertices = { textOrigin.x + (glyph.bearing.x * scale_old.x), textOrigin.y + (glyph.bearing.y * scale_old.y), 0.0f, 0.0f, 0.0f, // Top left
                         textOrigin.x + (glyph.bearing.x * scale_old.x) + (glyph.size.x * scale_old.x), textOrigin.y + (glyph.bearing.y * scale_old.y), 0.0f, 1.0f, 0.0f, // Top right
                         textOrigin.x + (glyph.bearing.x * scale_old.x) + (glyph.size.x * scale_old.x), textOrigin.y + (glyph.bearing.y * scale_old.y) - (glyph.size.y * scale_old.y), 0.0f, 1.0f, 1.0f, // Bottom right
                         textOrigin.x + (glyph.bearing.x * scale_old.x), textOrigin.y + (glyph.bearing.y * scale_old.y) - (glyph.size.y * scale_old.y), 0.0f, 0.0f, 1.0f }; // Bottom left
        }

        //        std::vector<GLfloat> UVs = { 0.0f, 0.0f,
        //                          1.0f, 0.0f,
        //                          1.0f, 1.0f,
        //                          0.0f, 1.0f };

        std::vector<GLuint> indices = { 0, 1, 3,
                                        3, 1, 2 };

        //resources->textShader.use(); // TODO: This statement adds extra safety but possibly unneeded because called in higher levels . . .
        resources->textShader.setTextureImage(juce::gl::GL_TEXTURE_2D, glyph.textureID);
        resources->textQuad.draw();

        textOrigin.x += glyph.advance * scale_old.x;
    }
}
