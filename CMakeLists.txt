#THIS FILE IS AUTO GENERATED FROM THE TEMPLATE! DO NOT CHANGE!
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.22)
cmake_policy(SET CMP0091 NEW)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(TOOLCHAIN_NAME "arm-none-embed")
set(CMAKE_SYSTEM_NAME Generic)

# specify cross-compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(HICRUB LANGUAGES ASM C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
add_executable(${PROJECT_NAME}.elf "")

# include directories
target_include_directories(${PROJECT_NAME}.elf PRIVATE
    Core/Inc
    Drivers/CMSIS/Include
    Drivers/STM32F4xx_HAL_Driver/Inc
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    utils

    # FreeRTOS
    components/FreeRTOS/Source/include
    components/FreeRTOS/Source/portable/GCC/ARM_CM4F

    components/CherryUSB/class/hid
    components/CherryUSB/class/hub
    components/CherryUSB/common
    components/CherryUSB/core
    components/CherryUSB/port/dwc2

    Drivers/usb/class/hid/
)



# source files
# source files list
file(GLOB SOURCES
    startup_stm32f407xx.s
    Core/Src/*.c
    Drivers/STM32F4xx_HAL_Driver/Src/*.c
    utils/*.c
    # Free RTOS
    components/FreeRTOS/Source/*.c
    components/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    components/FreeRTOS/Source/portable/MemMang/heap_4.c

    components/CherryUSB/class/hid/*.c
    components/CherryUSB/class/hub/*.c
    components/CherryUSB/core/*.c
    components/CherryUSB/port/dwc2/usb_dc_dwc2.c
    components/CherryUSB/port/dwc2/usb_hc_dwc2.c
    components/CherryUSB/port/dwc2/usb_glue_st.c
    components/CherryUSB/osal/usb_osal_freertos.c

    Drivers/usb/class/hid/hid_hcd_dwc2.c

)

# removed files
file(GLOB_RECURSE REMOVE_SOURCES
    Drivers/STM32F4xx_HAL_Driver/Src/*_template.c
)

list(REMOVE_ITEM SOURCES ${REMOVE_SOURCES})

target_sources(
    ${PROJECT_NAME}.elf PRIVATE
    ${SOURCES}
)

# compile options
target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
    STM32F407xx
    USE_HAL_DRIVER
)

target_compile_options(${PROJECT_NAME}.elf PRIVATE
    # Enable assembler files preprocessing
    $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp>

    $<$<COMPILE_LANGUAGE:C,CXX,ASM>:-mcpu=cortex-m4>
    $<$<COMPILE_LANGUAGE:C,CXX,ASM>:-mthumb>
    $<$<COMPILE_LANGUAGE:C,CXX,ASM>:-mfloat-abi=hard>
    $<$<COMPILE_LANGUAGE:C,CXX,ASM>:-mfpu=fpv4-sp-d16>
    $<$<COMPILE_LANGUAGE:C,CXX,ASM>:-Wall>
    $<$<COMPILE_LANGUAGE:C,CXX,ASM>:-fmessage-length=0>
    $<$<COMPILE_LANGUAGE:C,CXX,ASM>:-ffunction-sections>
    $<$<COMPILE_LANGUAGE:C,CXX,ASM>:-fdata-sections>
    $<$<COMPILE_LANGUAGE:C,CXX,ASM>:-fno-common>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>

    $<$<CONFIG:Release>:-Ofast>
    $<$<CONFIG:Debug>:-Og -g -gdwarf-2>
    $<$<CONFIG:RelWithDebInfo>:-Ofast -g -gdwarf-2>
    $<$<CONFIG:MinSizeRel>:-Os>
)

target_link_directories(${PROJECT_NAME}.elf PRIVATE

)

target_link_libraries(${PROJECT_NAME}.elf PRIVATE

)

target_link_options(${PROJECT_NAME}.elf PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfloat-abi=hard
    -mfpu=fpv4-sp-d16
    -fmessage-length=0
    -ffunction-sections
    -fdata-sections
    -fno-common
    -T${PROJECT_SOURCE_DIR}/STM32F407VETx_FLASH.ld
    -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map,--cref
    -Wl,--gc-sections,--print-memory-usage
    "SHELL:-lc -lm -lgcc"
    "SHELL:--specs=nosys.specs --specs=nano.specs"
    $<$<CONFIG:Release>:-Ofast>
    $<$<CONFIG:Debug>:-Og -g -gdwarf-2>
    $<$<CONFIG:RelWithDebInfo>:-Ofast -g -gdwarf-2>
    $<$<CONFIG:MinSizeRel>:-Os>
)




set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
    COMMENT "Building ${HEX_FILE} Building ${BIN_FILE}"
    COMMAND ${SIZE} -Ax $<TARGET_FILE:${PROJECT_NAME}.elf>
)
