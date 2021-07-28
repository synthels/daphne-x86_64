set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/forbia)

set(SRC
	${SRCROOT}/kernel.c
	${SRCROOT}/kernel.h
	${SRCROOT}/panic.c
	${SRCROOT}/panic.h
	${SRCROOT}/lock.c
	${SRCROOT}/lock.h
	${SRCROOT}/types.h
)

target_sources(kernel.bin PRIVATE ${SRC})
