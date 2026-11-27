cmake_minimum_required(VERSION 3.16)
project(stm32_fire_alarm_demo C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# The host demo compiles the same alarm state machine with a tiny GPIO shim.
# The STM32 firmware files require a CubeIDE project and STM32F4 HAL package.
add_executable(fire_alarm_host_demo
    Core/Src/fire_alarm.c
    host_demo/src/fake_hal.c
    host_demo/src/main.c
)
target_include_directories(fire_alarm_host_demo PRIVATE
    host_demo/include
    Core/Inc
)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(fire_alarm_host_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()
