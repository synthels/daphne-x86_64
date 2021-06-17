set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/arch/x86_32)

set(SRC
	${SRCROOT}/ring3.asm
	${SRCROOT}/gdt.asm
	${SRCROOT}/tss.c
	${SRCROOT}/tss.h
	${SRCROOT}/gdt.c
	${SRCROOT}/gdt.h
	${SRCROOT}/mm.c
	${SRCROOT}/mm.h
)

add_subdirectory(${SRCROOT}/boot)
add_subdirectory(${SRCROOT}/idt)

target_sources(kernel.bin PRIVATE ${SRC})
