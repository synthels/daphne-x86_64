set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/memory/paging)

set(SRC
	${SRCROOT}/paging.c
	${SRCROOT}/paging.h
)

target_sources(kernel.bin PRIVATE ${SRC})
