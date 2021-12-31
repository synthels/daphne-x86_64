set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/mod/pcspk)

set(SRC
    ${SRCROOT}/pcspk.c
    ${SRCROOT}/pcspk.h
)

target_sources(kernel.bin PRIVATE ${SRC})
