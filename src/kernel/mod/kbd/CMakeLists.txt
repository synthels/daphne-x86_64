set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/mod/kbd)

set(SRC
    ${SRCROOT}/kbd.c
    ${SRCROOT}/kbd.h
)

target_sources(kernel.bin PRIVATE ${SRC})
