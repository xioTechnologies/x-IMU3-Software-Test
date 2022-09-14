file(GLOB_RECURSE files "*.cpp")

add_executable(x-IMU3-Bootloader-Test ${files})

set_property(TARGET x-IMU3-Bootloader-Test PROPERTY CXX_STANDARD 17)

if (APPLE)
    target_link_libraries(x-IMU3-Bootloader-Test PRIVATE "-framework Cocoa" "-framework IOKit")
endif ()
