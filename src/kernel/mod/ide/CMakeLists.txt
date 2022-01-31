set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/mod/ide)

set(SRC
    ${SRCROOT}/ide.c
    ${SRCROOT}/ide.h
)

target_sources(kernel.bin PRIVATE ${SRC})
