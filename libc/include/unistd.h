#ifndef _UNISTD_H
#define _UNISTD_H
#include <sys/types.h>
#ifdef __cplusplus
extern "C" {
#endif
int execv(const char*, char* const[]);
int execve(const char*, char* const[], char* const[]);
int execvp(const char*, char* const[]);
pid_t fork(void);
#ifdef __cplusplus
}
#endif
#endif
