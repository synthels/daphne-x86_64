set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/generic)

add_subdirectory(${SRCROOT}/sched)
add_subdirectory(${SRCROOT}/memory)
add_subdirectory(${SRCROOT}/shrimp)
add_subdirectory(${SRCROOT}/forbia)
add_subdirectory(${SRCROOT}/malloc)
add_subdirectory(${SRCROOT}/fs)
add_subdirectory(${SRCROOT}/time)

target_sources(kernel.bin PRIVATE ${SRC})
