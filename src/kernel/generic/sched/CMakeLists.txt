set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/generic/sched)

set(SRC
    ${SRCROOT}/task.c
    ${SRCROOT}/task.h
)

target_sources(kernel.bin PRIVATE ${SRC})
