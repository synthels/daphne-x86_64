set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/generic/io)

set(SRC
    ${SRCROOT}/io.c
    ${SRCROOT}/io.h
)

target_sources(kernel.bin PRIVATE ${SRC})
