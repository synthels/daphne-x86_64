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
    ${SRCROOT}/acpi.c
    ${SRCROOT}/acpi.h
    ${SRCROOT}/debug.c
    ${SRCROOT}/debug.h
    ${SRCROOT}/madt.c
    ${SRCROOT}/madt.h
    ${SRCROOT}/smp.c
    ${SRCROOT}/smp.h
    ${SRCROOT}/tsc.c
    ${SRCROOT}/tsc.h
    ${SRCROOT}/smp.asm
    ${SRCROOT}/user.asm
    ${SRCROOT}/irq.c
    ${SRCROOT}/irq.h
    ${SRCROOT}/ports.c
    ${SRCROOT}/ports.h
    ${SRCROOT}/cmos.c
    ${SRCROOT}/cmos.h
)

add_subdirectory(${SRCROOT}/idt)
target_sources(kernel.bin PRIVATE ${SRC})
