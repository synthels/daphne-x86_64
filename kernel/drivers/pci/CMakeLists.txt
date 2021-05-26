set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/drivers/pci)

set(SRC
	${SRCROOT}/pci.c
	${SRCROOT}/pci.h
)

target_sources(kernel.bin PRIVATE ${SRC})
