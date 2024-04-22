// #include <unistd.h>
//
// extern "C"
// {
//   off_t _lseek(int, off_t, int)
//   {
//     return 0;
//   }
//
//   ssize_t __attribute__((weak)) _read(int fd, void *buf, size_t size)
//   {
//     return 0;
//   }
//
//   ssize_t _write(int fd, const char *buf, size_t count)
//   {
//     return 0;
//   }
// }
