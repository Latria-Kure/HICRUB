cmake_minimum_required(VERSION 3.22)
cmake_policy(SET CMP0091 NEW)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
# set(TOOLCHAIN_NAME "arm-none-embed")
# set(CMAKE_SYSTEM_NAME Generic)

# # specify cross-compilers and tools
# set(CMAKE_C_COMPILER arm-none-eabi-gcc)
# set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
# set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
# set(CMAKE_AR arm-none-eabi-ar)
# set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
# set(CMAKE_OBJDUMP arm-none-eabi-objdump)
# set(CMAKE_SIZE arm-none-eabi-size)
# set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
# project settings
project(HICRUB LANGUAGES ASM C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
message("[[[${LOCAL_TEST}]]]")
if(LOCAL_TEST)

else()
# specify cross-compiling tools
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(CMAKE_SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

add_executable(${PROJECT_NAME}.elf "")

# include directories
target_include_directories(${PROJECT_NAME}.elf PRIVATE
    app
    app/bsp
    app/usb_device
    app/usb_host

    # HAL
    extern/CMSIS/Include
    extern/STM32F4xx_HAL_Driver/Inc
    extern/CMSIS/Device/ST/STM32F4xx/Include
    app/config/stm32_hal

    # FreeRTOS
    extern/FreeRTOS/Kernel/include
    extern/FreeRTOS/Kernel/portable/GCC/ARM_CM4F
    app/config/FreeRTOS

    # CherryUSB
    extern/CherryUSB/class/hid
    extern/CherryUSB/class/hub
    extern/CherryUSB/common
    extern/CherryUSB/core
    extern/CherryUSB/port/dwc2
    app/config/CherryUSB

    # lib
    include/utils
)



# source files
# source files list
file(GLOB SOURCES
    startup_stm32f407xx.s
    app/*.c
    app/bsp/*.c
    app/usb_host/*.c
    app/usb_device/*.c

    # HAL
    extern/STM32F4xx_HAL_Driver/Src/*.c

    # FreeRTOS
    extern/FreeRTOS/Kernel/*.c
    extern/FreeRTOS/Kernel/portable/GCC/ARM_CM4F/port.c
    extern/FreeRTOS/Kernel/portable/MemMang/heap_4.c

    # CherryUSB
    extern/CherryUSB/class/hid/*.c
    extern/CherryUSB/class/hub/*.c
    extern/CherryUSB/core/*.c
    extern/CherryUSB/port/dwc2/usb_dc_dwc2.c
    extern/CherryUSB/port/dwc2/usb_hc_dwc2.c
    extern/CherryUSB/osal/usb_osal_freertos.c
    src/dwc2_port/usb_glue_st.c

    src/utils/*.c
)

# removed files
file(GLOB_RECURSE REMOVE_SOURCES
    extern/STM32F4xx_HAL_Driver/Src/*_template.c
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
    $<$<COMPILE_LANGUAGE:CXX>:--specs=nano.specs>

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
    COMMAND ${CMAKE_SIZE} -Ax $<TARGET_FILE:${PROJECT_NAME}.elf>
)
endif()