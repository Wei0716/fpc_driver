#include <linux/module.h>
#include "hooks.h"
#include "sys_hook.h"

extern struct sys_hook *lkh_sys_hook;

asmlinkage long mkdir_hook(const char __user *path, int mode)
{
    sys_mkdir_t sys_mkdir;
 
    sys_mkdir = (sys_mkdir_t)sys_hook_get_orig64(lkh_sys_hook, __NR_mkdir);
    printk(KERN_INFO "we enter the mkdir hook, real call at %lx\n",
                (unsigned long)sys_mkdir);

    return sys_mkdir(path, mode);

    return -EPERM;
    if (!strncmp(path, "/tmp/mytest", 11))
        return -EPERM;
    else
        return sys_mkdir(path, mode);
}

asmlinkage long unlink_hook(const char __user *path)
{
    sys_unlink_t sys_unlink;
    
    sys_unlink = (sys_unlink_t)sys_hook_get_orig64(lkh_sys_hook, __NR_unlink);
    printk(KERN_INFO "we enter the unlink hook, path is %s, real call at %lx\n",
                path, (unsigned long)sys_unlink);
    return sys_unlink(path);
    return -EINVAL;
    if (!strncmp(path, "/tmp/mytest", 11))
        return -EINVAL;
    else
        return sys_unlink(path);
}

asmlinkage long unlinkat_hook(int dfd, const char __user *path, int flags)
{
    sys_unlinkat_t sys_unlinkat;
    
    sys_unlinkat = (sys_unlinkat_t)sys_hook_get_orig64(lkh_sys_hook, __NR_unlinkat);
    printk(KERN_INFO "we enter the unlinkat hook, path is %sreal call at %lx\n",
                path, (unsigned long)sys_unlinkat); 
    return sys_unlinkat(dfd, path, flags);
    return -EINVAL;
    if (!strncmp(path, "/tmp/mytest", 11))
        return -EINVAL;
    else
        return sys_unlinkat(dfd, path, flags);
}
