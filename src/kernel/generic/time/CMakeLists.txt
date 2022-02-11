set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/generic/time)

set(SRC
    ${SRCROOT}/time.c
    ${SRCROOT}/time.h
    ${SRCROOT}/jiffies.c
    ${SRCROOT}/jiffies.h
    ${SRCROOT}/time_source.c
    ${SRCROOT}/time_source.h
    ${SRCROOT}/sleep.c
    ${SRCROOT}/sleep.h
)

target_sources(kernel.bin PRIVATE ${SRC})
