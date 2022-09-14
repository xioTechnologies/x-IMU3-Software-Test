#include "../x-IMU3-API/Cpp/Ximu3.hpp"
#include "../x-IMU3-Bootloader/Ximu3Bootloader.h"
#include <chrono>
#include <iostream>
#include <thread>

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Missing first argument (device name)" << std::endl;
        return 1;
    }

    if (argc < 3)
    {
        std::cout << "Missing second argument (file name)" << std::endl;
        return 1;
    }

    const char* deviceName = argv[1];
    const char* fileName = argv[2];

    if (argc >= 4 && strcmp(argv[3], "-b") == 0)
    {
        // Search for connections
        std::cout << "Searching for connections" << std::endl;
        const auto devices = ximu3::SerialDiscovery::scan(2000);
        if (devices.empty())
        {
            std::cout << "No connections available" << std::endl;
            return 1;
        }

        // Open connection
        const ximu3::UsbConnectionInfo connectionInfo(devices[0].usb_connection_info);
        std::cout << "Connecting to " << connectionInfo.toString() << std::endl;
        ximu3::Connection connection(connectionInfo);
        if (connection.open() != ximu3::XIMU3_ResultOk)
        {
            std::cout << "Unable to open connection" << std::endl;
            return 1;
        }

        // Send bootloader command
        std::cout << "Sending bootloader command" << std::endl;
        if (connection.sendCommands({ "{\"bootloader\":null}" }, 2, 500).empty())
        {
            std::cout << "Unable to confirm bootloader command" << std::endl;
            return 1;
        }

        // Close connection
        connection.close();

        // Upload
        std::cout << "Waiting for bootloader mode" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    for (const auto& port : ximu3::SerialDiscovery::getAvailablePorts())
    {
        std::cout << "Attempting upload on " + port << std::endl;

        if (XIMU3_upload_firmware(deviceName, fileName, port.data()) == 0)
        {
            std::cout << "Update complete" << std::endl;
            return 0;
        }
    }

    std::cout << "Firmware update failed" << std::endl;
    return 1;
}
