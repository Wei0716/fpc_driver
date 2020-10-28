#include <linux/module.h>
#include "hooks.h"
#include "sys_hook.h"

extern struct sys_hook *fpc_sys_hook;
extern struct inode *fpc_inode;

asmlinkage long mkdir_hook(const char __user *path, int mode)
{
    sys_mkdir_t sys_mkdir;
    struct file *cur_file;
    struct dentry *parent_entry;
    struct inode *parent_inode;

    sys_mkdir = (sys_mkdir_t)sys_hook_get_orig64(fpc_sys_hook, __NR_mkdir);
    printk(KERN_INFO "we enter the mkdir hook, path is %s\n",
                path);

    cur_file = filp_open(path, O_RDONLY, 0);
    if (IS_ERR(cur_file))
        return sys_mkdir(path, mode);

    printk(KERN_INFO "cur file at %lx\n", cur_file);
    parent_entry = cur_file->f_path.dentry->d_parent;
    parent_inode = parent_entry->d_inode;
    printk(KERN_INFO "cur parent inode at %lx\n", parent_inode);
    filp_close(cur_file, NULL);

    if (parent_inode == fpc_inode)
        return -EPERM;
    else
        return sys_mkdir(path, mode);
}

asmlinkage long unlink_hook(const char __user *path)
{
    sys_unlink_t sys_unlink;
    struct file *cur_file;
    struct dentry *parent_entry;
    struct inode *parent_inode;

    
    sys_unlink = (sys_unlink_t)sys_hook_get_orig64(fpc_sys_hook, __NR_unlink);
    printk(KERN_INFO "we enter the unlink hook, path is %s, real call at %lx\n",
                path, (unsigned long)sys_unlink);

    cur_file = filp_open(path, O_RDONLY, 0);
    if (IS_ERR(cur_file))
        return sys_unlink(path);

    printk(KERN_INFO "cur file at %lx\n", cur_file);
    parent_entry = cur_file->f_path.dentry->d_parent;
    parent_inode = parent_entry->d_inode;
    printk(KERN_INFO "cur parent inode at %lx\n", parent_inode);
    filp_close(cur_file, NULL);

    if (parent_inode == fpc_inode)
        return -EINVAL;
    else
        return sys_unlink(path);
}

asmlinkage long unlinkat_hook(int dfd, const char __user *path, int flags)
{
    sys_unlinkat_t sys_unlinkat;
    struct file *cur_file;
    struct dentry *parent_entry;
    struct inode *parent_inode;
    
    sys_unlinkat = (sys_unlinkat_t)sys_hook_get_orig64(fpc_sys_hook, __NR_unlinkat);
    printk(KERN_INFO "we enter the unlinkat hook, path is %sreal call at %lx\n",
                path, (unsigned long)sys_unlinkat); 

    cur_file = filp_open(path, O_RDONLY, 0);
    if (IS_ERR(cur_file))
        return sys_unlinkat(dfd, path, flags);

    printk(KERN_INFO "cur file at %lx\n", cur_file);
    parent_entry = cur_file->f_path.dentry->d_parent;
    parent_inode = parent_entry->d_inode;
    printk(KERN_INFO "cur parent inode at %lx\n", parent_inode);
    filp_close(cur_file, NULL);

    if (parent_inode == fpc_inode)
        return -EINVAL;
    else
        return sys_unlinkat(dfd, path, flags);
}
