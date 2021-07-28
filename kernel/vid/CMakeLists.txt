set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/vid)

set(SRC
    ${SRCROOT}/lfb.c
    ${SRCROOT}/lfb.h
)

target_sources(kernel.bin PRIVATE ${SRC})
