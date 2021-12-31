set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/mod/tm)

set(SRC
    ${SRCROOT}/pit.c
    ${SRCROOT}/pit.h
    ${SRCROOT}/tm.c
    ${SRCROOT}/tm.h
)

target_sources(kernel.bin PRIVATE ${SRC})
