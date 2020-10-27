#include <linux/module.h>
#include "hooks.h"
#include "sys_hook.h"

extern struct sys_hook *lkh_sys_hook;

asmlinkage int mkdir_hook(const char *path, int mode)
{
    sys_mkdir_t sys_mkdir;
    
    sys_mkdir = (sys_mkdir_t)sys_hook_get_orig64(lkh_sys_hook, __NR_mkdir);
    printk(KERN_INFO "we enter the mkdir hook\n");
    return -EPERM;
    //return sys_mkdir(path, mode);
}

asmlinkage int unlink_hook(const char *path)
{
    sys_unlink_t sys_unlink;
    
    sys_unlink = (sys_unlink_t)sys_hook_get_orig64(lkh_sys_hook, __NR_unlink);
    printk(KERN_INFO "we enter the unlink hook\n");
    return -EINVAL;
    //return sys_unlink(path);
}

asmlinkage int unlinkat_hook(int dfd, const char *path, int flags)
{
    sys_unlink_t sys_unlinkat;
    
    sys_unlinkat = (sys_unlink_t)sys_hook_get_orig64(lkh_sys_hook, __NR_unlinkat);
    printk(KERN_INFO "we enter the unlinkat hook\n");
    return -EINVAL;
    //return sys_unlink(dfd, path, flags);
}
