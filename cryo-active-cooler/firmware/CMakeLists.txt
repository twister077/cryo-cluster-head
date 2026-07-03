set(BOARD nrf52832dk_nrf52832)
set(CMAKE_C_STANDARD 99)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(active-cooler)

target_sources(app PRIVATE
    src/main.c
    src/temp_voter.c
    src/peltier.c
    src/safety.c
    src/auto_mode.c
    src/ble_patch.c
    src/ble_phone.c
)
