set(SRCROOT ${PROJECT_SOURCE_DIR}/tests/cases/vec)

set(SRC
    ${SRCROOT}/vec_test.c
    ${SRCROOT}/vec_test.h
)

target_sources(kernel.bin PRIVATE ${SRC})
