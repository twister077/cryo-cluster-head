set(BOARD nrf52832dk_nrf52832)
set(CMAKE_C_STANDARD 99)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(alert-patch)

target_sources(app PRIVATE
    src/main.c
    src/temp_sensor.c
)
