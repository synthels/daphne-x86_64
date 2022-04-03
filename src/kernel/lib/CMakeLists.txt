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
    ${SRCROOT}/log.h
    ${SRCROOT}/tree.c
    ${SRCROOT}/tree.h
    ${SRCROOT}/tar.c
    ${SRCROOT}/tar.h
    ${SRCROOT}/elf.c
    ${SRCROOT}/elf.h
    ${SRCROOT}/hashmap.c
    ${SRCROOT}/hashmap.h
)

target_sources(kernel.bin PRIVATE ${SRC})
