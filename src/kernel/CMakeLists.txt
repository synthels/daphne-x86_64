set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel)

add_subdirectory(${SRCROOT}/arch)
add_subdirectory(${SRCROOT}/lib)
add_subdirectory(${SRCROOT}/mod)
add_subdirectory(${SRCROOT}/generic)

set(SRC
    ${SRCROOT}/kmain.c
    ${SRCROOT}/kmain.h
)

target_sources(kernel.bin PRIVATE ${SRC})
