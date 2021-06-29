set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/vid/console)

set(SRC
    ${SRCROOT}/con.c
    ${SRCROOT}/con.h
    ${SRCROOT}/font.h
)

target_sources(kernel.bin PRIVATE ${SRC})
