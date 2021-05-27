set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/tests)

set(SRC
	${SRCROOT}/tests.c
	${SRCROOT}/tests.c
	${SRCROOT}/malloc_test.c
	${SRCROOT}/malloc_test.h
)

target_sources(kernel.bin PRIVATE ${SRC})
