set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/lib)

set(SRC
    ${SRCROOT}/string.c
    ${SRCROOT}/string.h
    ${SRCROOT}/stdlib.c
    ${SRCROOT}/stdlib.h
    ${SRCROOT}/printk.c
    ${SRCROOT}/printk.h
    ${SRCROOT}/panic.c
    ${SRCROOT}/panic.h
    ${SRCROOT}/lock.c
    ${SRCROOT}/lock.h
    ${SRCROOT}/list.c
    ${SRCROOT}/list.h
    ${SRCROOT}/vec.c
    ${SRCROOT}/vec.h
    ${SRCROOT}/log.h
)

target_sources(kernel.bin PRIVATE ${SRC})
