#pragma once

#include "../../OpenGL/NewGraph.h"
#include "../../Widgets/SimpleLabel.h"
#include "../Window.h"
#include "Ximu3.hpp"

class NewGraphWindow : public Window, private juce::ValueTree::Listener
{
public:
    NewGraphWindow(const juce::ValueTree& windowLayout_, const juce::Identifier& type_, DevicePanel& devicePanel_,
                   GLRenderer& glRenderer,
                   const juce::String& yAxis,
                   std::vector<juce::String> legendStrings_,
                   std::vector<juce::Colour> legendColours_,
                   juce::ValueTree settingsTree_,
                   bool defaultHorizontalAutoscale_);

    void paint(juce::Graphics& g) override;

    void resized() override;

    void mouseWheelMove(const juce::MouseEvent& mouseEvent, const juce::MouseWheelDetails& wheel) override;

    void mouseDown([[maybe_unused]] const juce::MouseEvent& mouseEvent) override;

    void mouseDrag(const juce::MouseEvent& mouseEvent) override;

protected:
    void write(const uint64_t timestamp, const std::vector<float>& arguments);

    static const juce::String degreeSymbol;

private:
    const std::vector<juce::String> legendStrings;
    const std::vector<juce::Colour> legendColours;
    const bool defaultHorizontalAutoscale;
    juce::ValueTree settingsTree;
    const int numberOfChannels = (int) legendStrings.size();

    NewGraph graph;

    SimpleLabel xLabel { "Time (s)", UIFonts::getDefaultFont(), juce::Justification::centred };
    SimpleLabel yLabel;

    bool paused = settingsTree["paused"];

    bool compactView = false;

    // Graph state for mouse dragging
    NewGraph::Settings graphSettingsMouseCache;
    float plotWidthJUCEPixelsMouseCache = 1.0f;
    float plotHeightJUCEPixelsMouseCache = 1.0f;

    void writeToValueTree(const NewGraph::Settings& settings);

    NewGraph::Settings readFromValueTree() const;

    void zoomHorizontal(const float multiplier);

    void zoomVertical(const float multiplier);

    juce::PopupMenu getMenu();

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;
};
