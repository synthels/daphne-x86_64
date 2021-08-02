set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/mem)

set(SRC
    ${SRCROOT}/mem.c
    ${SRCROOT}/mem.h
)

target_sources(kernel.bin PRIVATE ${SRC})
