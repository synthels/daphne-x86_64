set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/mod/hpet)

set(SRC
    ${SRCROOT}/hpet.c
    ${SRCROOT}/hpet.h
)

target_sources(kernel.bin PRIVATE ${SRC})
