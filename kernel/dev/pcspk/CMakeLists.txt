set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/dev/pcspk)

set(SRC
    ${SRCROOT}/pcspk.c
    ${SRCROOT}/pcspk.h
)

target_sources(kernel.bin PRIVATE ${SRC})
