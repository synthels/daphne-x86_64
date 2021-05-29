set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/libk)

set(SRC
	${SRCROOT}/string.c
	${SRCROOT}/string.h
	${SRCROOT}/stdlib.c
	${SRCROOT}/stdlib.h
	${SRCROOT}/malloc.c
	${SRCROOT}/malloc.h
)

target_sources(kernel.bin PRIVATE ${SRC})
