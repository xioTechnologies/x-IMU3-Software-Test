#include "NewGraphWindow.h"
#include "Ximu3.hpp"

class NewGyroscopeGraphWindow : public NewGraphWindow
{
public:
    NewGyroscopeGraphWindow(const juce::ValueTree& windowLayout, const juce::Identifier& type_, DevicePanel& devicePanel_, GLRenderer& glRenderer);

    ~NewGyroscopeGraphWindow() override;

private:
    static juce::ValueTree settingsTree_;

    std::vector<uint64_t> callbackIDs;
    std::function<void(ximu3::XIMU3_InertialMessage)> inertialCallback;
};
