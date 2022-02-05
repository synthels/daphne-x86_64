set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/arch/x86_64)

set(SRC
    ${SRCROOT}/gdt.c
    ${SRCROOT}/gdt.h
    ${SRCROOT}/vmm.c
    ${SRCROOT}/vmm.h
    ${SRCROOT}/pmm.c
    ${SRCROOT}/pmm.h
    ${SRCROOT}/cpu.c
    ${SRCROOT}/cpu.h
    ${SRCROOT}/context.c
    ${SRCROOT}/context.h
    ${SRCROOT}/pic.c
    ${SRCROOT}/pic.h
    ${SRCROOT}/apic.c
    ${SRCROOT}/apic.h
    ${SRCROOT}/acpi.c
    ${SRCROOT}/acpi.h
)

add_subdirectory(${SRCROOT}/idt)
target_sources(kernel.bin PRIVATE ${SRC})
