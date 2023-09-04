// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"

#include <cmocka.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

MOCK_FUNC_VAR_NEW(regcomp);
int MOCK_FUNC_WRAP(regcomp)(regex_t *preg, const char *regex, int cflags) {
    if (MOCK_IS_ACTIVE(regcomp)) {
        check_expected_ptr(preg);
        check_expected_ptr(regex);
        check_expected(cflags);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(regcomp)(preg, regex, cflags);
}

MOCK_FUNC_VAR_NEW(regerror);
size_t MOCK_FUNC_WRAP(regerror)(int errcode, const regex_t *preg, char *errbuf, size_t errbuf_size) {
    if (MOCK_IS_ACTIVE(regerror)) {
        check_expected(errcode);
        check_expected_ptr(preg);
        check_expected_ptr(errbuf);
        check_expected(errbuf_size);
        return mock_type(size_t);
    }
    return MOCK_FUNC_REAL(regerror)(errcode, preg, errbuf, errbuf_size);
}

MOCK_FUNC_VAR_NEW(regexec);
int MOCK_FUNC_WRAP(regexec)(const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags) {
    if (MOCK_IS_ACTIVE(regexec)) {
        check_expected_ptr(preg);
        check_expected_ptr(string);
        check_expected(nmatch);
        check_expected_ptr(pmatch);
        check_expected_ptr(eflags);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(regexec)(preg, string, nmatch, pmatch, eflags);
}

MOCK_FUNC_VAR_NEW(malloc);
void *MOCK_FUNC_WRAP(malloc)(size_t size) {
    if (MOCK_IS_ACTIVE(malloc)) {
        check_expected(size);
        return mock_ptr_type(void *);
    }
    return MOCK_FUNC_REAL(malloc)(size);
}

MOCK_FUNC_VAR_NEW(realloc);
void *MOCK_FUNC_WRAP(realloc)(void *ptr, size_t newSize) {
    if (MOCK_IS_ACTIVE(realloc)) {
        check_expected(ptr);
        check_expected(newSize);
        return mock_ptr_type(void *);
    }
    return MOCK_FUNC_REAL(realloc)(ptr, newSize);
}

MOCK_FUNC_VAR_NEW(calloc);
void *MOCK_FUNC_WRAP(calloc)(size_t nmemb, size_t size) {
    if (MOCK_IS_ACTIVE(calloc)) {
        check_expected(nmemb);
        check_expected(size);
        return mock_ptr_type(void *);
    }
    return MOCK_FUNC_REAL(calloc)(nmemb, size);
}

MOCK_FUNC_VAR_NEW(free);
void MOCK_FUNC_WRAP(free)(void *ptr) {
    if (MOCK_IS_ACTIVE(free)) {
        check_expected(ptr);
    } else {
        MOCK_FUNC_REAL(free)(ptr);
    }
}

MOCK_FUNC_VAR_NEW(memcpy);
void *MOCK_FUNC_WRAP(memcpy)(void *destination, const void *source, size_t num) {
    if (MOCK_IS_ACTIVE(memcpy)) {
        check_expected(destination);
        check_expected(source);
        check_expected(num);
        return mock_ptr_type(void *);
    }
    return MOCK_FUNC_REAL(memcpy)(destination, source, num);
}

MOCK_FUNC_VAR_NEW(strdup);
char *MOCK_FUNC_WRAP(strdup)(const char *string) {
    if (MOCK_IS_ACTIVE(strdup)) {
        check_expected(string);
        return mock_ptr_type(char *);
    }
    return MOCK_FUNC_REAL(strdup)(string);
}

MOCK_FUNC_VAR_NEW(strndup);
char *MOCK_FUNC_WRAP(strndup)(const char *string, size_t n) {
    if (MOCK_IS_ACTIVE(strndup)) {
        check_expected(string);
        check_expected(n);
        return mock_ptr_type(char *);
    }
    return MOCK_FUNC_REAL(strndup)(string, n);
}

MOCK_FUNC_VAR_NEW(fopen);
FILE *MOCK_FUNC_WRAP(fopen)(const char *filename, const char *mode) {
    if (MOCK_IS_ACTIVE(fopen)) {
        check_expected_ptr(filename);
        check_expected_ptr(mode);
        return mock_ptr_type(FILE *);
    }
    return MOCK_FUNC_REAL(fopen)(filename, mode);
}

MOCK_FUNC_VAR_NEW(fdopen);
FILE *MOCK_FUNC_WRAP(fdopen)(const int fd, const char *mode) {
    if (MOCK_IS_ACTIVE(fdopen)) {
        check_expected(fd);
        check_expected_ptr(mode);
        return mock_ptr_type(FILE *);
    }
    return MOCK_FUNC_REAL(fdopen)(fd, mode);
}

MOCK_FUNC_VAR_NEW(fclose);
int MOCK_FUNC_WRAP(fclose)(FILE *stream) {
    if (MOCK_IS_ACTIVE(fclose)) {
        check_expected_ptr(stream);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(fclose)(stream);
}

MOCK_FUNC_VAR_NEW(fwrite);
size_t MOCK_FUNC_WRAP(fwrite)(const void *ptr, size_t size, size_t count, FILE *stream) {
    if (MOCK_IS_ACTIVE(fwrite)) {
        check_expected_ptr(ptr);
        check_expected(size);
        check_expected(count);
        check_expected_ptr(stream);
        return mock_type(size_t);
    }
    return MOCK_FUNC_REAL(fwrite)(ptr, size, count, stream);
}

MOCK_FUNC_VAR_NEW(fread);
size_t MOCK_FUNC_WRAP(fread)(void *ptr, size_t size, size_t count, FILE *stream) {
    if (MOCK_IS_ACTIVE(fread)) {
        check_expected(ptr);
        check_expected(size);
        check_expected(count);
        check_expected_ptr(stream);
        return mock_type(size_t);
    }
    return MOCK_FUNC_REAL(fread)(ptr, size, count, stream);
}

MOCK_FUNC_VAR_NEW(scandir);
int MOCK_FUNC_WRAP(scandir)(const char *dirp, struct dirent ***namelist, int (*filter)(const struct dirent *),
                            int (*compar)(const struct dirent **, const struct dirent **)) {
    if (MOCK_IS_ACTIVE(scandir)) {
        check_expected_ptr(dirp);
        check_expected_ptr(namelist);
        check_expected_ptr(filter);
        check_expected_ptr(compar);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(scandir)(dirp, namelist, filter, compar);
}

MOCK_FUNC_VAR_NEW(access);
int MOCK_FUNC_WRAP(access)(const char *pathname, int mode) {
    if (MOCK_IS_ACTIVE(access)) {
        check_expected_ptr(pathname);
        check_expected(mode);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(access)(pathname, mode);
}

MOCK_FUNC_VAR_NEW(fseek);
int MOCK_FUNC_WRAP(fseek)(FILE *stream, long int offset, int origin) {
    if (MOCK_IS_ACTIVE(fseek)) {
        check_expected_ptr(stream);
        check_expected(offset);
        check_expected(origin);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(fseek)(stream, offset, origin);
}

MOCK_FUNC_VAR_NEW(rewind);
void MOCK_FUNC_WRAP(rewind)(FILE *stream) {
    if (MOCK_IS_ACTIVE(rewind)) {
        check_expected_ptr(stream);
    }
}

MOCK_FUNC_VAR_NEW(ftell);
long int MOCK_FUNC_WRAP(ftell)(FILE *stream) {
    if (MOCK_IS_ACTIVE(ftell)) {
        check_expected_ptr(stream);
        return mock_type(long);
    }
    return MOCK_FUNC_REAL(ftell)(stream);
}

MOCK_FUNC_VAR_NEW(popen);
FILE *MOCK_FUNC_WRAP(popen)(const char *command, const char *type) {
    if (MOCK_IS_ACTIVE(popen)) {
        check_expected_ptr(command);
        check_expected_ptr(type);
        return mock_ptr_type(FILE *);
    }
    return MOCK_FUNC_REAL(popen)(command, type);
}

MOCK_FUNC_VAR_NEW(pclose);
int MOCK_FUNC_WRAP(pclose)(FILE *stream) {
    if (MOCK_IS_ACTIVE(pclose)) {
        check_expected_ptr(stream);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(pclose)(stream);
}

MOCK_FUNC_VAR_NEW(getline);
ssize_t MOCK_FUNC_WRAP(getline)(char **lineptr, size_t *n, FILE *stream) {
    if (MOCK_IS_ACTIVE(getline)) {
        check_expected_ptr(lineptr);
        check_expected(n);
        check_expected_ptr(stream);

        *lineptr = mock_ptr_type(char *);
        return mock_type(ssize_t);
    }
    return MOCK_FUNC_REAL(getline)(lineptr, n, stream);
}

MOCK_FUNC_VAR_NEW(getenv);
char *MOCK_FUNC_WRAP(getenv)(const char *name) {
    if (MOCK_IS_ACTIVE(getenv)) {
        check_expected_ptr(name);
        return mock_ptr_type(char *);
    }
    return MOCK_FUNC_REAL(getenv)(name);
}

MOCK_FUNC_VAR_NEW(getpid);
pid_t MOCK_FUNC_WRAP(getpid)() {
    if (MOCK_IS_ACTIVE(getpid)) {
        return mock_type(pid_t);
    }
    return MOCK_FUNC_REAL(getpid)();
}

MOCK_FUNC_VAR_NEW(opendir);
DIR *MOCK_FUNC_WRAP(opendir)(const char *name) {
    if (MOCK_IS_ACTIVE(opendir)) {
        check_expected_ptr(name);
        return mock_type(DIR *);
    }
    return MOCK_FUNC_REAL(opendir)(name);
}

MOCK_FUNC_VAR_NEW(closedir);
int MOCK_FUNC_WRAP(closedir)(DIR *dirp) {
    if (MOCK_IS_ACTIVE(closedir)) {
        check_expected_ptr(dirp);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(closedir)(dirp);
}

MOCK_FUNC_VAR_NEW(remove);
int MOCK_FUNC_WRAP(remove)(const char *filename) {
    if (MOCK_IS_ACTIVE(remove)) {
        check_expected_ptr(filename);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(remove)(filename);
}

MOCK_FUNC_VAR_NEW(readdir);
struct dirent *MOCK_FUNC_WRAP(readdir)(DIR *dirp) {
    if (MOCK_IS_ACTIVE(readdir)) {
        check_expected_ptr(dirp);
        return mock_type(struct dirent *);
    }
    return MOCK_FUNC_REAL(readdir)(dirp);
}

MOCK_FUNC_VAR_NEW(stat);
int MOCK_FUNC_WRAP(stat)(const char *pathname, struct stat *statbuf) {
    if (MOCK_IS_ACTIVE(stat)) {
        check_expected_ptr(pathname);
        check_expected_ptr(statbuf);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(stat)(pathname, statbuf);
}

MOCK_FUNC_VAR_NEW(fputc);
int MOCK_FUNC_WRAP(fputc)(int character, FILE *stream) {
    if (MOCK_IS_ACTIVE(fputc)) {
        check_expected(character);
        check_expected_ptr(stream);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(fputc)(character, stream);
}

MOCK_FUNC_VAR_NEW(time);
time_t MOCK_FUNC_WRAP(time)(time_t *timer) {
    if (MOCK_IS_ACTIVE(time)) {
        check_expected_ptr(timer);
        if (timer != NULL) {
            *timer = mock_type(time_t);
        }
        return mock_type(time_t);
    }
    return MOCK_FUNC_REAL(time)(timer);
}

MOCK_FUNC_VAR_NEW(clock_gettime);
int MOCK_FUNC_WRAP(clock_gettime)(clockid_t clock_id, struct timespec *tp) {
    if (MOCK_IS_ACTIVE(clock_gettime)) {
        check_expected(clock_id);
        check_expected_ptr(tp);
        memcpy(tp, mock_ptr_type(struct timespec *), sizeof(struct timespec));
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(clock_gettime)(clock_id, tp);
}

MOCK_FUNC_VAR_NEW(inet_aton);
int MOCK_FUNC_WRAP(inet_aton)(const char *cp, struct in_addr *inp) {
    if (MOCK_IS_ACTIVE(inet_aton)) {
        check_expected_ptr(cp);
        check_expected_ptr(inp);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(inet_aton)(cp, inp);
}

MOCK_FUNC_VAR_NEW(inet_pton);
int MOCK_FUNC_WRAP(inet_pton)(int af, const char *cp, void *buf) {
    if (MOCK_IS_ACTIVE(inet_pton)) {
        check_expected(af);
        check_expected_ptr(cp);
        check_expected_ptr(buf);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(inet_pton)(af, cp, buf);
}

MOCK_FUNC_VAR_NEW(getaddrinfo);
int MOCK_FUNC_WRAP(getaddrinfo)(const char *node, const char *service, const struct addrinfo *hints,
                                struct addrinfo **res) {
    if (MOCK_IS_ACTIVE(getaddrinfo)) {
        check_expected_ptr(node);
        check_expected_ptr(service);
        check_expected_ptr(hints);
        *res = mock_ptr_type(struct addrinfo *);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(getaddrinfo)(node, service, hints, res);
}

MOCK_FUNC_VAR_NEW(freeaddrinfo);
void MOCK_FUNC_WRAP(freeaddrinfo)(struct addrinfo *res) {
    if (MOCK_IS_ACTIVE(freeaddrinfo)) {
        check_expected_ptr(res);
    } else {
        MOCK_FUNC_REAL(freeaddrinfo)(res);
    }
}

MOCK_FUNC_VAR_NEW(socket);
int MOCK_FUNC_WRAP(socket)(int domain, int type, int protocol) {
    if (MOCK_IS_ACTIVE(socket)) {
        check_expected(domain);
        check_expected(type);
        check_expected(protocol);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(socket)(domain, type, protocol);
}

MOCK_FUNC_VAR_NEW(setsockopt);
int MOCK_FUNC_WRAP(setsockopt)(int fd, int level, int optname, const void *optval, socklen_t optlen) {
    if (MOCK_IS_ACTIVE(setsockopt)) {
        check_expected(fd);
        check_expected(level);
        check_expected(optname);
        check_expected_ptr(optval);
        check_expected(optlen);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(setsockopt)(fd, level, optname, optval, optlen);
}

MOCK_FUNC_VAR_NEW(accept);
int MOCK_FUNC_WRAP(accept_errno = 0);
int MOCK_FUNC_WRAP(accept)(int fd, __SOCKADDR_ARG addr, socklen_t *len) {
    if (MOCK_IS_ACTIVE(accept)) {
        check_expected(fd);
        check_expected_ptr(addr);
        check_expected(len);
        if (MOCK_FUNC_WRAP(accept_errno) != 0) {
            errno = MOCK_FUNC_WRAP(accept_errno);
            MOCK_FUNC_WRAP(accept_errno) = 0;
        }
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(accept)(fd, addr, len);
}

MOCK_FUNC_VAR_NEW(connect);
int MOCK_FUNC_WRAP(connect)(int fd, const struct sockaddr *addr, socklen_t len) {
    if (MOCK_IS_ACTIVE(connect)) {
        check_expected(fd);
        check_expected_ptr(addr);
        check_expected(len);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(connect)(fd, addr, len);
}

MOCK_FUNC_VAR_NEW(bind);
int MOCK_FUNC_WRAP(bind)(int fd, __CONST_SOCKADDR_ARG addr, socklen_t len) {
    if (MOCK_IS_ACTIVE(bind)) {
        check_expected(fd);
        check_expected(addr);
        check_expected(len);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(bind)(fd, addr, len);
}

MOCK_FUNC_VAR_NEW(listen);
int MOCK_FUNC_WRAP(listen)(int fd, int n) {
    if (MOCK_IS_ACTIVE(listen)) {
        check_expected(fd);
        check_expected(n);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(listen)(fd, n);
}

MOCK_FUNC_VAR_NEW(close);
int MOCK_FUNC_WRAP(close)(int fd) {
    if (MOCK_IS_ACTIVE(close)) {
        check_expected(fd);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(close)(fd);
}

MOCK_FUNC_VAR_NEW(open);
int MOCK_FUNC_WRAP(open)(char *file, int flags, mode_t mode) {
    if (MOCK_IS_ACTIVE(open)) {
        check_expected(file);
        check_expected(flags);
        check_expected(mode);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(open)(file, flags, mode);
}

MOCK_FUNC_VAR_NEW(pselect);
int MOCK_FUNC_WRAP(pselect_errno = 0);
int MOCK_FUNC_WRAP(pselect)(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
                            const struct timespec *timeout, const __sigset_t *sigmask) {
    if (MOCK_IS_ACTIVE(pselect)) {
        check_expected(nfds);
        check_expected_ptr(readfds);
        check_expected_ptr(writefds);
        check_expected_ptr(exceptfds);
        check_expected_ptr(timeout);
        check_expected_ptr(sigmask);
        if (MOCK_FUNC_WRAP(pselect_errno) != 0) {
            errno = MOCK_FUNC_WRAP(pselect_errno);
            MOCK_FUNC_WRAP(pselect_errno) = 0;
        }
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(pselect)(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

MOCK_FUNC_VAR_NEW(raise);
int MOCK_FUNC_WRAP(raise)(int __sig) {
    if (MOCK_IS_ACTIVE(raise)) {
        check_expected(__sig);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(raise)(__sig);
}

MOCK_FUNC_VAR_NEW(pthread_create);
int MOCK_FUNC_WRAP(pthread_create)(pthread_t *__newthread, const pthread_attr_t *__attr,
                                   void *(*__start_routine)(void *), void *__arg) {
    int result;

    switch (MOCK_GET_TYPE(pthread_create)) {
        case CMOCKA_MOCK_ENABLED_WITH_FUNC:
            result = MOCK_FUNC_WITH(pthread_create)(__newthread, __attr, __start_routine, __arg);
            break;
        case CMOCKA_MOCK_ENABLED:
            check_expected_ptr(__newthread);
            check_expected_ptr(__attr);
            check_expected_ptr(__start_routine);
            check_expected_ptr(__arg);
            result = mock_type(int);
            break;
        default:
            result = MOCK_FUNC_REAL(pthread_create)(__newthread, __attr, __start_routine, __arg);
            break;
    }

    return result;
}

MOCK_FUNC_VAR_NEW(pthread_once);
int MOCK_FUNC_WRAP(pthread_once)(pthread_once_t *__once_control, void (*__init_routine)(void)) {
    if (MOCK_IS_ACTIVE(pthread_once)) {
        check_expected_ptr(__once_control);
        check_expected_ptr(__init_routine);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(pthread_once)(__once_control, __init_routine);
}

MOCK_FUNC_VAR_NEW(pthread_join);
int MOCK_FUNC_WRAP(pthread_join)(pthread_t __th, void **__thread_return) {
    if (MOCK_IS_ACTIVE(pthread_join)) {
        check_expected(__th);
        check_expected_ptr(__thread_return);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(pthread_join)(__th, __thread_return);
}

MOCK_FUNC_VAR_NEW(pthread_mutex_init);
int MOCK_FUNC_WRAP(pthread_mutex_init)(pthread_mutex_t *__mutex, const pthread_mutexattr_t *__mutexattr) {
    if (MOCK_IS_ACTIVE(pthread_mutex_init)) {
        check_expected_ptr(__mutex);
        check_expected_ptr(__mutexattr);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(pthread_mutex_init)(__mutex, __mutexattr);
}

MOCK_FUNC_VAR_NEW(pthread_mutex_destroy);
int MOCK_FUNC_WRAP(pthread_mutex_destroy)(pthread_mutex_t *__mutex) {
    if (MOCK_IS_ACTIVE(pthread_mutex_destroy)) {
        check_expected_ptr(__mutex);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(pthread_mutex_destroy)(__mutex);
}

MOCK_FUNC_VAR_NEW(pthread_mutex_trylock);
int MOCK_FUNC_WRAP(pthread_mutex_trylock)(pthread_mutex_t *__mutex) {
    if (MOCK_IS_ACTIVE(pthread_mutex_trylock)) {
        check_expected_ptr(__mutex);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(pthread_mutex_trylock)(__mutex);
}

MOCK_FUNC_VAR_NEW(pthread_mutex_lock);
int MOCK_FUNC_WRAP(pthread_mutex_lock)(pthread_mutex_t *__mutex) {
    if (MOCK_IS_ACTIVE(pthread_mutex_lock)) {
        check_expected_ptr(__mutex);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(pthread_mutex_lock)(__mutex);
}

MOCK_FUNC_VAR_NEW(pthread_mutex_unlock);
int MOCK_FUNC_WRAP(pthread_mutex_unlock)(pthread_mutex_t *__mutex) {
    if (MOCK_IS_ACTIVE(pthread_mutex_unlock)) {
        check_expected_ptr(__mutex);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(pthread_mutex_unlock)(__mutex);
}

MOCK_FUNC_VAR_NEW(pthread_mutex_timedlock);
int MOCK_FUNC_WRAP(pthread_mutex_timedlock)(pthread_mutex_t *__mutex, const struct timespec *__abstime) {
    if (MOCK_IS_ACTIVE(pthread_mutex_timedlock)) {
        check_expected_ptr(__mutex);
        check_expected_ptr(__abstime);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(pthread_mutex_timedlock)(__mutex, __abstime);
}

MOCK_FUNC_VAR_NEW(sem_init);
int MOCK_FUNC_WRAP(sem_init)(sem_t *__sem, int __pshared, unsigned int __value) {
    if (MOCK_IS_ACTIVE(sem_init)) {
        check_expected_ptr(__sem);
        check_expected(__pshared);
        check_expected(__value);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(sem_init)(__sem, __pshared, __value);
}

MOCK_FUNC_VAR_NEW(sem_post);
int MOCK_FUNC_WRAP(sem_post)(sem_t *__sem) {
    if (MOCK_IS_ACTIVE(sem_post)) {
        check_expected_ptr(__sem);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(sem_post)(__sem);
}

MOCK_FUNC_VAR_NEW(sem_destroy);
int MOCK_FUNC_WRAP(sem_destroy)(sem_t *__sem) {
    if (MOCK_IS_ACTIVE(sem_destroy)) {
        check_expected_ptr(__sem);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(sem_destroy)(__sem);
}

MOCK_FUNC_VAR_NEW(sem_timedwait);
int MOCK_FUNC_WRAP(sem_timedwait)(sem_t *__sem, const struct timespec *__abstime) {
    if (MOCK_IS_ACTIVE(sem_timedwait)) {
        check_expected_ptr(__sem);
        check_expected_ptr(__abstime);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(sem_timedwait)(__sem, __abstime);
}

MOCK_FUNC_VAR_NEW(eventfd);
int MOCK_FUNC_WRAP(eventfd)(unsigned int __count, int __flags) {
    if (MOCK_IS_ACTIVE(eventfd)) {
        check_expected(__count);
        check_expected(__flags);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(eventfd)(__count, __flags);
}

MOCK_FUNC_VAR_NEW(eventfd_read);
int MOCK_FUNC_WRAP(eventfd_read)(int __fd, eventfd_t *__value) {
    if (MOCK_IS_ACTIVE(eventfd_read)) {
        check_expected(__fd);
        check_expected_ptr(__value);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(eventfd_read)(__fd, __value);
}

MOCK_FUNC_VAR_NEW(eventfd_write);
int MOCK_FUNC_WRAP(eventfd_write)(int __fd, eventfd_t __value) {
    if (MOCK_IS_ACTIVE(eventfd_write)) {
        check_expected(__fd);
        check_expected(__value);
        return mock_type(int);
    }
    return MOCK_FUNC_REAL(eventfd_write)(__fd, __value);
}
