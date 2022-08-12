set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/generic/binfmt)

set(SRC
    ${SRCROOT}/binfmt.c
    ${SRCROOT}/binfmt.h
)

target_sources(kernel.bin PRIVATE ${SRC})
