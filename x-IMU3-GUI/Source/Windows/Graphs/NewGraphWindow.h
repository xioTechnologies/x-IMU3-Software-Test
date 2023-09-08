#pragma once

#include "../Window.h"
#include "../../Widgets/SimpleLabel.h"
#include "../../OpenGL/NewGraph.h"
#include "../../Widgets/PopupMenuHeader.h"

class NewGraphWindow : public Window
{
public:
    NewGraphWindow(const juce::ValueTree& windowLayout_, const juce::Identifier& type_, DevicePanel& devicePanel_, GLRenderer& glRenderer, std::vector<juce::String> legendStrings_, std::vector<juce::Colour> legendColours_)
            : Window(devicePanel_, windowLayout_, type_, "", std::bind(&NewGraphWindow::getMenu, this)),
              legendStrings(legendStrings_),
              legendColours(legendColours_),
              graph(glRenderer, legendColours_)
    {
        jassert(legendStrings.size() == legendColours.size());

        addAndMakeVisible(graph);
        addAndMakeVisible(xLabel);
        addAndMakeVisible(yLabel);

        //setMouseCursor(juce::MouseCursor::StandardCursorType::UpDownLeftRightResizeCursor);

        callbackIDs.push_back(devicePanel.getConnection()->addInertialCallback(inertialCallback = [this](auto message)
        {
            if (paused)
            {
                return;
            }

            graph.write(message.timestamp, { message.gyroscope_x, message.gyroscope_y, message.gyroscope_z });
        }));
    }

    ~NewGraphWindow() override
    {
        for (const auto callbackID : callbackIDs)
        {
            devicePanel.getConnection()->removeCallback(callbackID);
        }
    }

    void paint(juce::Graphics& g) override
    {
        if (compactView)
        {
            return;
        }

        // Draw background colour, but do not draw on top of the portions rendered by OpenGL.
        {
            juce::Graphics::ScopedSaveState scopedState(g);
            g.excludeClipRegion(graph.getBounds().withRight(graph.getBounds().getRight() + UILayout::graphRightMargin));
            g.fillAll(UIColours::backgroundLight);
        }

        // Draw legend strings with colors at the top of the graph
        auto settings = graph.getSettings();
        // TODO: Consider combining into one structure: Legend (strings, colours, enabled state)
        if (legendStrings.size() != legendColours.size() || legendStrings.size() != settings.enabledChannels.size())
        {
            return;
        }

        auto bounds = getContentBounds();
        auto legendBounds = bounds.removeFromTop(labelHeight);
        legendBounds.reduce(UILayout::graphRightMargin, 0);
        auto font = UIFonts::getDefaultFont();
        g.setFont(font);
        static constexpr int legendStringGap = 10;

        for (int index = static_cast<int>(legendStrings.size()) - 1; index >= 0; index--)
        {
            auto legendString = legendStrings[(size_t) index];
            g.setColour(settings.enabledChannels[(size_t) index] ? legendColours[(size_t) index] : UIColours::graphText);
            g.drawText(legendString, legendBounds.removeFromRight(font.getStringWidth(legendString) + legendStringGap), juce::Justification::centredRight);
        }
    }

    void resized() override
    {
        Window::resized();

        auto graphArea = getContentBounds();

        compactView = std::min(getWidth(), getHeight()) < 200;

        xLabel.setVisible(compactView == false);
        yLabel.setVisible(compactView == false);

        if (compactView == false)
        {
            xLabel.setBounds(graphArea.removeFromBottom(labelHeight).withTrimmedLeft(labelHeight));

            // Rotate Y label vertical
            auto yLabelBounds = graphArea.removeFromLeft(labelHeight);
            yLabel.setTransform({}); // prevent glitch on subsequent resize
            yLabel.setSize(yLabelBounds.getHeight(), yLabelBounds.getWidth()); // invert width/height
            yLabel.setCentrePosition(0, 0);
            yLabel.setTransform(juce::AffineTransform::rotation(-juce::MathConstants<float>::halfPi).translated(yLabelBounds.getCentreX(), yLabelBounds.getCentreY()));
        }

        graph.setBounds(graphArea);
        graph.setTicksEnabled(compactView == false);
    }

    void mouseWheelMove(const juce::MouseEvent& mouseEvent, const juce::MouseWheelDetails& wheel) override
    {
        if (mouseEvent.mods.isAltDown())
        {
            zoomVertical(std::exp2(wheel.deltaY));
        }
        else
        {
            zoomHorizontal(std::exp2(wheel.deltaY));
        }
    }

    void mouseDown([[maybe_unused]] const juce::MouseEvent& mouseEvent) override
    {
        // Cache graph state for mouseDrag
        graphSettingsMouseCache = graph.getSettings();
        plotWidthJUCEPixelsMouseCache = graph.plotWidthJUCEPixels;
        plotHeightJUCEPixelsMouseCache = graph.plotHeightJUCEPixels;
    }

    void mouseDrag(const juce::MouseEvent& mouseEvent) override
    {
        auto settings = graph.getSettings();
        if (settings.horizontalAutoscale && settings.verticalAutoscale)
        {
            return;
        }

        if (plotWidthJUCEPixelsMouseCache <= 0.0f || plotHeightJUCEPixelsMouseCache <= 0.0f)
        {
            jassertfalse;
            return;
        }

        auto dragOffsetPixels = mouseEvent.position - mouseEvent.mouseDownPosition;

        if (settings.horizontalAutoscale == false)
        {
            auto& cachedXLimits = graphSettingsMouseCache.axesLimits.getXLimits();
            auto xLimitsOffset = -(dragOffsetPixels.x * cachedXLimits.getRange() / plotWidthJUCEPixelsMouseCache);
            settings.axesLimits.setXLimits(cachedXLimits.withOffset(xLimitsOffset));
        }

        if (settings.verticalAutoscale == false)
        {
            auto& cachedYLimits = graphSettingsMouseCache.axesLimits.getYLimits();
            auto yLimitsOffset = dragOffsetPixels.y * cachedYLimits.getRange() / plotHeightJUCEPixelsMouseCache;
            settings.axesLimits.setYLimits(cachedYLimits.withOffset(yLimitsOffset));
        }

        graph.setSettings(settings);
    }

private:
    static constexpr int labelPadding = 8;
    const int labelHeight = juce::roundToInt(UIFonts::getDefaultFont().getHeight()) + labelPadding;

    const std::vector<juce::String> legendStrings;
    const std::vector<juce::Colour> legendColours;

    NewGraph graph;

    SimpleLabel xLabel { "Time (s)", UIFonts::getDefaultFont(), juce::Justification::centred };
    SimpleLabel yLabel { "Degrees Per Second", UIFonts::getDefaultFont(), juce::Justification::centred };

    bool compactView = false;

    std::vector<uint64_t> callbackIDs;
    std::function<void(ximu3::XIMU3_InertialMessage)> inertialCallback;

    const NewGraph::Settings defaultSettings;

    std::atomic<bool> paused = false;

    // Graph state for mouse dragging
    NewGraph::Settings graphSettingsMouseCache;
    float plotWidthJUCEPixelsMouseCache = 1.0f;
    float plotHeightJUCEPixelsMouseCache = 1.0f;

    juce::PopupMenu getMenu()
    {
        juce::PopupMenu menu;

        menu.addItem("Restore Defaults", true, false, [this]
        {
            graph.setSettings(defaultSettings);
        });
        menu.addItem("Clear", true, false, [this]
        {
            graph.clear();
        });
        menu.addItem("Pause", true, paused, [this]
        {
            paused = (paused == false);
        });

        menu.addSeparator();
        menu.addCustomItem(-1, std::make_unique<PopupMenuHeader>("HORIZONTAL"), nullptr);
        menu.addItem("Autoscale", true, graph.getSettings().horizontalAutoscale, [this]
        {
            auto settings = graph.getSettings();
            settings.horizontalAutoscale = !settings.horizontalAutoscale;
            graph.setSettings(settings);
        });
        menu.addItem("Zoom In (Scroll)", graph.getSettings().horizontalAutoscale == false, false, [this]
        {
            zoomHorizontal(0.5f);
        });
        menu.addItem("Zoom Out (Scroll)", graph.getSettings().horizontalAutoscale == false, false, [this]
        {
            zoomHorizontal(2.0f);
        });

        menu.addSeparator();
        menu.addCustomItem(-1, std::make_unique<PopupMenuHeader>("VERTICAL"), nullptr);
        menu.addItem("Autoscale", true, graph.getSettings().verticalAutoscale, [this]
        {
            auto settings = graph.getSettings();
            settings.verticalAutoscale = !settings.verticalAutoscale;
            graph.setSettings(settings);
        });
        menu.addItem("Zoom In (Alt+Scroll)", graph.getSettings().verticalAutoscale == false, false, [this]
        {
            zoomVertical(0.5f);
        });
        menu.addItem("Zoom Out (Alt+Scroll)", graph.getSettings().verticalAutoscale == false, false, [this]
        {
            zoomVertical(2.0f);
        });

        if (legendStrings.size() > 1)
        {
            menu.addSeparator();
            menu.addCustomItem(-1, std::make_unique<PopupMenuHeader>("VIEW"), nullptr);

            auto settings = graph.getSettings();

            bool allTicked = true;
            for (auto const line : settings.enabledChannels)
            {
                allTicked = allTicked && line;
            }
            menu.addItem("All", true, allTicked, [this, allTicked]
            {
                auto settings_ = graph.getSettings();
                for (auto&& line : settings_.enabledChannels)
                {
                    line = !allTicked;
                }
                graph.setSettings(settings_);
                repaint(); // refresh legend text color
            });

            for (size_t index = 0; index < legendStrings.size(); index++)
            {
                menu.addItem(legendStrings[index], true, settings.enabledChannels[index], [this, index]
                {
                    auto settings_ = graph.getSettings();
                    settings_.enabledChannels[index] = settings_.enabledChannels[index] == false;
                    graph.setSettings(settings_);
                    repaint(); // refresh legend text color
                });
            }
        }

        return menu;
    }

    void zoomHorizontal(const float multiplier)
    {
        auto settings = graph.getSettings();
        auto xLimits = settings.axesLimits.getXLimits();
        settings.axesLimits.setXLimits(xLimits.getMin() * multiplier, xLimits.getMax());
        graph.setSettings(settings);
    }

    void zoomVertical(const float multiplier)
    {
        auto settings = graph.getSettings();
        auto yLimits = settings.axesLimits.getYLimits();
        auto range = yLimits.getRange();
        const auto offset = (multiplier * range - range) / 2;
        settings.axesLimits.setYLimits(yLimits.getMin() - offset, yLimits.getMax() + offset);
        graph.setSettings(settings);
    }
};
