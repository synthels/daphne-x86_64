set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/malloc)

set(SRC
    ${SRCROOT}/malloc.c
    ${SRCROOT}/malloc.h
)

target_sources(kernel.bin PRIVATE ${SRC})
