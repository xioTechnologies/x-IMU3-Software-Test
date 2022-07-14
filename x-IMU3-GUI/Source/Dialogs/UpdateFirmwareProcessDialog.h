#pragma once

#include "Dialog.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include "Ximu3.hpp"

class UpdateFirmwareProcessDialog : public Dialog, private juce::Thread
{
public:
    UpdateFirmwareProcessDialog(std::unique_ptr<ximu3::ConnectionInfo> connectionInfo_, const juce::String& hexFile_);

    ~UpdateFirmwareProcessDialog() override;

    void resized() override;

private:
    const std::unique_ptr<ximu3::ConnectionInfo> connectionInfo;
    const juce::String hexFile;

    double progressBarValue = 0.0;
    juce::ProgressBar progressBar { progressBarValue };

    void updateFirmware();

    void run() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UpdateFirmwareProcessDialog)
};
