#include "../../Convert.h"
#include "NewGyroscopeGraphWindow.h"
#include "../../DevicePanel/DevicePanel.h"

juce::ValueTree NewGyroscopeGraphWindow::settingsTree_("GyroscopeGraphSettings");

NewGyroscopeGraphWindow::NewGyroscopeGraphWindow(const juce::ValueTree& windowLayout, const juce::Identifier& type_, DevicePanel& devicePanel_, GLRenderer& glRenderer)
        : NewGraphWindow(windowLayout, type_, devicePanel_,
                         glRenderer,
                         "Angular velocity (" + degreeSymbol + "/s)",
                         { "X", "Y", "Z" },
                         { UIColours::graphRed, UIColours::graphGreen, UIColours::graphBlue },
                         settingsTree_,
                         true)
{{
    callbackIDs.push_back(devicePanel.getConnection()->addInertialCallback(inertialCallback = [&](auto message)
    {
        write(message.timestamp, { message.gyroscope_x, message.gyroscope_y, message.gyroscope_z });
    }));
}}

NewGyroscopeGraphWindow::~NewGyroscopeGraphWindow()
{
    for (const auto callbackID : callbackIDs)
    {
        devicePanel.getConnection()->removeCallback(callbackID);
    }
}
