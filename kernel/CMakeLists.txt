set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel)

add_subdirectory(${SRCROOT}/arch)
add_subdirectory(${SRCROOT}/drivers)
add_subdirectory(${SRCROOT}/io)
add_subdirectory(${SRCROOT}/kmain)
add_subdirectory(${SRCROOT}/logger)
add_subdirectory(${SRCROOT}/memory)
add_subdirectory(${SRCROOT}/tty)
add_subdirectory(${SRCROOT}/libk)

set(SRC
	${SRCROOT}/mutex.c
	${SRCROOT}/mutex.h
	${SRCROOT}/kernel.c
	${SRCROOT}/kernel.h
)

target_sources(kernel.bin PRIVATE ${SRC})
