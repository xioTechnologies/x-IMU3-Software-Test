#pragma once

#include "../CustomLookAndFeel.h"
#include "Common/GLRenderer.h"
#include "Common/GLUtil.h"
#include "NewGraph/AxesLimits.h"
#include "NewGraph/NewBuffer.h"
#include "NewGraph/Ticks.h"
#include "OpenGLComponent.h"

class NewGraph : public OpenGLComponent
{
public:
    struct Settings
    {
        AxesLimits axesLimits;
        bool horizontalAutoscale = false;
        bool verticalAutoscale = false;
        std::vector<bool> enabledChannels;
    };

    NewGraph(GLRenderer& renderer_, const std::vector<juce::Colour>& colours);

    ~NewGraph() override;

    void render() override;

    void setSettings(const Settings& settings_);

    Settings getSettings() const;

    void setTicksEnabled(const bool enabled);

    void clear();

    void write(const uint64_t timestamp, const std::vector<float>& values);

    // Plot size info for NewGraphWindow mouse drag functionality
    std::atomic<float> plotWidthJUCEPixels = 0.0f;
    std::atomic<float> plotHeightJUCEPixels = 0.0f;

private:
    GLRenderer& renderer;

    mutable std::mutex settingsMutex;
    Settings settings;

    NewBuffer buffer;

    std::atomic<bool> ticksEnabled { false };

    const std::vector<juce::Colour> legendColours;

    static constexpr GLfloat majorTickColorBrightness = 0.65f;
    static constexpr GLfloat minorTickColorBrightness = 0.45f;
    static constexpr GLfloat borderColorBrightness = 0.65f;

    static float engineeringValueToNDC(float value, const AxisLimits& axisLimits); // map to OpenGL Normalized Device Coordinates (NDC) with values ranging from -1.0 to 1.0

    void drawPlot(const juce::Rectangle<int>& bounds, const AxesLimits& limits, const Ticks& xTicks, const Ticks& yTicks, const std::vector<std::span<juce::Point<GLfloat>>>& channelBuffers, const std::vector<bool>& enabledChannels);

    void drawGrid(const AxesLimits& limits, const Ticks& xTicks, const Ticks& yTicks);

    void drawData(const AxesLimits& limits, const std::vector<std::span<juce::Point<GLfloat>>>& channelBuffers, const std::vector<bool>& enabledChannels);

    void drawXTicks(const juce::Rectangle<int>& bounds, int yTicksLeftEdge, const AxisLimits& limits, const Ticks& ticks);

    void drawYTicks(const juce::Rectangle<int>& bounds, const AxisLimits& limits, const Ticks& ticks);

    void drawTicks(bool isXTicks, const juce::Rectangle<int>& plotBounds, const juce::Rectangle<int>& drawBounds, const AxisLimits& limits, const Ticks& ticks);

    static void drawText(GLResources& resources, const juce::Rectangle<int>& openGLBounds, Text& text, const juce::String& label, const juce::Colour& colour, float x, float y, juce::Justification justification);

    static int getMaximumStringWidth(const Ticks& ticks, const Text& text);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NewGraph)
};
