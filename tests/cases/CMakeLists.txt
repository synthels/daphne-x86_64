set(SRCROOT ${PROJECT_SOURCE_DIR}/tests/cases)

add_subdirectory(${SRCROOT}/malloc)

target_sources(kernel.bin PRIVATE ${SRC})
