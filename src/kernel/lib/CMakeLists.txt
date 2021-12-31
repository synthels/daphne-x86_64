set(SRCROOT ${PROJECT_SOURCE_DIR}/src/kernel/lib)

set(SRC
    ${SRCROOT}/string.c
    ${SRCROOT}/string.h
    ${SRCROOT}/stdlib.c
    ${SRCROOT}/stdlib.h
    ${SRCROOT}/printk.c
    ${SRCROOT}/printk.h
    ${SRCROOT}/sleep.c
    ${SRCROOT}/sleep.h
    ${SRCROOT}/vec.c
    ${SRCROOT}/vec.h
)

target_sources(kernel.bin PRIVATE ${SRC})
