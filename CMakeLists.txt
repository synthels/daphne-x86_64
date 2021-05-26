cmake_minimum_required(VERSION 3.12)
project(phiOS C ASM_NASM)

# Configure binary
set(KERNEL_BIN "kernel.bin")
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf)

# Enable nasm
enable_language(ASM_NASM)

add_executable(${KERNEL_BIN})

set(SRCROOT ${PROJECT_SOURCE_DIR})
set(CMAKE_C_FLAGS "-std=gnu99 -ffreestanding -O2 -Wall -Wextra -Ilib/include -Ikernel -Wno-maybe-uninitialized")

# Build kernel
target_include_directories(${KERNEL_BIN} PRIVATE ${SRCROOT}/kernel)
add_subdirectory(${SRCROOT}/kernel)
