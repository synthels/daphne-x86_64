set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/generic/memory)

set(SRC
    ${SRCROOT}/bitmap.c
    ${SRCROOT}/bitmap.h
    ${SRCROOT}/mem.c
    ${SRCROOT}/mem.h
    ${SRCROOT}/space.c
    ${SRCROOT}/space.h
    ${SRCROOT}/context.c
    ${SRCROOT}/context.h
)

target_sources(kernel.bin PRIVATE ${SRC})
