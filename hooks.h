#pragma once

#include <linux/syscalls.h>
#include "types.h"

typedef asmlinkage long (*sys_mkdir_t)(const char __user *, int);
typedef asmlinkage long (*sys_unlink_t)(const char __user *);
typedef asmlinkage long (*sys_unlinkat_t)(int, const char __user *, int);

asmlinkage long mkdir_hook(const char __user *, int);
asmlinkage long unlink_hook(const char __user *);
asmlinkage long unlinkat_hook(int, const char __user *, int);
