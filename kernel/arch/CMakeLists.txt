set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/arch)

if(NOT ARCH)
    message(FATAL_ERROR "ARCH not defined. Please set to x86_64")
endif()

if (NOT (ARCH STREQUAL "x86_64"))
	message(FATAL_ERROR "The target ${ARCH} does not exist!")
endif()

add_subdirectory(${SRCROOT}/${ARCH})
