set(SRCROOT ${PROJECT_SOURCE_DIR})

# Maybe these do something...
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER_WORKS 1)
# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "-T ${SRCROOT}/kernel/linker.ld -fno-pic -fpie -Wl,-static,-pie,--no-dynamic-linker -ztext -static -pie -nostdlib -z max-page-size=0x1000" CACHE STRING "" FORCE)
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
