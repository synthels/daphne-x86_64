set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/arch/x86_64)

set(SRC
    ${SRCROOT}/gdt.c
    ${SRCROOT}/gdt.h
)

add_subdirectory(${SRCROOT}/idt)
target_sources(kernel.bin PRIVATE ${SRC})
