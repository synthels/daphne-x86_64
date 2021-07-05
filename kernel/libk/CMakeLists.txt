set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/libk)

set(SRC
	${SRCROOT}/string.c
	${SRCROOT}/string.h
	${SRCROOT}/sleep.c
	${SRCROOT}/sleep.h
)

target_sources(kernel.bin PRIVATE ${SRC})
