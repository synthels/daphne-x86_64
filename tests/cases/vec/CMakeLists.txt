set(SRCROOT ${PROJECT_SOURCE_DIR}/tests/cases/vec)

set(SRC
    ${SRCROOT}/struct Vectorest.c
    ${SRCROOT}/struct Vectorest.h
)

target_sources(kernel.bin PRIVATE ${SRC})
