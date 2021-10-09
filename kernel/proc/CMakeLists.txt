set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/proc)

set(SRC
    ${SRCROOT}/process.c
    ${SRCROOT}/process.h
    ${SRCROOT}/sched.c
    ${SRCROOT}/sched.h
)

target_sources(kernel.bin PRIVATE ${SRC})
