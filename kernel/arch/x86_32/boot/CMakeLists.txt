set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/arch/x86_32/boot)

set(SRC
	${SRCROOT}/boot.asm
	${SRCROOT}/paging.asm
)

target_sources(kernel.bin PRIVATE ${SRC})
