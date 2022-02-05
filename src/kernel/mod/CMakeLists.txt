set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/mod)

add_subdirectory(${SRCROOT}/kbd)
add_subdirectory(${SRCROOT}/pcspk)
add_subdirectory(${SRCROOT}/tm)
add_subdirectory(${SRCROOT}/pci)
add_subdirectory(${SRCROOT}/fb)

target_sources(kernel.bin PRIVATE ${SRC})
