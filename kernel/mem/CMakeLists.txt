set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/mem)

set(SRC
    ${SRCROOT}/mem.c
    ${SRCROOT}/mem.h
    ${SRCROOT}/malloc.c
    ${SRCROOT}/malloc.h
)

target_sources(kernel.bin PRIVATE ${SRC})
