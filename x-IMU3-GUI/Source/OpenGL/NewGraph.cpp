#include "NewGraph.h"

NewGraph::NewGraph(GLRenderer& renderer_, const std::vector<juce::Colour>& colours_)
        : OpenGLComponent(renderer_.getContext()),
          renderer(renderer_),
          colours(colours_)
{
    renderer.addComponent(*this);

    setSettings(settings);
}

NewGraph::~NewGraph()
{
    renderer.removeComponent(*this);
}

void NewGraph::setSettings(Settings settings_)
{
    std::scoped_lock _(settingsMutex);
    settings = settings_;
}

NewGraph::Settings NewGraph::getSettings() const
{
    std::scoped_lock _(settingsMutex);
    return settings;
}

void NewGraph::setTicksEnabled(const bool enabled)
{
    ticksEnabled = enabled;
}

void NewGraph::clear()
{
    buffer.clear();
}

void NewGraph::write(const uint64_t timestamp, const std::vector<float>& values)
{
    buffer.write(timestamp, values);
}

void NewGraph::render()
{
    std::scoped_lock _(settingsMutex);

    auto bounds = getBoundsInMainWindow();

    // TODO: Could NewBuffer update and read be one operation updateAndRead?
    buffer.update();
    auto channelBuffers = buffer.read();

    settings.axesLimits.autoscale(settings.horizontalAutoscale, settings.verticalAutoscale, channelBuffers, settings.enabledChannels);

    // Paint graph background color
    GLUtil::clear(UIColours::backgroundLight, toOpenGLBounds(bounds));

    if (ticksEnabled)
    {
        // Remove top/right margins from graph plot
        bounds.removeFromRight(UILayout::graphRightMargin);
        bounds.removeFromTop(UILayout::graphTopMargin);

        static constexpr auto xTickMargin = 2;
        static constexpr auto yTickMargin = 5;

        // TODO: This was a bug: renderer.getResources().getGraphAxisValuesText().getFontSize() is using OpenGL font size pixels
        // but here, we are still working in JUCE coordinates (virtualized pixels), so we actually want to use the font size WITHOUT
        // OpenGLContext::getRenderingScale(). Replaced temporarily below . . .
        //auto xTicksBounds = plotBounds.removeFromBottom((int) renderer.getResources().getGraphAxisValuesText().getFontSize()); // font height
        // TODO: Refactor Text to handle both JUCE and GL font sizes
        // TODO: Refactor GLResources getGraphAxisValuesText to return ONE Text object that was created at GL startup.
        //  The Text object should handle knowing both JUCE and GL font sizes instead of requiring reconstruction
        auto xTicksBounds = bounds.removeFromBottom(13); // font height
        bounds.removeFromBottom(xTickMargin);

        const auto yTicks = Ticks::createYTicks(bounds.getHeight(), settings.axesLimits.getYLimits());

        const auto yTicksWidth = getMaximumStringWidth(yTicks, renderer.getResources().getGraphAxisValuesText());
        const auto yTicksBounds = bounds.removeFromLeft(yTicksWidth);
        bounds.removeFromLeft(yTickMargin);

        xTicksBounds.removeFromLeft(yTicksWidth);
        xTicksBounds.removeFromLeft(yTickMargin);

        const auto xTicks = Ticks::createXTicks(bounds.getWidth(), settings.axesLimits.getXLimits());

        drawXTicks(xTicksBounds, yTicksBounds.getX(), settings.axesLimits.getXLimits(), xTicks);
        drawYTicks(yTicksBounds, settings.axesLimits.getYLimits(), yTicks);
        drawPlot(bounds, settings.axesLimits, xTicks, yTicks, channelBuffers, settings.enabledChannels);
    }
    else
    {
        const auto xTicks = Ticks::createXTicks(bounds.getWidth(), settings.axesLimits.getXLimits());
        const auto yTicks = Ticks::createYTicks(bounds.getHeight(), settings.axesLimits.getYLimits());
        drawPlot(bounds, settings.axesLimits, xTicks, yTicks, channelBuffers, settings.enabledChannels);
    }

    plotWidthJUCEPixels = (float) bounds.getWidth();
    plotHeightJUCEPixels = (float) bounds.getHeight();
}

void NewGraph::drawPlot(const juce::Rectangle<int>& bounds, const AxesLimits& limits, const Ticks& xTicks, const Ticks& yTicks, const std::vector<std::span<juce::Point<GLfloat>>>& channelBuffers, const std::vector<bool>& enabledChannels)
{
    // Set rendering bounds
    auto glBounds = toOpenGLBounds(bounds);
    GLUtil::ScopedCapability scopedScissor(juce::gl::GL_SCISSOR_TEST, true);
    GLUtil::viewportAndScissor(glBounds);

    // Setup OpenGL state
    GLUtil::ScopedCapability scopedCull(juce::gl::GL_CULL_FACE, false);
    GLUtil::ScopedCapability scopedDepth(juce::gl::GL_DEPTH_TEST, false);

    /*  NOTE: We are using OpenGL's built-in line rendering system, GL_LINES. By using this system, we are
     *  limited to drawing lines with a width of 1 pixel. The OpenGL spec only requires graphics drivers
     *  implement line widths of 1 pixel, allowing optional implementation of more widths. To have consistent
     *  line drawing behavior across devices, we can only really use glLineWidth of 1.0.
     *
     *  If we want to increase line width in the future, we will need to create our own line system using
     *  elongated quads.
     *
     *  Refs:
     *  - https://registry.khronos.org/OpenGL-Refpages/gl4/html/glLineWidth.xhtml "Only width 1 is guaranteed to be supported" and this is often the case.
     *  - https://mattdesl.svbtle.com/drawing-lines-is-hard
     */
    juce::gl::glLineWidth(1.0f);

    // Draw
    drawGrid(limits, xTicks, yTicks);
    drawData(limits, channelBuffers, enabledChannels);
}

void NewGraph::drawGrid(const AxesLimits& limits, const Ticks& xTicks, const Ticks& yTicks)
{
    auto addGridLines = [](std::vector<GLfloat>& linesToAddTo, bool areVertical, const Ticks& ticks, const AxisLimits& axisLimits)
    {
        if (ticks.major <= 0.0 || ticks.minorPerMajor <= 0)
        {
            jassertfalse;
            return;
        }

        // Add line to grid based on position in graph units
        auto addLine = [&](float position, bool isMajorTick)
        {
            float tickBrightness = isMajorTick ? majorTickColorBrightness : minorTickColorBrightness;
            const float ndcPosition = engineeringValueToNDC(position, axisLimits);
            if (areVertical)
            {
                linesToAddTo.insert(linesToAddTo.end(), { ndcPosition, -1.0f, tickBrightness, ndcPosition, 1.0f, tickBrightness });
            }
            else
            {
                linesToAddTo.insert(linesToAddTo.end(), { -1.0f, ndcPosition, tickBrightness, 1.0f, ndcPosition, tickBrightness });
            }
        };

        const float minorDistance = ticks.major / (float) ticks.minorPerMajor;

        // Major and minor ticks from first major position and greater
        const float firstMajorPosition = GLUtil::roundUpToNearestMultiple(axisLimits.getMin(), ticks.major);
        const auto maxPossibleMajorTickCount = static_cast<unsigned int> (std::floor(axisLimits.getRange() / ticks.major)) + 1;
        for (unsigned int majorTickIndex = 0; majorTickIndex < maxPossibleMajorTickCount; majorTickIndex++)
        {
            const float majorPosition = firstMajorPosition + (float) majorTickIndex * ticks.major;
            if (majorPosition > axisLimits.getMax())
            {
                break;
            }

            addLine(majorPosition, true);

            for (unsigned int minorTickIndex = 1; minorTickIndex < ticks.minorPerMajor; minorTickIndex++)
            {
                const float minorPosition = majorPosition + (float) minorTickIndex * minorDistance;
                if (minorPosition > axisLimits.getMax())
                {
                    break;
                }
                addLine(minorPosition, false);
            }
        }

        // Minor ticks prior to first major position
        for (unsigned int minorTickIndex = 1; minorTickIndex < ticks.minorPerMajor; minorTickIndex++)
        {
            const float minorPosition = firstMajorPosition - (float) minorTickIndex * minorDistance;
            if (minorPosition < axisLimits.getMin())
            {
                break;
            }
            addLine(minorPosition, false);
        }
    };

    // Compute line vertices for LineBuffer
    std::vector<GLfloat> lines;
    addGridLines(lines, true, xTicks, limits.getXLimits()); // vertical x ticks
    addGridLines(lines, false, yTicks, limits.getYLimits()); // horizontal y ticks

    // Border lines
    lines.insert(lines.end(), {
            -1.0f, -1.0f, borderColorBrightness, -1.0f, 1.0f, borderColorBrightness, // left edge
            1.0f, -1.0f, borderColorBrightness, 1.0f, 1.0f, borderColorBrightness, // right edge
            -1.0f, 1.0f, borderColorBrightness, 1.0f, 1.0f, borderColorBrightness, // top edge
            -1.0f, -1.0f, borderColorBrightness, 1.0f, -1.0f, borderColorBrightness // bottom edge
    });

    // Draw lines
    GLUtil::ScopedCapability scopedLineSmooth(juce::gl::GL_LINE_SMOOTH, false); // provides sharper horizontal/vertical lines

    auto& newGraphGridShader = resources.newGraphGridShader;
    newGraphGridShader.use();

    auto& gridBuffer = resources.graphGridBuffer;
    gridBuffer.fillBuffers(lines);
    gridBuffer.draw(juce::gl::GL_LINES);
}

void NewGraph::drawData(const AxesLimits& limits, const std::vector<std::span<juce::Point<GLfloat>>>& channelBuffers, const std::vector<bool>& enabledChannels)
{
    if ((channelBuffers.size() != enabledChannels.size()) || (channelBuffers.size() != colours.size()))
    {
        jassertfalse;
        return;
    }

    resources.newGraphDataShader.use();

    for (size_t index = 0; index < channelBuffers.size(); index++)
    {
        if (enabledChannels[index] == false)
        {
            continue;
        }

        // Convert to raw floats for OpenGL buffer
        std::vector<GLfloat> lines;
        for (const auto& point : channelBuffers[index])
        {
            float xNDC = engineeringValueToNDC(point.x, limits.getXLimits());
            float yNDC = engineeringValueToNDC(point.y, limits.getYLimits());
            lines.insert(lines.end(), { xNDC, yNDC });
        }

        resources.newGraphDataShader.colour.setRGBA(colours[index]);
        resources.newGraphDataBuffer.fillBuffers(lines);
        resources.newGraphDataBuffer.draw(juce::gl::GL_LINE_STRIP);
    }
}

void NewGraph::drawTicks(bool isXTicks, const juce::Rectangle<int>& plotBounds, const juce::Rectangle<int>& drawBounds, const AxisLimits& limits, const Ticks& ticks)
{
    // Set rendering bounds, expanded to allow drawing past graph edges
    auto glPlotBounds = toOpenGLBounds(plotBounds); // only plot area
    auto glDrawBounds = toOpenGLBounds(drawBounds); // full area allowed to draw text
    GLUtil::ScopedCapability scopedScissor(juce::gl::GL_SCISSOR_TEST, true);
    GLUtil::viewportAndScissor(glDrawBounds);

    // Setup OpenGL state
    GLUtil::ScopedCapability scopedCull(juce::gl::GL_CULL_FACE, false); // TODO: Why is this necessary??
    GLUtil::ScopedCapability scopedDepthTest(juce::gl::GL_DEPTH_TEST, false); // do not hide text based on depth

    auto& text = resources.getGraphAxisValuesText();
    const int distanceOfPlotAxis = isXTicks ? glPlotBounds.getWidth() : glPlotBounds.getHeight();
    const int plotStartOffset = isXTicks ? glPlotBounds.getX() - glDrawBounds.getX() : glPlotBounds.getY() - glDrawBounds.getY();
    auto labelsToDraw = ticks.labels;

    // For X-axis, hide tick labels that extend out of bounds or overlap
    if (isXTicks)
    {
        auto getLabelEdges = [&](const Ticks::Label& label) -> std::tuple<float, float>
        {
            const auto centreX = juce::jmap<float>(label.value, limits.getMin(), limits.getMax(), 0.0f, (float) distanceOfPlotAxis) + (float) plotStartOffset + (float) glDrawBounds.getX();
            const auto leftEdgeX = centreX - ((float) text.getStringWidthGLPixels(label.text) / 2.0f);
            const auto rightEdgeX = centreX + ((float) text.getStringWidthGLPixels(label.text) / 2.0f);
            return { leftEdgeX, rightEdgeX };
        };

        // Remove any tick text which would extend past the edges of the drawing bounds
        labelsToDraw.erase(std::remove_if(labelsToDraw.begin(), labelsToDraw.end(), [&](auto& label)
        {
            auto [leftEdgeX, rightEdgeX] = getLabelEdges(label);
            return leftEdgeX < (float) glDrawBounds.getX() || rightEdgeX > (float) glDrawBounds.getRight();
        }), labelsToDraw.end());

        auto areAnyLabelsTooClose = [&]
        {
            // Check each pair of labels to see if they are too close
            constexpr float minimumSpaceBetweenLabels = 8.0f;
            for (size_t index = 0; index < labelsToDraw.size() - 1; index++)
            {
                auto [label1LeftEdge, label1RightEdge] = getLabelEdges(labelsToDraw[index]);
                auto [label2LeftEdge, label2RightEdge] = getLabelEdges(labelsToDraw[index + 1]);
                if (label1RightEdge + minimumSpaceBetweenLabels > label2LeftEdge)
                {
                    return true;
                }
            }

            return false;
        };

        // If labels are too close, remove every other label until no overlaps
        while (labelsToDraw.size() > 1 && areAnyLabelsTooClose())
        {
            // Erase every other element, except for "0" if it is displayed
            labelsToDraw.erase(std::remove_if(labelsToDraw.begin(), labelsToDraw.end(), [&](auto& label)
            {
                return ((&label - &*labelsToDraw.begin()) % 2) && label.text != "0";
            }), labelsToDraw.end());
        }
    }

    // Draw each text string
    for (const auto& label : labelsToDraw)
    {
        const auto offsetAlongAxis = juce::jmap<float>(label.value, limits.getMin(), limits.getMax(), 0.0f, (float) distanceOfPlotAxis) + (float) plotStartOffset;
        const auto offsetTowardsAxis = isXTicks ? (float) (glDrawBounds.getHeight() - (int) text.getFontSize()) : (float) glDrawBounds.getWidth();

        const auto x = isXTicks ? offsetAlongAxis : offsetTowardsAxis;
        const auto y = isXTicks ? offsetTowardsAxis : offsetAlongAxis;

        drawText(resources, glDrawBounds, text, label.text, UIColours::graphText, x, y, isXTicks ? juce::Justification::horizontallyCentred : juce::Justification::centredRight);
    }
}

void NewGraph::drawXTicks(const juce::Rectangle<int>& bounds, int yTicksLeftEdge, const AxisLimits& limits, const Ticks& ticks)
{
    // Expand drawing bounds to allow text to be drawn past the corners of the plot.
    auto drawBounds = bounds.withRight(bounds.getRight() + UILayout::graphRightMargin);
    drawBounds.setLeft(yTicksLeftEdge);
    drawTicks(true, bounds, drawBounds, limits, ticks);
}

void NewGraph::drawYTicks(const juce::Rectangle<int>& bounds, const AxisLimits& limits, const Ticks& ticks)
{
    // Expand drawing bounds to allow text to be drawn past the corners of the plot.
    auto drawBounds = bounds.expanded(0, UILayout::graphTopMargin);
    drawTicks(false, bounds, drawBounds, limits, ticks);
}

void NewGraph::drawText(GLResources& resources, const juce::Rectangle<int>& openGLBounds, Text& text, const juce::String& label, const juce::Colour& colour, float x, float y, juce::Justification justification)
{
    resources.textShader.use();
    resources.textShader.colour.setRGBA(colour);
    text.setText(label);

    // NOTE: The 2.0 / width here and -1.0 offset in the translation matrix below are specifically translating to NDC coordinates via math
    // computed in the shader. We could consider moving this math to one place here, not in the shader, so it is more obvious.
    const juce::Point<GLfloat> pixelSize(2.0f / (GLfloat) openGLBounds.getWidth(), 2.0f / (GLfloat) openGLBounds.getHeight());
    text.setScale({ pixelSize.x, pixelSize.y });

    if (justification.testFlags(juce::Justification::horizontallyCentred))
    {
        x -= (GLfloat) text.getTotalWidth() / 2.0f;
    }
    else if (justification.testFlags(juce::Justification::right))
    {
        x -= (GLfloat) text.getTotalWidth();
    }

    if (justification.testFlags(juce::Justification::verticallyCentred))
    {
        const auto offset = (GLfloat) text.getFontSize() / 2.0f + text.getDescender();
        y -= offset;
    }

    // NOTE: Translate position to NDC in -1.0 to 1.0 range
    auto translation = juce::Matrix3D<float>::fromTranslation(juce::Vector3D<float>(-1 + (x * pixelSize.x), -1 + (y * pixelSize.y), 0.0f));

    resources.textShader.transformation.setMatrix4(translation.mat, 1, false);

    text.render(resources);
}

float NewGraph::engineeringValueToNDC(float value, const AxisLimits& axisLimits)
{
    return (((value - axisLimits.getMin()) / axisLimits.getRange()) * 2.0f) - 1.0f;
}

int NewGraph::getMaximumStringWidth(const Ticks& ticks, const Text& text)
{
    int maxStringWidth = 0;
    for (const auto& label : ticks.labels)
    {
        maxStringWidth = std::max(maxStringWidth, text.getStringWidthJucePixels(label.text));
    }
    return maxStringWidth;
}
