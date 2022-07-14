add_library(x-IMU3-Bootloader STATIC Ximu3Bootloader.c)

if (APPLE)
    target_link_libraries(x-IMU3-Bootloader PRIVATE "-framework Cocoa" "-framework IOKit")

    if (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL arm64)
        set(BOOTLOADER_LIBRARY libds30_loader_macos_arm64.a)
    else()
        set(BOOTLOADER_LIBRARY libds30_loader_macos_x64.a)
    endif()
elseif (WIN32)
    set(BOOTLOADER_LIBRARY ds30_Loader_Win_x64.lib)
endif()

target_link_libraries(x-IMU3-Bootloader PRIVATE "${PROJECT_SOURCE_DIR}/x-IMU3-Bootloader/${BOOTLOADER_LIBRARY}")
