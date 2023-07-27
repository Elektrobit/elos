// SPDX-License-Identifier: MIT
#ifndef __MOCK_LIBC_H__
#define __MOCK_LIBC_H__

// clang-format off
// because this order is a cmocka requirement
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <cmocka.h>
// clang-format on

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_extensions/mock_func_wrap.h>
#include <dirent.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <regex.h>
#include <semaphore.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/eventfd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

MOCK_FUNC_VAR_EXTERN(regcomp);
int MOCK_FUNC_WRAP(regcomp)(regex_t *preg, const char *regex, int cflags);
int MOCK_FUNC_REAL(regcomp)(regex_t *preg, const char *regex, int cflags);

MOCK_FUNC_VAR_EXTERN(regerror);
size_t MOCK_FUNC_WRAP(regerror)(int errcode, const regex_t *preg, char *errbuf, size_t errbuf_size);
size_t MOCK_FUNC_REAL(regerror)(int errcode, const regex_t *preg, char *errbuf, size_t errbuf_size);

MOCK_FUNC_VAR_EXTERN(regexec);
int MOCK_FUNC_WRAP(regexec)(const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags);
int MOCK_FUNC_REAL(regexec)(const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags);

MOCK_FUNC_VAR_EXTERN(malloc);
void *MOCK_FUNC_WRAP(malloc)(size_t size);
void *MOCK_FUNC_REAL(malloc)(size_t size);

MOCK_FUNC_VAR_EXTERN(realloc);
void *MOCK_FUNC_WRAP(realloc)(void *ptr, size_t newSize);
void *MOCK_FUNC_REAL(realloc)(void *ptr, size_t newSize);

MOCK_FUNC_VAR_EXTERN(calloc);
void *MOCK_FUNC_WRAP(calloc)(size_t nmemb, size_t size);
void *MOCK_FUNC_REAL(calloc)(size_t nmemb, size_t size);

MOCK_FUNC_VAR_EXTERN(free);
void MOCK_FUNC_WRAP(free)(void *ptr);
void MOCK_FUNC_REAL(free)(void *ptr);

MOCK_FUNC_VAR_EXTERN(memcpy);
void *MOCK_FUNC_WRAP(memcpy)(void *destination, const void *source, size_t num);
void *MOCK_FUNC_REAL(memcpy)(void *destination, const void *source, size_t num);

MOCK_FUNC_VAR_EXTERN(strdup);
char *MOCK_FUNC_WRAP(strdup)(const char *string);
char *MOCK_FUNC_REAL(strdup)(const char *string);

MOCK_FUNC_VAR_EXTERN(strndup);
char *MOCK_FUNC_WRAP(strndup)(const char *string, size_t n);
char *MOCK_FUNC_REAL(strndup)(const char *string, size_t n);

MOCK_FUNC_VAR_EXTERN(access);
int MOCK_FUNC_WRAP(access)(const char *pathname, int mode);
int MOCK_FUNC_REAL(access)(const char *pathname, int mode);

MOCK_FUNC_VAR_EXTERN(fopen);
FILE *MOCK_FUNC_WRAP(fopen)(const char *filename, const char *mode);
FILE *MOCK_FUNC_REAL(fopen)(const char *filename, const char *mode);

MOCK_FUNC_VAR_EXTERN(fdopen);
FILE *MOCK_FUNC_WRAP(fdopen)(const int fd, const char *mode);
FILE *MOCK_FUNC_REAL(fdopen)(const int fd, const char *mode);

MOCK_FUNC_VAR_EXTERN(fclose);
int MOCK_FUNC_WRAP(fclose)(FILE *stream);
int MOCK_FUNC_REAL(fclose)(FILE *stream);

MOCK_FUNC_VAR_EXTERN(fwrite);
size_t MOCK_FUNC_WRAP(fwrite)(const void *ptr, size_t size, size_t count, FILE *stream);
size_t MOCK_FUNC_REAL(fwrite)(const void *ptr, size_t size, size_t count, FILE *stream);

MOCK_FUNC_VAR_EXTERN(fread);
size_t MOCK_FUNC_WRAP(fread)(void *ptr, size_t size, size_t count, FILE *stream);
size_t MOCK_FUNC_REAL(fread)(void *ptr, size_t size, size_t count, FILE *stream);

MOCK_FUNC_VAR_EXTERN(scandir);
int MOCK_FUNC_WRAP(scandir)(const char *dirp, struct dirent ***namelist, int (*filter)(const struct dirent *),
                            int (*compar)(const struct dirent **, const struct dirent **));
int MOCK_FUNC_REAL(scandir)(const char *dirp, struct dirent ***namelist, int (*filter)(const struct dirent *),
                            int (*compar)(const struct dirent **, const struct dirent **));

MOCK_FUNC_VAR_EXTERN(fseek);
int MOCK_FUNC_WRAP(fseek)(FILE *stream, long int offset, int origin);
int MOCK_FUNC_REAL(fseek)(FILE *stream, long int offset, int origin);

MOCK_FUNC_VAR_EXTERN(rewind);
void MOCK_FUNC_WRAP(rewind)(FILE *stream);
void MOCK_FUNC_REAL(rewind)(FILE *stream);

MOCK_FUNC_VAR_EXTERN(ftell);
long int MOCK_FUNC_WRAP(ftell)(FILE *stream);
long int MOCK_FUNC_REAL(ftell)(FILE *stream);

MOCK_FUNC_VAR_EXTERN(popen);
FILE *MOCK_FUNC_WRAP(popen)(const char *command, const char *type);
FILE *MOCK_FUNC_REAL(popen)(const char *command, const char *type);

MOCK_FUNC_VAR_EXTERN(pclose);
int MOCK_FUNC_WRAP(pclose)(FILE *stream);
int MOCK_FUNC_REAL(pclose)(FILE *stream);

MOCK_FUNC_VAR_EXTERN(getline);
ssize_t MOCK_FUNC_WRAP(getline)(char **lineptr, size_t *n, FILE *stream);
ssize_t MOCK_FUNC_REAL(getline)(char **lineptr, size_t *n, FILE *stream);

MOCK_FUNC_VAR_EXTERN(getenv);
char *MOCK_FUNC_WRAP(getenv)(const char *name);
char *MOCK_FUNC_REAL(getenv)(const char *name);

MOCK_FUNC_VAR_EXTERN(getpid);
pid_t MOCK_FUNC_WRAP(getpid)();
pid_t MOCK_FUNC_REAL(getpid)();

MOCK_FUNC_VAR_EXTERN(opendir);
DIR *MOCK_FUNC_WRAP(opendir)(const char *name);
DIR *MOCK_FUNC_REAL(opendir)(const char *name);

MOCK_FUNC_VAR_EXTERN(closedir);
int MOCK_FUNC_WRAP(closedir)(DIR *dirp);
int MOCK_FUNC_REAL(closedir)(DIR *dirp);

MOCK_FUNC_VAR_EXTERN(remove);
int MOCK_FUNC_WRAP(remove)(const char *filename);
int MOCK_FUNC_REAL(remove)(const char *filename);

MOCK_FUNC_VAR_EXTERN(readdir);
struct dirent *MOCK_FUNC_WRAP(readdir)(DIR *dirp);
struct dirent *MOCK_FUNC_REAL(readdir)(DIR *dirp);

MOCK_FUNC_VAR_EXTERN(stat);
int MOCK_FUNC_WRAP(stat)(const char *pathname, struct stat *statbuf);
int MOCK_FUNC_REAL(stat)(const char *pathname, struct stat *statbuf);

MOCK_FUNC_VAR_EXTERN(fputc);
int MOCK_FUNC_WRAP(fputc)(int character, FILE *stream);
int MOCK_FUNC_REAL(fputc)(int character, FILE *stream);

MOCK_FUNC_VAR_EXTERN(time);
time_t MOCK_FUNC_REAL(time)(time_t *timer);
time_t MOCK_FUNC_WRAP(time)(time_t *timer);

MOCK_FUNC_VAR_EXTERN(clock_gettime);
int MOCK_FUNC_REAL(clock_gettime)(clockid_t clock_id, struct timespec *tp);
int MOCK_FUNC_WRAP(clock_gettime)(clockid_t clock_id, struct timespec *tp);

MOCK_FUNC_VAR_EXTERN(inet_aton);
int MOCK_FUNC_WRAP(inet_aton)(const char *cp, struct in_addr *inp);
int MOCK_FUNC_REAL(inet_aton)(const char *cp, struct in_addr *inp);

MOCK_FUNC_VAR_EXTERN(inet_pton);
int MOCK_FUNC_WRAP(inet_pton)(int af, const char *cp, void *buf);
int MOCK_FUNC_REAL(inet_pton)(int af, const char *cp, void *buf);

MOCK_FUNC_VAR_EXTERN(getaddrinfo);
int MOCK_FUNC_WRAP(getaddrinfo)(const char *node, const char *service, const struct addrinfo *hints,
                                struct addrinfo **res);
int MOCK_FUNC_REAL(getaddrinfo)(const char *node, const char *service, const struct addrinfo *hints,
                                struct addrinfo **res);

MOCK_FUNC_VAR_EXTERN(freeaddrinfo);
void MOCK_FUNC_WRAP(freeaddrinfo)(struct addrinfo *res);
void MOCK_FUNC_REAL(freeaddrinfo)(struct addrinfo *res);

MOCK_FUNC_VAR_EXTERN(socket);
int MOCK_FUNC_WRAP(socket)(int domain, int type, int protocol);
int MOCK_FUNC_REAL(socket)(int domain, int type, int protocol);

MOCK_FUNC_VAR_EXTERN(setsockopt);
int MOCK_FUNC_WRAP(setsockopt)(int fd, int level, int optname, const void *optval, socklen_t optlen);
int MOCK_FUNC_REAL(setsockopt)(int fd, int level, int optname, const void *optval, socklen_t optlen);

MOCK_FUNC_VAR_EXTERN(accept);
extern int MOCK_FUNC_WRAP(accept_errno);
int MOCK_FUNC_WRAP(accept)(int fd, __SOCKADDR_ARG addr, socklen_t *len);
int MOCK_FUNC_REAL(accept)(int fd, __SOCKADDR_ARG addr, socklen_t *len);

MOCK_FUNC_VAR_EXTERN(connect);
int MOCK_FUNC_WRAP(connect)(int fd, const struct sockaddr *addr, socklen_t len);
int MOCK_FUNC_REAL(connect)(int fd, const struct sockaddr *addr, socklen_t len);

MOCK_FUNC_VAR_EXTERN(bind);
int MOCK_FUNC_WRAP(bind)(int fd, __CONST_SOCKADDR_ARG addr, socklen_t len);
int MOCK_FUNC_REAL(bind)(int fd, __CONST_SOCKADDR_ARG addr, socklen_t len);

MOCK_FUNC_VAR_EXTERN(listen);
int MOCK_FUNC_WRAP(listen)(int fd, int n);
int MOCK_FUNC_REAL(listen)(int fd, int n);

MOCK_FUNC_VAR_EXTERN(close);
int MOCK_FUNC_WRAP(close)(int fd);
int MOCK_FUNC_REAL(close)(int fd);

MOCK_FUNC_VAR_EXTERN(open);
int MOCK_FUNC_WRAP(open)(char *file, int flags, mode_t mode);
int MOCK_FUNC_REAL(open)(char *file, int flags, mode_t mode);

MOCK_FUNC_VAR_EXTERN(pselect);
extern int MOCK_FUNC_WRAP(pselect_errno);
int MOCK_FUNC_WRAP(pselect)(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
                            const struct timespec *timeout, const __sigset_t *sigmask);
int MOCK_FUNC_REAL(pselect)(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
                            const struct timespec *timeout, const __sigset_t *sigmask);

MOCK_FUNC_PROTOTYPE(raise, int, int __sig)
MOCK_FUNC_PROTOTYPE(pthread_create, int, pthread_t *__newthread, const pthread_attr_t *__attr,
                    void *(*__start_routine)(void *), void *__arg)
MOCK_FUNC_PROTOTYPE(pthread_join, int, pthread_t __th, void **__thread_return)
MOCK_FUNC_PROTOTYPE(pthread_mutex_init, int, pthread_mutex_t *__mutex, const pthread_mutexattr_t *__mutexattr)
MOCK_FUNC_PROTOTYPE(pthread_mutex_destroy, int, pthread_mutex_t *__mutex)
MOCK_FUNC_PROTOTYPE(pthread_mutex_trylock, int, pthread_mutex_t *__mutex)
MOCK_FUNC_PROTOTYPE(pthread_mutex_lock, int, pthread_mutex_t *__mutex)
MOCK_FUNC_PROTOTYPE(pthread_mutex_unlock, int, pthread_mutex_t *__mutex)
MOCK_FUNC_PROTOTYPE(pthread_mutex_timedlock, int, pthread_mutex_t *__mutex, const struct timespec *__abstime)
MOCK_FUNC_PROTOTYPE(sem_init, int, sem_t *__sem, int __pshared, unsigned int __value)
MOCK_FUNC_PROTOTYPE(sem_post, int, sem_t *__sem)
MOCK_FUNC_PROTOTYPE(sem_destroy, int, sem_t *__sem)
MOCK_FUNC_PROTOTYPE(sem_timedwait, int, sem_t *__sem, const struct timespec *__abstime)

MOCK_FUNC_PROTOTYPE(eventfd, int, unsigned int __count, int __flags)
MOCK_FUNC_PROTOTYPE(eventfd_read, int, int __fd, eventfd_t *__value)
MOCK_FUNC_PROTOTYPE(eventfd_write, int, int __fd, eventfd_t __value)

#endif
