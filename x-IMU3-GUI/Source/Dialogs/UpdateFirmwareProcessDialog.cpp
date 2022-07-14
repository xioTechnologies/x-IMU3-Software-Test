#include "../CommandMessage.h"
#include "../Firmware/Firmware.h"
#include "ErrorDialog.h"
#include "UpdateFirmwareProcessDialog.h"
#include "Ximu3Bootloader.h"

void updateFirmware(ximu3::ConnectionInfo& connectionInfo, const std::string& hexFile, std::function<void(const std::string&)> callback)
{
    // Open connection
    callback("Opening Connection");
    auto connection = std::make_unique<ximu3::Connection>(connectionInfo);
    if (connection->open() != ximu3::XIMU3_ResultOk)
    {
        callback("Open Connection Failed");
        return;
    }

    // Send bootloader command
    callback("Sending Bootloader Command");
    if (connection->sendCommands({ "{\"bootloader\":null}" }, 2, 500).empty())
    {
        callback("Send Bootloader Command Failed");
        return;
    }

    // Close connection
    connection->close();
    connection.reset();

    // Upload
    juce::Thread::sleep(5000);

    for (const auto& portName : ximu3::SerialDiscovery::getAvailablePorts())
    {
        callback("Attempting " + portName);
        if (XIMU3_upload(hexFile.data(), portName.data()) == 0)
        {
            callback("Upload completed");
            return;
        }
    }

    callback("Upload failed");
}

UpdateFirmwareProcessDialog::UpdateFirmwareProcessDialog(std::unique_ptr<ximu3::ConnectionInfo> connectionInfo_, const juce::String& hexFile_)
        : Dialog(BinaryData::tools_svg, "Update Firmware Progress", "Cancel", ""),
          juce::Thread("Update Firmware"),
          connectionInfo(std::move(connectionInfo_)),
          hexFile(hexFile_)
{
    addAndMakeVisible(progressBar);

    setSize(dialogWidth, calculateHeight(1));

    startThread();
}

UpdateFirmwareProcessDialog::~UpdateFirmwareProcessDialog()
{
    stopThread(10000);
}

void UpdateFirmwareProcessDialog::resized()
{
    Dialog::resized();

    progressBar.setBounds(getContentBounds().removeFromTop(UILayout::textComponentHeight));
}

void UpdateFirmwareProcessDialog::run()
{
    std::cout << " UpdateFirmwareProcessDialog::run    start" << std::endl;

    updateFirmware(*connectionInfo, hexFile.toStdString(), [&](const auto& progress)
    {
        juce::MessageManager::callAsync([&, self = SafePointer<juce::Component>(this), progress]
                                        {
                                            if (self != nullptr)
                                            {
                                                progressBar.setTextToDisplay(progress);
                                            }
                                        });
    });

    std::cout << " UpdateFirmwareProcessDialog::run    end" << std::endl;
}
