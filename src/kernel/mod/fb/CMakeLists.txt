set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/mod/fb)

set(SRC
    ${SRCROOT}/lfb.c
    ${SRCROOT}/lfb.h
)

target_sources(kernel.bin PRIVATE ${SRC})
