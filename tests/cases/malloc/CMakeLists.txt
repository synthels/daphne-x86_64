set(SRCROOT ${PROJECT_SOURCE_DIR}/tests/cases/malloc)

set(SRC
    ${SRCROOT}/malloc_test.c
    ${SRCROOT}/malloc_test.h
)

target_sources(kernel.bin PRIVATE ${SRC})
