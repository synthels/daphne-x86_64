set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/memory)

set(SRC
	${SRCROOT}/mm.c
	${SRCROOT}/mm.h
	${SRCROOT}/malloc.c
	${SRCROOT}/malloc.h
)

add_subdirectory(${SRCROOT}/paging)

target_sources(kernel.bin PRIVATE ${SRC})
