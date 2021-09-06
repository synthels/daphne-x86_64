set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel)

add_subdirectory(${SRCROOT}/arch)
add_subdirectory(${SRCROOT}/io)
add_subdirectory(${SRCROOT}/vid)
add_subdirectory(${SRCROOT}/libk)
add_subdirectory(${SRCROOT}/mem)
add_subdirectory(${SRCROOT}/dev)
add_subdirectory(${SRCROOT}/shrimp)
add_subdirectory(${SRCROOT}/forbia)
add_subdirectory(${SRCROOT}/malloc)
add_subdirectory(${SRCROOT}/proc)

set(SRC
    ${SRCROOT}/kmain.c
    ${SRCROOT}/kmain.h
)

target_sources(kernel.bin PRIVATE ${SRC})
