#pragma once

#include "../../OpenGL/NewGraph.h"
#include "../../Widgets/SimpleLabel.h"
#include "../Window.h"
#include "Ximu3.hpp"

class NewGraphWindow : public Window
{
public:
    NewGraphWindow(const juce::ValueTree& windowLayout_, const juce::Identifier& type_, DevicePanel& devicePanel_, GLRenderer& glRenderer, std::vector<juce::String> legendStrings_, std::vector<juce::Colour> legendColours_);

    ~NewGraphWindow() override;

    void paint(juce::Graphics& g) override;

    void resized() override;

    void mouseWheelMove(const juce::MouseEvent& mouseEvent, const juce::MouseWheelDetails& wheel) override;

    void mouseDown([[maybe_unused]] const juce::MouseEvent& mouseEvent) override;

    void mouseDrag(const juce::MouseEvent& mouseEvent) override;

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

    juce::PopupMenu getMenu();

    void zoomHorizontal(const float multiplier);

    void zoomVertical(const float multiplier);
};
