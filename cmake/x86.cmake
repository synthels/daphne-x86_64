set(SRCROOT ${PROJECT_SOURCE_DIR})

# Maybe these do something...
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER_WORKS 1)
# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "-T ${SRCROOT}/kernel/linker.ld -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -ffreestanding -O2 -nostdlib -lgcc" CACHE STRING "" FORCE)
