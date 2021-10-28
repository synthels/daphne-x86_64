set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/memory)

set(SRC
    ${SRCROOT}/bitmap.c
    ${SRCROOT}/bitmap.h
    ${SRCROOT}/mem.c
    ${SRCROOT}/mem.h
    ${SRCROOT}/space.c
    ${SRCROOT}/space.h
)

target_sources(kernel.bin PRIVATE ${SRC})
