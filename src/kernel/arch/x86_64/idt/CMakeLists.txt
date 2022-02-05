set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/arch/x86_64/idt)

set(SRC
    ${SRCROOT}/irq.asm
    ${SRCROOT}/syscall.c
    ${SRCROOT}/idt.c
    ${SRCROOT}/idt.h
    ${SRCROOT}/PIC.c
    ${SRCROOT}/PIC.h
)

target_sources(kernel.bin PRIVATE ${SRC})
