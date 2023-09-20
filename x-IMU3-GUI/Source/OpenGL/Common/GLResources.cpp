#include "GLResources.h"

GLResources::GLResources(juce::OpenGLContext& context_) : context(context_)
{
    auto unzipObjAndMtl = [](const char* data, const int size, const juce::String& internalFileName)
    {
        juce::MemoryInputStream stream(data, (size_t) size, false);
        auto zipFile = juce::ZipFile(stream);

        auto* objFile = zipFile.getEntry(internalFileName + ".obj");
        auto* mtlFile = zipFile.getEntry(internalFileName + ".mtl");

        juce::String objectString = objFile ? std::unique_ptr<juce::InputStream>(zipFile.createStreamForEntry(*objFile))->readEntireStreamAsString() : "";
        juce::String materialString = mtlFile ? std::unique_ptr<juce::InputStream>(zipFile.createStreamForEntry(*mtlFile))->readEntireStreamAsString() : "";

        return std::tuple<juce::String, juce::String> { objectString, materialString };
    };

    const auto& [boardObj, boardMtl] = unzipObjAndMtl(BinaryData::xIMU3_Board_zip, BinaryData::xIMU3_Board_zipSize, "x-IMU3 Board");
    board.setModel(boardObj, boardMtl);

    const auto& [housingObj, housingMtl] = unzipObjAndMtl(BinaryData::xIMU3_Housing_zip, BinaryData::xIMU3_Housing_zipSize, "x-IMU3 Housing");
    housing.setModel(housingObj, housingMtl);

    arrow.setModel(BinaryData::Arrow_obj, "");

    compassTexture.loadImage(juce::ImageFileFormat::loadFrom(BinaryData::Compass_png, BinaryData::Compass_pngSize));

    graphTickText = std::make_unique<Text>();
    threeDViewAxesText = std::make_unique<Text>();
}

Text& GLResources::getGraphTickText()
{
    const auto fontSize = (unsigned int) juce::roundToInt(12 * context.getRenderingScale());
    if (graphTickText->getFontSize() != fontSize)
    {
        graphTickText->loadFont(BinaryData::MontserratMedium_ttf, BinaryData::MontserratMedium_ttfSize, fontSize);
    }
    return *graphTickText;
}

Text& GLResources::get3DViewAxesText()
{
    const auto fontSize = (unsigned int) juce::roundToInt(30 * context.getRenderingScale());
    if (threeDViewAxesText->getFontSize() != fontSize)
    {
        threeDViewAxesText->loadFont(BinaryData::MontserratMedium_ttf, BinaryData::MontserratMedium_ttfSize, fontSize);
    }
    return *threeDViewAxesText;
}
