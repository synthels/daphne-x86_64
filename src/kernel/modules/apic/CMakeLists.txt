set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/modules/apic)

set(SRC
    ${SRCROOT}/apic.c
    ${SRCROOT}/apic.h
)

target_sources(kernel.bin PRIVATE ${SRC})
