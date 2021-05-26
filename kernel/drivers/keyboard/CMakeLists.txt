set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/drivers/keyboard)

set(SRC
	${SRCROOT}/keyboard.c
	${SRCROOT}/keyboard.h
)

target_sources(kernel.bin PRIVATE ${SRC})
