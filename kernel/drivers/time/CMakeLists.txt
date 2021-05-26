set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/drivers/time)

set(SRC
	${SRCROOT}/PIT.c
	${SRCROOT}/PIT.h
	${SRCROOT}/sleep.c
	${SRCROOT}/sleep.h
)

target_sources(kernel.bin PRIVATE ${SRC})
