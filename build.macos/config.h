/* config.h.  Generated automatically by configure.  */
/* config.h.in.  Generated automatically from configure.in by autoheader.  */

/* Define to empty if the keyword does not work.  */
/* #undef const */

/* Define if you have a working `mmap' system call.  */
#define HAVE_MMAP 1

/* Define if your struct stat has st_blksize.  */
#define HAVE_ST_BLKSIZE 1

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef mode_t */

/* Define to `long' if <sys/types.h> doesn't define.  */
/* #undef off_t */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef pid_t */

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #undef size_t */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if your processor stores words with the most significant
   byte first (like Motorola and SPARC, unlike Intel and VAX).  */
#define WORDS_BIGENDIAN 1

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef ssize_t */

/* Define if you want a debugging version. */
/* #undef DEBUG */

/* Define if you have sigfillset (and sigprocmask). */
/* #undef HAVE_SIGFILLSET 1*/

/* Define if your off_t isn't 64-bit and you have the _llseek() call. */
/* #undef HAVE_LLSEEK */

/* Define if your off_t isn't 64-bit and you have the _lseeki64() call. */
/* #undef HAVE_LSEEKI */

/* Define if you have spinlocks. */
#define HAVE_SPINLOCKS	1

/* Define if you have <sys/select.h> */
/* #undef HAVE_SYS_SELECT_H */

/* Define if your sprintf returns a pointer, not a length. */
/* #undef SPRINTF_RET_CHARPNT */

/* Define if you have the getcwd function.  */
#define HAVE_GETCWD 1

/* Define if you have the geteuid function.  */
/* #undef HAVE_GETEUID */

/* Define if you have the getopt function.  */
/* #undef HAVE_GETOPT */

/* Define if you have the getpagesize function.  */
/* #undef HAVE_GETPAGESIZE */

/* Define if you have the getuid function.  */
/* #undef HAVE_GETUID */

/* Define if you have the memcmp function.  */
#define HAVE_MEMCMP 1

/* Define if you have the memcpy function.  */
#define HAVE_MEMCPY 1

/* Define if you have the memmove function.  */
#define HAVE_MEMMOVE 1

/* Define if you have the opendir function.  */
#define HAVE_OPENDIR 1

/* Define if you have the select function.  */
#define HAVE_SELECT 1

/* Define if you have the snprintf function.  */
/* #undef HAVE_SNPRINTF */

/* Define if you have the strdup function.  */
/* #undef HAVE_STRDUP */

/* Define if you have the strerror function.  */
#define HAVE_STRERROR 1

/* Define if you have the strsep function.  */
/* #undef HAVE_STRSEP */

/* Define if you have the <dirent.h> header file.  */
#define HAVE_DIRENT_H 1

/* Define if you have the <ndir.h> header file.  */
/* #undef HAVE_NDIR_H */

/* Define if you have the <sys/dir.h> header file.  */
/* #undef HAVE_SYS_DIR_H */

/* Define if you have the <sys/ndir.h> header file.  */
/* #undef HAVE_SYS_NDIR_H */

/* Define if you have the <sys/select.h> header file.  */
/* #undef HAVE_SYS_SELECT_H */

/* Define if you have the <sys/time.h> header file.  */
/* #undef HAVE_SYS_TIME_H */

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

#include <GUSI.h>
#include <sys/errno.h>

#define open(a,b,c) (open)(a,b)
#define write(fd,buf,len) (write)(fd,(char*)(buf),len)
#define read(fd,buf,len) (read)(fd,(char*)(buf),len)
#define fcntl(fd,msg,arg) (fcntl)(fd,msg,(int) arg)

void * mmap(void * addr, size_t len, int prot, int flags, int fd, off_t offset);
int munmap(void * addr, size_t len);
