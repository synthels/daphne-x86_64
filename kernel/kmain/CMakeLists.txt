set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/kmain)

set(SRC
	${SRCROOT}/kmain.c
	${SRCROOT}/kmain.h
)

target_sources(kernel.bin PRIVATE ${SRC})
