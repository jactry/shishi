extern int _shisa_isdir (const char *path);
extern int _shisa_isdir2 (const char *path1, const char *path2);
extern int _shisa_isdir3 (const char *path1, const char *path2,
			  const char *path3);
extern int _shisa_isdir4 (const char *path1, const char *path2,
			  const char *path3, const char *path4);
extern int _shisa_mkdir (const char *file);
extern int _shisa_mkdir2 (const char *path1, const char *path2);
extern int _shisa_mkdir3 (const char *path1, const char *path2,
			  const char *path3);
extern int _shisa_mkdir4 (const char *path1, const char *path2,
			  const char *path3, const char *path4);
extern int _shisa_rmdir2 (const char *path1, const char *path2);
extern int _shisa_rmdir3 (const char *path1, const char *path2,
			  const char *path3);
extern int _shisa_mtime4 (const char *path1, const char *path2,
			  const char *path3, const char *path4);
extern int _shisa_isfile4 (const char *path1, const char *path2,
			   const char *path3, const char *path4);
extern int _shisa_uint32link4 (const char *path1, const char *path2,
			       const char *path3, const char *path4);
extern int _shisa_ls (const char *path, char ***files, size_t *nfiles);
extern int _shisa_ls2 (const char *path1, const char *path2,
		       char ***files, size_t *nfiles);
extern int _shisa_ls4 (const char *path1, const char *path2,
		       const char *path3, const char *path4,
		       char ***files, size_t *nfiles);
extern int _shisa_lsdir (const char *path, char ***files, size_t *nfiles);
extern int _shisa_lsdir2 (const char *path1, const char *path2,
			  char ***files, size_t *nfiels);