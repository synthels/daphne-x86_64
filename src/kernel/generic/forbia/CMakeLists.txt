set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/generic/forbia)

set(SRC
    ${SRCROOT}/kernel.c
    ${SRCROOT}/kernel.h
    ${SRCROOT}/types.h
)

target_sources(kernel.bin PRIVATE ${SRC})
