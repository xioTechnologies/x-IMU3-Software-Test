#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>
#include "Text.h"
#include "TextQuad.h"
#include "OpenGL/Graph/LineBuffer.h"
#include "OpenGL/Shaders/GraphDataShader.h"
#include "OpenGL/Shaders/GraphGridShader.h"
#include "OpenGL/Shaders/LitShader.h"
#include "OpenGL/Shaders/GraphDataShader.h"
#include "OpenGL/Shaders/GraphGridShader.h"
#include "OpenGL/Shaders/ScreenSpaceLitShader.h"
#include "OpenGL/Shaders/TextShader.h"
#include "OpenGL/Shaders/UnlitShader.h"
#include "OpenGL/Shaders/WorldGridShader.h"
#include "OpenGL/ThreeDView/Model.h"
#include "OpenGL/ThreeDView/OrbitCamera.h"
#include "OpenGL/ThreeDView/PlaneModel.h"

class GLResources
{
    juce::OpenGLContext& context;

public:
    static constexpr int graphBufferSize = 1 << 16;

    explicit GLResources(juce::OpenGLContext& context_);

    // Text
    Text& getGraphAxisValuesText();
    Text& get3DViewAxisText();

    LineBuffer graphGridBuffer { true };
    LineBuffer graphDataBuffer { false };

    // Geometry
    Model arrow { context };
    Model board { context };
    Model housing { context };
    Model custom { context };
    PlaneModel plane;
    TextQuad textQuad;

    // Shaders
    const GraphDataShader graphDataShader { context };
    const GraphGridShader graphGridShader { context };
    const GraphGridShader gridLinesShader { context };
    const WorldGridShader grid3DShader { context };
    const TextShader textShader { context };
    const LitShader threeDViewShader { context };
    const UnlitShader unlitShader { context };
    const ScreenSpaceLitShader screenSpaceLitShader { context };

    // Textures
    juce::OpenGLTexture compassTexture;

    OrbitCamera orbitCamera;

private:
    std::unique_ptr<Text> infoText;
    std::unique_ptr<Text> legendText;
    std::unique_ptr<Text> axisValuesText;
    std::unique_ptr<Text> axisLabelText;
    std::unique_ptr<Text> axisMarkerText;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GLResources)
};
