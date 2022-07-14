#include "../ApplicationSettings.h"
#include "../Firmware/Firmware.h"
#include <filesystem>
#include "UpdateFirmwareDialog.h"

UpdateFirmwareDialog::UpdateFirmwareDialog() : Dialog(BinaryData::tools_svg, "Update Firmware", "Update")
{
    addAndMakeVisible(deviceLabel);
    addAndMakeVisible(deviceValue);
    addAndMakeVisible(hexFileLabel);
    addAndMakeVisible(hexFileValue);
    addAndMakeVisible(hexFileButton);

    const auto hexFile = ApplicationSettings::getDirectory().getChildFile(Firmware::fileName);
    hexFile.replaceWithData(Firmware::memoryBlock.getData(), Firmware::memoryBlock.getSize());
    hexFileValue.setText(hexFile.getFullPathName());

    hexFileButton.onClick = [&]
    {
        juce::FileChooser fileChooser(hexFileButton.getTooltip(), std::filesystem::exists(hexFileValue.getText().toStdString()) ? hexFileValue.getText() : "", "*.hex");
        if (fileChooser.browseForFileToOpen())
        {
            hexFileValue.setText(fileChooser.getResult().getFullPathName());
        }
    };

    deviceValue.onChange = hexFileValue.onTextChange = [&]
    {
        setValid(getConnectionInfo() != nullptr && std::filesystem::exists(hexFileValue.getText().toStdString()));
    };
    setValid(false);

    setSize(dialogWidth, calculateHeight(2));
}

void UpdateFirmwareDialog::resized()
{
    Dialog::resized();

    auto bounds = getContentBounds();
    auto portRow = bounds.removeFromTop(UILayout::textComponentHeight);
    deviceLabel.setBounds(portRow.removeFromLeft(columnWidth));
    deviceValue.setBounds(portRow);

    bounds.removeFromTop(Dialog::margin);

    auto sourceRow = bounds.removeFromTop(UILayout::textComponentHeight);
    hexFileLabel.setBounds(sourceRow.removeFromLeft(columnWidth));
    hexFileButton.setBounds(sourceRow.removeFromRight(50));
    hexFileValue.setBounds(sourceRow.withTrimmedRight(margin));
}

std::unique_ptr<ximu3::ConnectionInfo> UpdateFirmwareDialog::getConnectionInfo() const
{
    if (deviceValue.getSelectedId() == 0)
    {
        return nullptr;
    }

    const auto& device = devices[(size_t) deviceValue.getSelectedId() - 1];
    switch (device.connection_type)
    {
        case ximu3::XIMU3_ConnectionTypeUsb:
            return std::make_unique<ximu3::UsbConnectionInfo>(device.usb_connection_info);
        case ximu3::XIMU3_ConnectionTypeSerial:
            return std::make_unique<ximu3::SerialConnectionInfo>(device.serial_connection_info);
        case ximu3::XIMU3_ConnectionTypeBluetooth:
        case ximu3::XIMU3_ConnectionTypeTcp:
        case ximu3::XIMU3_ConnectionTypeUdp:
        case ximu3::XIMU3_ConnectionTypeFile:
            return nullptr;
    }
}

juce::String UpdateFirmwareDialog::getHexFile() const
{
    return hexFileValue.getText();
}
