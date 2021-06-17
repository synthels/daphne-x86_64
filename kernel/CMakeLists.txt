set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel)

add_subdirectory(${SRCROOT}/arch)
add_subdirectory(${SRCROOT}/drivers)
add_subdirectory(${SRCROOT}/io)
add_subdirectory(${SRCROOT}/memory)
add_subdirectory(${SRCROOT}/tty)
add_subdirectory(${SRCROOT}/proc)
add_subdirectory(${SRCROOT}/libk)

set(SRC
	${SRCROOT}/mutex.c
	${SRCROOT}/mutex.h
	${SRCROOT}/kmain.c
	${SRCROOT}/kmain.h
	${SRCROOT}/kernel.c
	${SRCROOT}/kernel.h
	${SRCROOT}/logk.c
	${SRCROOT}/logk.h
	${SRCROOT}/panic.c
	${SRCROOT}/panic.h
)

target_sources(kernel.bin PRIVATE ${SRC})
