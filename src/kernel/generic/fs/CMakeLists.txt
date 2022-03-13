set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/generic/fs)

set(SRC
    ${SRCROOT}/vfs.c
    ${SRCROOT}/vfs.h
)

target_sources(kernel.bin PRIVATE ${SRC})
