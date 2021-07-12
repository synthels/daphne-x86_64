set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/forbia)

set(SRC
	${SRCROOT}/types.h
)

target_sources(kernel.bin PRIVATE ${SRC})
