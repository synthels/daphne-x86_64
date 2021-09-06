set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/proc)

set(SRC
    ${SRCROOT}/process.c
    ${SRCROOT}/process.h
)

target_sources(kernel.bin PRIVATE ${SRC})
