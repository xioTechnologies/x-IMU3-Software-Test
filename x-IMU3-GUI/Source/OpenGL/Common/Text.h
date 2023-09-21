#pragma once

#include "glm/mat4x4.hpp"
#include <juce_gui_basics/juce_gui_basics.h>
#include <unordered_map>
#include "Shader.h"
#include <string>
#include "freetype/freetype.h"

class GLResources;

class Text
{
public:
    explicit Text(const bool isFirstLetterCentered_);
    virtual ~Text();

    bool loadFont(const char* data, size_t dataSize, int fontSizeJucePixels_);
    void unloadFont();

    int getFontSizeGLPixels() const;
    int getFontSizeJucePixels() const;

    float getTotalWidth();

    int getStringWidthGLPixels(const juce::String& string) const;

    int getStringWidthJucePixels(const juce::String& string) const;

    float getDescender() const;

    const juce::String& getText() const;

    void setText(const juce::String& text_);

    void setScale(const juce::Point<GLfloat>& scale_);

    // TODO: Unused, maybe we should remove
    void setPosition(const juce::Vector3D<GLfloat>& position_);

    void render(GLResources* const resources);

    void render(GLResources* const resources, const juce::String& text_, glm::vec2 screenPosition, juce::Rectangle<int> viewport, const juce::Colour& colour, juce::Justification justification);

    void renderScreenSpace(GLResources* const resources, const juce::String& label, const juce::Colour& colour, const glm::mat4& transform, juce::Rectangle<int> viewportBounds);

    static int toGLPixels(int jucePixels);

private:

    // Default FreeType glyph loading for some data uses fractional pixels in the 26.6 fixed point float format, F26Dot6, where 1 unit = 1/64th of a pixel.
    // Freetype glyph units can optionally be represented in raw pixels by calling FT_Load_Char with the FT_LOAD_NO_SCALE flag.
    // Refs: https://freetype.org/freetype2/docs/tutorial/step2.html https://freetype.org/freetype1/docs/api/freetype1.txt
    static float toPixels(float f26Dot6Units)
    {
        return f26Dot6Units / 64.0f;
    }

    class FreeTypeLibrary
    {
    public:
        FreeTypeLibrary();

        ~FreeTypeLibrary();

        FT_Library get();

    private:
        FT_Library freetypeLibrary = nullptr;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FreeTypeLibrary)
    };

    juce::SharedResourcePointer<FreeTypeLibrary> freeTypeLibrary;

    struct Glyph
    {
        GLuint textureID; // texture freetypeTextureID for each letter
        glm::ivec2 size; // width/height of glyph
        glm::ivec2 bearing; // offset from origin to top left of glyph
        float advance; // offset to advance to next glyph in pixels
    };

    juce::Vector3D<GLfloat> position;

    juce::String text;

    juce::Point<GLfloat> scale_old = juce::Point<GLfloat>(1.0f, 1.0f);

    unsigned int fontSizeGLPixels = 0;
    int fontSizeJucePixels = 0;

    float descender = 0;

    bool isFirstLetterCentered = false;

    std::unordered_map<unsigned char, Glyph> alphabet;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Text)
};
