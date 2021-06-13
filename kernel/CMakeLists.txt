set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel)

add_subdirectory(${SRCROOT}/arch)
add_subdirectory(${SRCROOT}/drivers)
add_subdirectory(${SRCROOT}/io)
add_subdirectory(${SRCROOT}/logger)
add_subdirectory(${SRCROOT}/memory)
add_subdirectory(${SRCROOT}/tty)
add_subdirectory(${SRCROOT}/libk)
add_subdirectory(${SRCROOT}/proc)

set(SRC
	${SRCROOT}/mutex.c
	${SRCROOT}/mutex.h
	${SRCROOT}/kmain.c
	${SRCROOT}/kmain.h
	${SRCROOT}/kernel.c
	${SRCROOT}/kernel.h
)

target_sources(kernel.bin PRIVATE ${SRC})
