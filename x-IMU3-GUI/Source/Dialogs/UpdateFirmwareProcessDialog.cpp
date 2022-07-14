#include "../CommandMessage.h"
#include "../Firmware/Firmware.h"
#include "ErrorDialog.h"
#include "UpdateFirmwareProcessDialog.h"
#include "Ximu3Bootloader.h"

UpdateFirmwareProcessDialog::UpdateFirmwareProcessDialog(std::unique_ptr<ximu3::ConnectionInfo> connectionInfo_, const juce::String& hexFile_)
        : Dialog(BinaryData::tools_svg, "Update Firmware Progress", "OK", ""),
          juce::Thread("Update Firmware"),
          connectionInfo(std::move(connectionInfo_)),
          hexFile(hexFile_)
{
    addAndMakeVisible(progressBar);

    setSize(dialogWidth, calculateHeight(1));

    setValid(false);

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

void UpdateFirmwareProcessDialog::updateFirmware()
{
    const auto updateProgress = [&](const auto& progress)
    {
        juce::MessageManager::callAsync([&, progress]
                                        {
                                            progressBar.setTextToDisplay(progress);
                                        });
    };

    // Open connection
    updateProgress("Opening Connection");
    auto connection = std::make_unique<ximu3::Connection>(*connectionInfo);
    if (connection->open() != ximu3::XIMU3_ResultOk)
    {
        updateProgress("Open Connection Failed");
        return;
    }

    // Send bootloader command
    updateProgress("Sending Bootloader Command");
    if (connection->sendCommands({ "{\"bootloader\":null}" }, 2, 500).empty())
    {
        updateProgress("Send Bootloader Command Failed");
        return;
    }

    // Close connection
    connection->close();
    connection.reset();

    // Upload
    juce::Thread::sleep(5000);

    for (const auto& portName : ximu3::SerialDiscovery::getAvailablePorts())
    {
        updateProgress("Attempting " + portName);
        if (XIMU3_upload(hexFile.toRawUTF8(), portName.data()) == 0)
        {
            updateProgress("Upload completed");
            return;
        }
    }

    updateProgress("Upload failed");
}

void UpdateFirmwareProcessDialog::run()
{
    updateFirmware();

    juce::MessageManager::callAsync([&]
                                    {
                                        setValid(true);
                                    });
}
