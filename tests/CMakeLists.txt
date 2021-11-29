set(SRCROOT ${PROJECT_SOURCE_DIR}/tests)

add_subdirectory(${SRCROOT}/cases)

set(SRC
    ${SRCROOT}/test.c
    ${SRCROOT}/test.h
)

target_sources(kernel.bin PRIVATE ${SRC})
