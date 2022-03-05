set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/modules/pit)

set(SRC
    ${SRCROOT}/pit.c
    ${SRCROOT}/pit.h
)

target_sources(kernel.bin PRIVATE ${SRC})
