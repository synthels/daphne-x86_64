set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel)

add_subdirectory(${SRCROOT}/arch)
add_subdirectory(${SRCROOT}/io)
add_subdirectory(${SRCROOT}/vid)
add_subdirectory(${SRCROOT}/libk)
add_subdirectory(${SRCROOT}/mem)

set(SRC
	${SRCROOT}/kernel.c
	${SRCROOT}/kernel.h
	${SRCROOT}/mutex.c
	${SRCROOT}/mutex.h
	${SRCROOT}/kmain.c
	${SRCROOT}/kmain.h
	${SRCROOT}/panic.c
	${SRCROOT}/panic.h
)

target_sources(kernel.bin PRIVATE ${SRC})
