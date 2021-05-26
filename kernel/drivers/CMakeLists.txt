set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/drivers)

add_subdirectory(${SRCROOT}/keyboard)
add_subdirectory(${SRCROOT}/pci)
add_subdirectory(${SRCROOT}/sound)
add_subdirectory(${SRCROOT}/time)

set(SRC
	${SRCROOT}/driver.c
	${SRCROOT}/driver.h
)

target_sources(kernel.bin PRIVATE ${SRC})
