set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/io)

set(SRC
    ${SRCROOT}/io.c
    ${SRCROOT}/io.h
)

target_sources(kernel.bin PRIVATE ${SRC})
