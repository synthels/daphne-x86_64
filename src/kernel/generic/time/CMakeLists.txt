set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/generic/time)

set(SRC
    ${SRCROOT}/clock.c
    ${SRCROOT}/clock.h
    ${SRCROOT}/jiffies.c
    ${SRCROOT}/jiffies.h
    ${SRCROOT}/time_source.c
    ${SRCROOT}/time_source.h
    ${SRCROOT}/persistent_time.h
    ${SRCROOT}/sleep.c
    ${SRCROOT}/sleep.h
    ${SRCROOT}/time.c
    ${SRCROOT}/time.h
    ${SRCROOT}/tm.h
)

target_sources(kernel.bin PRIVATE ${SRC})
