set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/tty)

set(SRC
	${SRCROOT}/printk.c
	${SRCROOT}/printk.h
	${SRCROOT}/tty_io.c
	${SRCROOT}/tty_io.h
	${SRCROOT}/vga.h
)

target_sources(kernel.bin PRIVATE ${SRC})
