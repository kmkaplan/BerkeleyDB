/* DO NOT EDIT: automatically built by dist/distrib. */
int __db_abspath __P((const char *));
void *__db_calloc __P((size_t, size_t));
void *__db_malloc __P((size_t));
void *__db_realloc __P((void *, size_t));
int __os_dirlist __P((const char *, char ***, int *));
void __os_dirfree __P((char **, int));
int __db_fileid __P((DB_ENV *, const char *, int, u_int8_t *));
int __db_fsync __P((int));
int __os_map __P((int, size_t, int, int, void **));
int __os_unmap __P((void *, size_t));
int __db_oflags __P((int));
int __db_open __P((const char *, int, int, int, int *));
int __db_close __P((int));
char *__db_rpath __P((const char *));
int __db_read __P((int, void *, size_t, ssize_t *));
int __db_write __P((int, void *, size_t, ssize_t *));
int __os_seek __P((int, size_t, db_pgno_t, u_long, int));
int __os_sleep __P((u_long, u_long));
int __os_spin __P((void));
int __os_exists __P((const char *, int *));
int __os_ioinfo __P((const char *, int, off_t *, off_t *));
int __db_unlink __P((const char *));
