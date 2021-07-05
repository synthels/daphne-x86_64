set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/dev)

set(SRC
	${SRCROOT}/kbd.c
	${SRCROOT}/kbd.h
	${SRCROOT}/pit.c
	${SRCROOT}/pit.h
	${SRCROOT}/pcspk.c
	${SRCROOT}/pcspk.h
	${SRCROOT}/dev.c
	${SRCROOT}/dev.h
)

target_sources(kernel.bin PRIVATE ${SRC})
