set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/shrimp)

set(SRC
    ${SRCROOT}/shrimp.c
    ${SRCROOT}/shrimp.h
    ${SRCROOT}/shrimp_font.c
    ${SRCROOT}/shrimp_font.h
)

target_sources(kernel.bin PRIVATE ${SRC})
