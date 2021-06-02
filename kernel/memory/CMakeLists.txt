set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/memory)

set(SRC
	${SRCROOT}/mm.c
	${SRCROOT}/mm.h
	${SRCROOT}/alloc.c
	${SRCROOT}/alloc.h
)

add_subdirectory(${SRCROOT}/paging)
add_subdirectory(${SRCROOT}/heap)

target_sources(kernel.bin PRIVATE ${SRC})
