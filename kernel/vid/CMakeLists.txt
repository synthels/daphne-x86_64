set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/vid)

set(SRC
	${SRCROOT}/vid.c
	${SRCROOT}/vid.h
)

target_sources(kernel.bin PRIVATE ${SRC})
