#pragma once

#include <linux/syscalls.h>
#include "types.h"

typedef asmlinkage int (*sys_mkdir_t)(const char *, int);
typedef asmlinkage int (*sys_unlink_t)(const char *);
typedef asmlinkage int (*sys_unlinkat_t)(int, const char *, int);

asmlinkage int mkdir_hook(const char *, int);
asmlinkage int unlink_hook(const char *);
asmlinkage int unlinkat_hook(int dfd, const char *path, int flags);
