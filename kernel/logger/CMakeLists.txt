set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/logger)

set(SRC
	${SRCROOT}/logk.c
	${SRCROOT}/logk.h
	${SRCROOT}/panic.c
	${SRCROOT}/panic.h
)

target_sources(kernel.bin PRIVATE ${SRC})
