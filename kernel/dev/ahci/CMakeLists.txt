set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/dev/ahci)

set(SRC
    ${SRCROOT}/ahci.c
    ${SRCROOT}/ahci.h
)

target_sources(kernel.bin PRIVATE ${SRC})
