#ifndef IO_H
#define IO_H

// io.h stub for Linux/GCC builds
// Provides file I/O functions that replace Windows _open, _close, etc.

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define _O_RDONLY O_RDONLY
#define _O_WRONLY O_WRONLY
#define _O_CREAT O_CREAT
#define _O_TRUNC O_TRUNC
#define _S_IWRITE S_IWUSR
#define _S_IREAD S_IRUSR

inline int _open(const char* pathname, int flags, int mode = 0) {
    return open(pathname, flags, mode);
}

inline int _close(int fd) {
    return close(fd);
}

inline ssize_t _read(int fd, void* buf, size_t count) {
    return read(fd, buf, count);
}

inline ssize_t _write(int fd, const void* buf, size_t count) {
    return write(fd, buf, count);
}

inline off_t _lseek(int fd, off_t offset, int whence) {
    return lseek(fd, offset, whence);
}

inline int _chmod(const char* pathname, int mode) {
    return chmod(pathname, mode);
}

inline int _stat(const char* pathname, struct stat* buf) {
    return stat(pathname, buf);
}

inline int _access(const char* pathname, int mode) {
    return access(pathname, mode);
}

#endif // IO_H
