#ifndef IO_H
#define IO_H

// io.h stub for cross-platform compatibility
// Provides file I/O functions that work on both Linux and Windows

#ifdef _WIN32
// Windows: use real io.h and fcntl.h from Windows SDK
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <direct.h>
#else
// Linux/GCC: use POSIX headers
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

inline int _unlink(const char* pathname) {
    return unlink(pathname);
}

inline int _mkdir(const char* pathname) {
    return mkdir(pathname, 0755);
}

inline int _rmdir(const char* pathname) {
    return rmdir(pathname);
}

inline int _getcwd(char* buf, size_t size) {
    return (getcwd(buf, size) != NULL) ? 0 : -1;
}

inline int _chdir(const char* pathname) {
    return chdir(pathname);
}

#endif // _WIN32

#endif // IO_H
