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
}

Text& GLResources::getGraphAxisValuesText()
{
    const auto fontSize = (unsigned int) juce::roundToInt(12 * context.getRenderingScale());
    if (axisValuesText == nullptr || axisValuesText->getFontSize() != fontSize)
    {
        axisValuesText = std::make_unique<Text>(false);
        axisValuesText->loadFont(BinaryData::MontserratMedium_ttf, BinaryData::MontserratMedium_ttfSize, fontSize);
    }
    return *axisValuesText;
}

Text& GLResources::get3DViewAxisText()
{
    const auto fontSize = (unsigned int) juce::roundToInt(30 * context.getRenderingScale());
    if (axisMarkerText == nullptr || axisMarkerText->getFontSize() != fontSize)
    {
        axisMarkerText = std::make_unique<Text>(true);
        axisMarkerText->loadFont(BinaryData::MontserratMedium_ttf, BinaryData::MontserratMedium_ttfSize, fontSize);
    }
    return *axisMarkerText;
}
