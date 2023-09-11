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

    GLuint getFontSize() const;

    GLuint getTotalWidth();

    [[nodiscard]] int getStringWidthGLPixels(const juce::String& string) const; // TODO: Remove [[nodiscard]]

    [[nodiscard]] int getStringWidthJucePixels(const juce::String& string) const;

    float getDescender() const;

    const juce::String& getText() const;

    void setText(const juce::String& text_);

    void setScale(const juce::Point<GLfloat>& scale_);

    // TODO: Unused, maybe we should remove
    void setPosition(const juce::Vector3D<GLfloat>& position_);

    void render(GLResources * const resources);

    void renderScreenSpace(GLResources * const resources, const juce::String& label, const juce::Colour& colour, const glm::mat4& transform);

    // TODO: getFontSizeInJUCEPixels

private:
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

    GLuint fontSize = 0;
    GLuint totalWidth = 0;

    GLfloat descender = 0;

    bool isFirstLetterCentered = false;

    std::map<GLchar, Glyph> alphabet;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Text)
};
