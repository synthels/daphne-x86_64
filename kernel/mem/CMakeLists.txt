set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/mem)

set(SRC
    ${SRCROOT}/mem.c
    ${SRCROOT}/mem.h
    ${SRCROOT}/bitmap.c
    ${SRCROOT}/bitmap.h
)

target_sources(kernel.bin PRIVATE ${SRC})
