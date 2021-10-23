set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/dev/pit)

set(SRC
    ${SRCROOT}/pit.c
    ${SRCROOT}/pit.h
)

target_sources(kernel.bin PRIVATE ${SRC})
