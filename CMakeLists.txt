cmake_minimum_required(VERSION 3.13)
project(daphne C ASM_NASM)

# Configure binary
set(KERNEL_BIN "kernel.bin")
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf)

# Enable nasm
enable_language(ASM_NASM)

add_executable(${KERNEL_BIN})

set(SRCROOT ${PROJECT_SOURCE_DIR})
set(CMAKE_C_FLAGS "-ffreestanding -O2 -Wall -Wextra -fPIE -Ilib -Ikernel -Wno-maybe-uninitialized")
set(CMAKE_C_STANDARD 11)

# Build tests if needed
if (BUILD_TESTS)
    add_subdirectory(${SRCROOT}/tests)
    add_compile_definitions(BUILD_TESTS)
endif()

if (ARCH STREQUAL "x86_64")
    add_compile_definitions(ARCH_x86_64)
endif()

# Compile ramdisk
execute_process(COMMAND ${SRCROOT}/tools/mk-ramdisk.sh WORKING_DIRECTORY ${SRCROOT})

# Build kernel
target_include_directories(${KERNEL_BIN} PRIVATE ${SRCROOT} ${SRCROOT}/src/kernel)
add_subdirectory(${SRCROOT}/src/kernel)
