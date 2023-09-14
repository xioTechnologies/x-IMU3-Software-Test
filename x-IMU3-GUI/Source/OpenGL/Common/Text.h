#pragma once

#include "glm/mat4x4.hpp"
#include <juce_gui_basics/juce_gui_basics.h>
#include <map>
#include "Shader.h"
#include <string>

class GLResources;

class Text
{
public:
    explicit Text(const bool isFirstLetterCentered_);

    bool loadFont(const char* data, size_t dataSize, GLuint fontSize_);

    void unloadFont();

    unsigned int getFontSize() const;

    unsigned int getTotalWidth();

    int getStringWidthGLPixels(const juce::String& string) const;

    int getStringWidthJucePixels(const juce::String& string) const;

    float getDescender() const;

    const juce::String& getText() const;

    void setText(const juce::String& text_);

    void setScale(const juce::Point<GLfloat>& scale_);

    // TODO: Unused, maybe we should remove
    void setPosition(const juce::Vector3D<GLfloat>& position_);

    void render(GLResources* const resources);

    void renderScreenSpace(GLResources* const resources, const juce::String& label, const juce::Colour& colour, const glm::mat4& transform);

    // TODO: getFontSizeInJUCEPixels

private:

    // Default FreeType glyph loading uses fractional pixels in the 26.6 fixed point float format called F26Dot6, where 1 unit = 1/64th of a pixel.
    // Freetype glyph units can optionally be represented in raw pixels by calling FT_Load_Char with the FT_LOAD_NO_SCALE flag.
    // Refs: https://freetype.org/freetype2/docs/tutorial/step2.html https://freetype.org/freetype1/docs/api/freetype1.txt
    static float toPixels(float f26Dot6Units)
    {
        return f26Dot6Units / 64.0f;
    }

    class Initializer
    {
    public:
        Initializer();

        ~Initializer();

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Initializer)
    };

    juce::SharedResourcePointer<Initializer> initializer;

    struct Glyph
    {
        GLuint textureID; // texture freetypeTextureID for each letter
        GLuint width; // width of the letter
        GLuint height; // height of the letter
        GLint bearingX; // distance from the y-axis origin
        GLint bearingY; // distance from the x-axis baseline
        GLint advance; // offset to advance to next glyph
    };

    juce::Vector3D<GLfloat> position;

    juce::String text;

    juce::Point<GLfloat> scale = juce::Point<GLfloat>(1.0f, 1.0f);

    unsigned int fontSize = 0;

    float descender = 0;

    bool isFirstLetterCentered = false;

    std::map<GLchar, Glyph> alphabet;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Text)
};
