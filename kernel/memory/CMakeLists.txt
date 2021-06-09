set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/memory)

set(SRC
	${SRCROOT}/mm.c
	${SRCROOT}/mm.h
	${SRCROOT}/wm.c
	${SRCROOT}/wm.h
	${SRCROOT}/malloc.c
	${SRCROOT}/malloc.h
)

target_sources(kernel.bin PRIVATE ${SRC})
