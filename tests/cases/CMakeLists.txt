set(SRCROOT ${PROJECT_SOURCE_DIR}/tests/cases)

add_subdirectory(${SRCROOT}/malloc)
add_subdirectory(${SRCROOT}/vec)

target_sources(kernel.bin PRIVATE ${SRC})
