set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/lib/tinf)

set(SRC
    ${SRCROOT}/adler32.c
    ${SRCROOT}/crc32.c
    ${SRCROOT}/tinfgzip.c
    ${SRCROOT}/tinflate.c
    ${SRCROOT}/tinfzlib.c
    ${SRCROOT}/tinf.h
)

target_sources(kernel.bin PRIVATE ${SRC})
