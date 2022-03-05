set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel)

add_subdirectory(${SRCROOT}/arch)
add_subdirectory(${SRCROOT}/lib)
add_subdirectory(${SRCROOT}/modules)
add_subdirectory(${SRCROOT}/generic)

set(SRC
    ${SRCROOT}/main.c
    ${SRCROOT}/main.h
)

target_sources(kernel.bin PRIVATE ${SRC})
