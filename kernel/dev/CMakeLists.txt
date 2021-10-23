set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/dev)

add_subdirectory(${SRCROOT}/kbd)
add_subdirectory(${SRCROOT}/pcspk)
add_subdirectory(${SRCROOT}/pit)
add_subdirectory(${SRCROOT}/ahci)

set(SRC
    ${SRCROOT}/dev.c
    ${SRCROOT}/dev.h
)

target_sources(kernel.bin PRIVATE ${SRC})
