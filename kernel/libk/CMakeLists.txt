set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/libk)

set(SRC
	${SRCROOT}/string.c
	${SRCROOT}/string.h
	${SRCROOT}/stdlib.c
	${SRCROOT}/stdlib.h
	${SRCROOT}/printk.c
	${SRCROOT}/printk.h
	${SRCROOT}/sleep.c
	${SRCROOT}/sleep.h
)

target_sources(kernel.bin PRIVATE ${SRC})
