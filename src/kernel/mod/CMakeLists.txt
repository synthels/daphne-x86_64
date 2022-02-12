set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/mod)

add_subdirectory(${SRCROOT}/kbd)
add_subdirectory(${SRCROOT}/pcspk)
add_subdirectory(${SRCROOT}/pci)
add_subdirectory(${SRCROOT}/fb)
add_subdirectory(${SRCROOT}/pit)
add_subdirectory(${SRCROOT}/apic)

target_sources(kernel.bin PRIVATE ${SRC})
