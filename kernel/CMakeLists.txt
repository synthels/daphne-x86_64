set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel)

add_subdirectory(${SRCROOT}/arch)
add_subdirectory(${SRCROOT}/libk)
add_subdirectory(${SRCROOT}/dev)
add_subdirectory(${SRCROOT}/shrimp)
add_subdirectory(${SRCROOT}/forbia)

set(SRC
    ${SRCROOT}/kmain.c
    ${SRCROOT}/kmain.h
    ${SRCROOT}/io.c
    ${SRCROOT}/io.h
    ${SRCROOT}/malloc.c
    ${SRCROOT}/malloc.h
    ${SRCROOT}/mem.c
    ${SRCROOT}/mem.h
    ${SRCROOT}/pci.c
    ${SRCROOT}/pci.h
    ${SRCROOT}/process.c
    ${SRCROOT}/process.h
    ${SRCROOT}/sched.c
    ${SRCROOT}/sched.h
    ${SRCROOT}/space.c
    ${SRCROOT}/space.h
)

target_sources(kernel.bin PRIVATE ${SRC})
