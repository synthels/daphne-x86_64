set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/tty)

set(SRC
	${SRCROOT}/printk.c
	${SRCROOT}/printk.h
)

target_sources(kernel.bin PRIVATE ${SRC})
