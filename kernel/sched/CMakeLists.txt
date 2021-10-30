set(SRCROOT ${PROJECT_SOURCE_DIR}/kernel/sched)

set(SRC
    ${SRCROOT}/task.c
    ${SRCROOT}/task.h
    ${SRCROOT}/sched.c
    ${SRCROOT}/sched.h
)

target_sources(kernel.bin PRIVATE ${SRC})
