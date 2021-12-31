set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/generic/shrimp)

set(SRC
    ${SRCROOT}/shrimp.c
    ${SRCROOT}/shrimp.h
    ${SRCROOT}/font.c
    ${SRCROOT}/font.h
)

target_sources(kernel.bin PRIVATE ${SRC})
