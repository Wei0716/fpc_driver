#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include "sys_hook.h"
#include "hooks.h"

struct sys_hook *lkh_sys_hook;

/* Module parameter macros */
static char *kbase32 = "ffffffff82001580", *kbase64 = "ffffffff820001c0";
module_param(kbase32, charp, 0);
MODULE_PARM_DESC(kbase32, "Base address of the x86 syscall table, in hex");
module_param(kbase64, charp, 0);
MODULE_PARM_DESC(kbase64, "Base address of the x64 syscall table, in hex");

unsigned long *
get_sys_call_table(unsigned int syscall_nr, unsigned long syscall_addr)
{
    unsigned long *entry = (unsigned long *)0xffffffff81000000;

    for (;(unsigned long)entry < 0xffffffff81000000 + 32*0x100000; entry += 1) {

        if (entry[syscall_nr] == syscall_addr) {
        printk("entry at %lx, %pK, syscall addr %lx, mem addr %lx\n",
                (unsigned long)entry, entry, syscall_addr, entry[syscall_nr]);
            return entry;
        }
    }

    return NULL;
}

static int __init
module_entry(void)
{
    unsigned long syscall_close_addr;
    unsigned long *syscall_table_addr = NULL;
    unsigned long *real_table = (unsigned long *)0xffffffff820001c0;

    printk(KERN_INFO "lkh initializing...\n");

    syscall_close_addr = kallsyms_lookup_name("__x64_sys_close");

    if (0 == syscall_close_addr) {
        printk(KERN_INFO "failed to get syscall close addr\n");
        return -1;
    }

    printk("sys_close at %lx\n", syscall_close_addr);

    printk("kernel begin %lx, close at table is %lx, %lx, %lx, %lx\n",
            PAGE_OFFSET, 
            real_table[0], 
            real_table[1], 
            real_table[2], 
            real_table[3]);
    syscall_table_addr = get_sys_call_table(__NR_close, syscall_close_addr);

    if (NULL == syscall_table_addr) {
        printk(KERN_INFO "failed to get x86 syscall table\n");
        return -1;
    }
    printk("sys_close at %lx, table at %lx\n",
            syscall_close_addr, syscall_table_addr);

    if ((lkh_sys_hook = sys_hook_init(0, (uintptr_t)syscall_table_addr)) == NULL) {
        printk(KERN_INFO "failed to initialize sys_hook\n");
        return -1;
    }

    sys_hook_add64(lkh_sys_hook, __NR_mkdir, (void *)mkdir_hook);
    sys_hook_add64(lkh_sys_hook, __NR_unlink, (void *)unlink_hook);
    sys_hook_add64(lkh_sys_hook, __NR_unlinkat, (void *)unlinkat_hook);

    printk(KERN_INFO "lkh loaded\n");
    return 0;
}

static void __exit
module_cleanup(void)
{
    sys_hook_free(lkh_sys_hook);
    printk(KERN_INFO "lkh has finished\n");
}

/* Declare the entry and exit points of our module */
module_init(module_entry);
module_exit(module_cleanup);

/* Shut up kernel warnings about tainted kernels due to non-free software */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("github:jha");
MODULE_DESCRIPTION("Linux Kernel Hook");
